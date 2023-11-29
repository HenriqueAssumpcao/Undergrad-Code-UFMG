#ifndef COMMON_HEADER
#define COMMON_HEADER

#include <stdlib.h>
#include <arpa/inet.h>

// Constantes
#define TOPIC_SZ 50 // Tamanho do buffer do tópico
#define BUFSZ 2048 // Tamanho do buffer do conteúdo

#define EMPTY_ID 0 // ID para indicar posição desocupada em um array 
#define ERROR_TYPE_ID -1 // ID para indicar que houve algum erro na troca de mensagens

#define NEW_CONNECTION_ID 1
#define NEW_POST_ID 2
#define LIST_TOPICS_ID 3
#define SUBSCRIBE_ID 4
#define DISCONNECT_ID 5
#define UNSUBSCRIBE_ID 6

// Estrutura BlogOperation definida no enunciado
typedef struct BlogOperation{
    int client_id;
    int operation_type;
    int server_response;
    char topic[TOPIC_SZ];
    char content[BUFSZ];
}BlogOperation;

// Funções de send e receive

/*
Função que envia uma operação. 
Recebe uma operação e um socket.
Não retorna nada.
*/
void send_op(const BlogOperation *op, int *sock);

/*
Função que recebe uma operação. 
Recebe uma operação e um socket.
Não retorna nada.
*/
void recv_op(BlogOperation *op,int *sock);

#endif