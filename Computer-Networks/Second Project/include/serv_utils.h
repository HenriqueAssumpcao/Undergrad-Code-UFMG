#ifndef SERV_UTILS_HEADER
#define SERV_UTILS_HEADER

#include "common.h"
#include <pthread.h>

// Constantes
#define MAX_CLIENTS 20 // Quantidade máxima de clientes que podem se inscriver em um tópico

// Struct representando os dados de um cliente que serão passados para uma thread.
typedef struct client_data{
    int sock;
    int id;
}client_data;

// Struct representando um tópico criado no blog. Essa struct será utilizada para criar uma lista ligada de tópicos.
typedef struct blog_topic blog_topic;
struct blog_topic{
    char name[TOPIC_SZ];
    client_data *subscribed_clts[MAX_CLIENTS];
    blog_topic *next; // Ponteiro para o próximo tópico
};

// Variáveis globais
extern int is_id_available[MAX_CLIENTS]; // Variável global indicando quais IDS estão disponíveis para uso
extern blog_topic *list_head; // Variável global que indica o primeiro elemento da lista ligada de tópicos no blog

extern pthread_mutex_t id_mutex; // Variável mutex para garantir exclusão mútua no acesso ao vetor de IDS disponíveis
extern pthread_mutex_t list_mutex; // Variável mutex para garantir exclusão mútua no acesso à lista ligada de topicos
extern pthread_mutex_t socket_mutex[MAX_CLIENTS]; // Vetor de mutex, cada uma para garantir exclusão mútua na comunicação com um dado cliente

// Funções de parsing de IP e Porto

/*
Função que inicializa o endereço do servidor.
Recebe duas strings representando o tipo de protocolo e o porto, respectivamente, e um struct sockaddr_storage.
Retorna 0 se o parsing foi bem sucedido, e -1 caso contrário.
*/
int parse_serv_addr(const char *proto, const char *portstr,
                         struct sockaddr_storage *storage);

// Funções de manipulação da lista ligada de tópicos

/*
Função que libera a memória alocada para a lista ligada de tópicos.
Não recebe nem retorna nada.
*/
void free_topic_list();

// Funções de threading

/*
Função de execução da thread de atendimento ao cliente.
Recebe um ponteiro void para os dados do cliente.
Não retorna nada.
*/
void *run_thread(void *args);



#endif