#include "../include/common.h"
#include "../include/serv_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BACKLOG_SIZE 20 // Tamanho do BACKLOG do listen feito pelo servidor

int is_id_available[MAX_CLIENTS]; // Variável global indicando quais IDS estão disponíveis para uso
blog_topic *list_head = NULL; // Variável global que indica o primeiro elemento da lista ligada de tópicos no blog

pthread_mutex_t id_mutex = PTHREAD_MUTEX_INITIALIZER; // Variável mutex para garantir exclusão mútua no acesso ao vetor de IDS disponíveis
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER; // Variável mutex para garantir exclusão mútua no acesso à lista ligada de topicos
pthread_mutex_t socket_mutex[MAX_CLIENTS]; // Vetor de mutex, cada uma para garantir exclusão mútua na comunicação com um dado cliente


int main(int argc, char **argv) {
    /*
    Função main.
    Recebe os argumentos de entrada do servidor: protocolo (v4 ou v6), porto e nome do arquivo para ler o tabuleiro do servidor.
    Retorna 0 em caso de sucesso, e 1 em caso de erro.
    */
    if(argc != 3){
        exit(EXIT_FAILURE);
    }
    char *protocol = argv[1];
    char *portstr = argv[2];

    // Inicializa o endereço do servidor
    struct sockaddr_storage storage;
    if(parse_serv_addr(protocol, portstr, &storage) == -1){
        exit(EXIT_FAILURE); // Erro na entrada na linha de comando
    }

    // Inicializa o socket do servidor e evita que o endereço seja travado após o fechamento do servidor (enable = 1)
    int serv_sock = socket(storage.ss_family, SOCK_STREAM, 0); 
    int enable = 1;
    if(setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1){
        exit(EXIT_FAILURE); // Erro no enable do socket
    }

    // Bind e Listen do servidor
    struct sockaddr *serv_addr = (struct sockaddr *)(&storage);
    bind(serv_sock, serv_addr, sizeof(storage));

    if(listen(serv_sock, BACKLOG_SIZE) == -1){
        exit(EXIT_FAILURE); // Erro no Listen
    }
    
    // Inicializa o vetor indicando os ids disponíveis com o valor 1, i.e., todos estão disponíveis
    // Também inicializa o vetor de mutex de acesso aos sockets dos clientes
    int i;
    for(i = 0; i < MAX_CLIENTS; i++){
        is_id_available[i] = 1;
        pthread_mutex_init(&socket_mutex[i],NULL);
    }

    // Loop principal do servidor
    while (1) {
        // Aceita a conexão de um cliente
        struct sockaddr_storage clt_storage;
        struct sockaddr *clt_addr = (struct sockaddr *)(&clt_storage);
        socklen_t clt_addr_len = sizeof(clt_storage);

        int clt_sock = accept(serv_sock, clt_addr, &clt_addr_len);
        
        client_data *clt_data = malloc(sizeof(client_data));

        // Acessa o vetor de ids disponíveis, checa pelo id de menor valor disponível e então informa que tal id agora está indisponível
        pthread_mutex_lock(&id_mutex); // Acesso feito em exclusão mútua pois a variável é global
        i = 0;
        while(is_id_available[i] != 1 && i < MAX_CLIENTS){
            i++;
        }
        clt_data->id = i+1;
        is_id_available[i] = 0;
        pthread_mutex_unlock(&id_mutex);

        clt_data->sock = clt_sock;
        printf("client %d connected\n",clt_data->id);

        pthread_t clt_thread;
        pthread_create(&clt_thread,NULL,run_thread,(void*)clt_data); // Inicializa a thread do novo cliente
    }

    // Libera a lista ligada de topicos e destroi as mutex
    pthread_mutex_lock(&list_mutex);
    free_topic_list();
    pthread_mutex_unlock(&list_mutex);

    for(i = 0; i < MAX_CLIENTS; i++){
        pthread_mutex_destroy(&socket_mutex[i]);
    }
    pthread_mutex_destroy(&id_mutex);
    pthread_mutex_destroy(&list_mutex);

    exit(EXIT_SUCCESS);
}

