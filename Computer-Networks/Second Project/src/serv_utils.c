#include "../include/serv_utils.h"
#include "../include/common.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

// Funções de parsing de IP e Porto
int parse_serv_addr(const char *proto, const char *portstr,
                         struct sockaddr_storage *storage) {
    /*
    Função que inicializa o endereço do servidor.
    Recebe duas strings representando o tipo de protocolo e o porto, respectivamente, e um struct sockaddr_storage.
    Retorna 0 se o parsing foi bem sucedido, e -1 caso contrário.
    */
    uint16_t port = (uint16_t)atoi(portstr);
    if (port == 0) {
        return -1;
    }
    port = htons(port);

    memset(storage, 0, sizeof(*storage));
    if (strcmp(proto, "v4") == 0) {
        // Checa se o protocolo fornecido é o IPv4
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_addr.s_addr = INADDR_ANY;
        addr4->sin_port = port;
        return 0;
    } else if (strcmp(proto, "v6") == 0) {
        // Checa se o protocolo fornecido é o IPv6
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_addr = in6addr_any;
        addr6->sin6_port = port;
        return 0;
    }
    return -1;
}

// Funções de manipulação do tipo blog_topic e da lista ligada de tópicos
void init_topic(blog_topic *topic, char topic_name[TOPIC_SZ]){
    /*
    Função que inicializa um tópico que foi inserido no blog.
    Recebe um tópico e o nome do tópico.
    Não retorna nada.
    */
    // Inicializa o tópico da seguinte forma: o campo topic_name recebe o nome, o campo next recebe NULL, e os subscribed_clts recebem NULL.
    strcpy(topic->name,topic_name);
    topic->next = NULL;
    int i,j;
    for(i = 0; i < MAX_CLIENTS; i++){
        for(j = 0; j < MAX_CLIENTS; j++){
            topic->subscribed_clts[j] = NULL;
        }
    }
}

blog_topic* search_and_insert_topic(char topic_name[TOPIC_SZ]){
    /*
    Procura por um tópico na lista ligada de tópicos a partir de um nome.
    Se o tópico estiver presente na lista, retorna um ponteiro para o tópico. Se não, cria um novo tópico com o nome fornecido como entrada.
    Recebe um nome de tópico.
    Retorna um ponteiro para blog_topic.
    */
    if(list_head == NULL){ // Se a lista esta vazia, preenche a cabeça da lista com o novo tópico
        list_head = malloc(sizeof(blog_topic));
        init_topic(list_head,topic_name);
        return list_head;
    }
    else{
        blog_topic *curr_topic = list_head;
        while(curr_topic != NULL){ // Caminha pela lista ligada
            if(strcmp(curr_topic->name,topic_name) == 0){ // Encontrou tópico na lista
                return curr_topic;
            }

            if(curr_topic->next == NULL){
                break;
            }
            curr_topic = curr_topic->next;
        }

        // Tópico não encontrado na lista, logo ele é criado e inserido no final da lista
        curr_topic->next = malloc(sizeof(blog_topic));
        init_topic(curr_topic->next,topic_name);

        return curr_topic->next;
    }
}

int subscribe_to_topic(char topic_name[TOPIC_SZ], client_data *clt_data){ 
    /*
    Função que inscreve um dado cliente em um tópico da lista.
    Recebe um nome de tópico e os dados de um cliente.
    Retorna 1 caso a inscrição seja bem-sucedida (tópico não-encontrado e portanto criado, ou tópico encontrado e cliente não inscrito),
    ou 0 em caso de erro (cliente já inscrito no tópico).
    */
    if(list_head == NULL){ // Se a lista esta vazia, preenche a cabeça da lista com o novo tópico
        list_head = malloc(sizeof(blog_topic));
        init_topic(list_head,topic_name);
        list_head->subscribed_clts[0] = clt_data;
        return 1;
    }
    else{
        blog_topic *curr_topic = list_head;
        int i,empty_slot;
        while(curr_topic != NULL){ // Caminha pela lista ligada
            if(strcmp(curr_topic->name,topic_name) == 0){ // Encontrou tópico na lista
                for(i = 0; i < MAX_CLIENTS; i++){ // Checa se o cliente já está inscrito
                    if(curr_topic->subscribed_clts[i] != NULL){
                        if(curr_topic->subscribed_clts[i]->id == clt_data->id){
                            return 0; // Erro: cliente já está inscrito no tópico.
                        }
                    }
                    else{ // Identifica o slot vazio para inserir o cliente no vetor de clientes inscritos do tópico
                        empty_slot = i;
                        break;
                    }
                }
                curr_topic->subscribed_clts[empty_slot] = clt_data; // Inscreve o cliente no tópico
                return 1;
            }

            if(curr_topic->next == NULL){
                break;
            }
            curr_topic = curr_topic->next;
        }

        // Tópico não encontrado na lista, logo ele é criado e inserido no final da lista, e o cliente é inscrito
        curr_topic->next = malloc(sizeof(blog_topic));
        init_topic(curr_topic->next,topic_name);
        curr_topic->next->subscribed_clts[0] = clt_data;
        return 1;
    }
}

