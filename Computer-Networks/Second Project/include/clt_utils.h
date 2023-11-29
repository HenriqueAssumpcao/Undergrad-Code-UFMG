#ifndef CLT_UTILS_HEADER
#define CLT_UTILS_HEADER

#include "common.h"
#include <pthread.h>

// Variáveis globais

extern int error_recvd; // Variável global que indica se ocorreu erro na thread de receive.
extern int exit_recvd; // Variável global que indica se o cliente recebeu um comando de exit.

extern pthread_mutex_t error_mutex; // Variável mutex para controlar acesso à variável error_recvd
extern pthread_mutex_t exit_mutex; // Variável mutex para controlar acesso à variável exit_recvd

// Funções de parsing de IP e Porto

/*
Função que faz o parsing do endereço fornecido pelo cliente.
Recebe duas strings representando o endereço e o porto, respectivamente, e um struct sockaddr_storage.
Retorna 0 se o parsing foi bem sucedido, e -1 caso contrário.
*/
int parse_clt_addr(const char *addrstr, const char *portstr,
              struct sockaddr_storage *storage);

// Funções de inicialização

/*
Função que inicializa uma operação com valores padrão.
Recebe uma operação e um id de cliente.
Não retorna nada.
*/
void init_clt_op(BlogOperation *op,int clt_id);

// Funções de parsing

/*
Função que faz o parsing do input do cliente.
Recebe uma operação e o id do cliente.
Não retorna nada.
*/
void parse_input(BlogOperation *op,int *clt_id);

// Funções de threading

/*
Procedimento que será executado pela thread que faz o recebimento de dados pelo servidor.
Recebe um ponteiro void para o socket do cliente.
Não retorna nada.
*/
void *run_recv_thread(void* sock);


#endif