int unsubscribe_to_topic(char topic_name[TOPIC_SZ], client_data *clt_data){
    /*
    Função que desinscreve um dado cliente de um tópico da lista.
    Recebe um nome de tópico e os dados de um cliente.
    Retorna 1 se o tópico foi encontrado e o cliente está inscrito no mesmo, e 0 caso contrário.
    */
    if(list_head != NULL){ // Checa se a lista está vazia
        blog_topic *curr_topic = list_head;
        int i;
        while(curr_topic != NULL){ // Caminha pela lista ligada
            if(strcmp(curr_topic->name,topic_name) == 0){ // Encontrou tópico na lista
                for(i = 0; i < MAX_CLIENTS; i++){ // Checa se o cliente está inscrito no tópico
                    if(curr_topic->subscribed_clts[i] != NULL){
                        if(curr_topic->subscribed_clts[i]->id == clt_data->id){
                            curr_topic->subscribed_clts[i] = NULL; // Desinscreve o cliente do tópico
                            return 1;
                        }
                    }
                }
                break;
            }
            if(curr_topic->next == NULL){
                break;
            }
            curr_topic = curr_topic->next;
        }
    }
    // Tópico não encontrado
    return 0;
}

void unsubscribe_all(client_data *clt_data){
    /*
    Função que desinscreve um dado cliente de todos os tópicos do blog.
    Recebe os dados de um cliente.
    Não retorna nada.
    */
    if(list_head != NULL){ // Checa se a lista está vazia
        blog_topic *curr_topic = list_head;
        int i;
        while(curr_topic != NULL){ // Caminha pela lista e checa se o cliente está inscrito nos tópicos, se sim o desinscreve.
            for(i = 0; i < MAX_CLIENTS; i++){ // Checa se o cliente está inscrito no tópico
                if(curr_topic->subscribed_clts[i] != NULL){
                    if(curr_topic->subscribed_clts[i]->id == clt_data->id){
                        curr_topic->subscribed_clts[i] = NULL; // Desinscreve o cliente do tópico
                    }
                }
            }
            curr_topic = curr_topic->next;
        }
    }
}

void list_topics(char buff[BUFSIZ]){
    /*
    Função que lista os tópicos presentes no blog.
    Recebe um buffer para armazenar os nomes dos tópicos.
    Não retorna nada.
    */
    if(list_head != NULL){ // Checa se existem tópicos
        blog_topic *curr_topic = list_head;
        int total = 0; // Total de bits já lidos
        while(curr_topic != NULL){ // Caminha pela lista e adiciona o nome do tópico atual para o buffer
            strcpy(buff + total,curr_topic->name);
            total += strlen(curr_topic->name);
            
            strcpy(buff + total,";");
            total += 1;
            
            curr_topic = curr_topic->next;
        }
    }
    else{
        sprintf(buff,"no topics available");
    }
}

void free_topic_list(){
    /*
    Função que libera a memória alocada para a lista ligada de tópicos.
    Não recebe nem retorna nada.
    */
    if(list_head != NULL){ 
        blog_topic* curr_topic;
        int i;
        while(list_head != NULL){ // Percorre a lista
            curr_topic = list_head;
            list_head = list_head->next; // Passa para o próximo tópico

            for(i = 0; i < MAX_CLIENTS; i++){ // Libera os ponteiros para clientes inscritos
                if(curr_topic->subscribed_clts[i] != NULL){
                    free(curr_topic->subscribed_clts[i]);
                }
            }
            free(curr_topic); // Libera o tópico atual
        }
    }
}

// Funcoes auxiliares
int process_op(BlogOperation *op, client_data *clt_data){
    /*
    Função que processa uma operação do cliente.
    Recebe uma operação e os dados do cliente.
    Retorna 1 caso o cliente queira se desconectar ou caso tenha ocorrido erro de recebimento de dados, e 0 caso contrário.
    */
    if(op->operation_type == ERROR_TYPE_ID){ // Se ocorreu erro na recepção de dados
        // Desinscreve o cliente de todos os tópicos e libera seu id (usando as respectivas mutex), e então sai do loop e fecha a conexão
        pthread_mutex_lock(&list_mutex);
        unsubscribe_all(clt_data);
        pthread_mutex_unlock(&list_mutex);

        pthread_mutex_lock(&id_mutex);
        is_id_available[clt_data->id-1] = 1; // Fazemos ID - 1 pois os IDS começam do número 1, e os indices do vetor do 0
        pthread_mutex_unlock(&id_mutex);

        return 1;
    }
    else if(op->operation_type == DISCONNECT_ID){
        // Libera (em exclusão mútua) o id do cliente que desconectou, e então encerra o loop e desconecta o cliente
        pthread_mutex_lock(&list_mutex);
        unsubscribe_all(clt_data);
        pthread_mutex_unlock(&list_mutex);

        pthread_mutex_lock(&id_mutex);
        is_id_available[clt_data->id-1] = 1;
        pthread_mutex_unlock(&id_mutex);

        printf("client %d disconnected\n",clt_data->id);
        return 1;
    }
    else if(op->operation_type == NEW_CONNECTION_ID){ // Se for nova conexão, preenche a operação de resposta com valores padrão.
        op->client_id = clt_data->id;
        strcpy(op->topic,"");
        strcpy(op->content,"");
        op->server_response = 1;

        pthread_mutex_lock(&socket_mutex[clt_data->id-1]);
        send_op(op,&clt_data->sock);
        pthread_mutex_unlock(&socket_mutex[clt_data->id-1]);
    }
    else if(op->operation_type == NEW_POST_ID){
        printf("new post added in %s by %d\n",op->topic,clt_data->id);
        printf("%s\n",op->content);

        // Busca pelo tópico desejado (também utilizando a mutex da lista)
        pthread_mutex_lock(&list_mutex);
        blog_topic *tgt_topic = search_and_insert_topic(op->topic);
        pthread_mutex_unlock(&list_mutex);

        // Inicializa um buffer com a mensagem que será enviada para os clientes inscritos no tópico
        int i;
        char buff[BUFSIZ];
        memset(buff,0,sizeof(buff));

        sprintf(buff,"new post added in %s by %d\n%s",op->topic,clt_data->id,op->content);
        strcpy(op->content,buff);
        op->server_response = 1;
        // Envia a mensagem para todos os clientes inscritos 
        for(i = 0; i < MAX_CLIENTS; i++){
            if(tgt_topic->subscribed_clts[i] != NULL){
                pthread_mutex_lock(&socket_mutex[tgt_topic->subscribed_clts[i]->id-1]);
                send_op(op,&tgt_topic->subscribed_clts[i]->sock);
                pthread_mutex_unlock(&socket_mutex[tgt_topic->subscribed_clts[i]->id-1]);
            }
        }
    }
    else if(op->operation_type == SUBSCRIBE_ID){
        // Se inscreve no tópico recebido utilizando a mutex de acesso à lista
        pthread_mutex_lock(&list_mutex);
        int sub_result = subscribe_to_topic(op->topic,clt_data);
        pthread_mutex_unlock(&list_mutex);
        
        if(sub_result == 0){ // Erro se o cliente já está inscrito
            strcpy(op->content,"error: already subscribed");
            op->server_response = 1;

            pthread_mutex_lock(&socket_mutex[clt_data->id-1]);
            send_op(op,&clt_data->sock);
            pthread_mutex_unlock(&socket_mutex[clt_data->id-1]);
        }
        else{
            printf("client %d subscribed to %s\n",clt_data->id,op->topic);
        }
    }
    else if(op->operation_type == UNSUBSCRIBE_ID){
        // Se desinscreve no tópico recebido (também utilizando a mutex da lista)
        pthread_mutex_lock(&list_mutex);
        int unsub_result = unsubscribe_to_topic(op->topic,clt_data);
        pthread_mutex_unlock(&list_mutex);

        if(unsub_result == 1){ // Imprime somente se o cliente estava inscrito no tópico
            printf("client %d unsubscribed to %s\n",clt_data->id,op->topic);
        }
    }
    else if(op->operation_type == LIST_TOPICS_ID){
        // Lista os tópicos em formato de string e os armazena em um buffer, e então envia o buffer para o cliente
        char buff[BUFSIZ];
        memset(buff,0,sizeof(buff));

        pthread_mutex_lock(&list_mutex);
        list_topics(buff);
        pthread_mutex_unlock(&list_mutex);

        strcpy(op->content,buff);
        op->server_response = 1;
        
        pthread_mutex_lock(&socket_mutex[clt_data->id-1]);
        send_op(op,&clt_data->sock); // Envia mensagem para o cliente que requisitou
        pthread_mutex_unlock(&socket_mutex[clt_data->id-1]);
    }
    return 0;
}

// Funções de threading
void *run_thread(void *args){
    /*
    Função de execução da thread de atendimento ao cliente.
    Recebe um ponteiro void para os dados do cliente.
    Não retorna nada.
    */
    client_data *clt_data = (client_data*)args;
    BlogOperation op;
    int exit_loop; // Variável que vai identificar se houve erro ou se o cliente deseja se desconectar
    // Loop principal da função
    while(1){
        recv_op(&op,&clt_data->sock);

        exit_loop = process_op(&op,clt_data);
        if(exit_loop == 1){ // Caso ocorra erro de recebimento ou caso o cliente queira se desconectar, encerra o loop
            break;
        }
    }
    // Fecha o socket do cliente e encerra a thread.
    close(clt_data->sock);
    free(clt_data); // Libera a memoria alocada para os dados do cliente
    pthread_exit(NULL);
}