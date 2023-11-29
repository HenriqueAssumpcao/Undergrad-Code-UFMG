#ifndef CLT_UTILS_HEADER
#define CLT_UTILS_HEADER

#include "common.h"
#include <arpa/inet.h>
// Funções de parsing de IP e Porto

/*
Função que faz o parsing do endereço fornecido pelo cliente.
Recebe duas strings representando o endereço e o porto, respectivamente, e um struct sockaddr_storage.
Retorna 0 se o parsing foi bem sucedido, e -1 caso contrário.
*/
int parse_clt_addr(const char *addrstr, const char *portstr,
              struct sockaddr_storage *storage);

// Funções auxiliares

/*
Função que converte uma string indicando o tipo de operação de um jogo e a converte para um inteiro.
Recebe uma string.
Retorna um inteiro.
*/
int typestr2id(const char* typestr);

// Funções de inicialização

/*
Função que inicializa um objeto do tipo ação.
Recebe uma ação e um tabuleiro.
Não retorna nada.
*/
void init_action(action *act, int board[NROWS][NCOLS]);

// Funções de processamento de ações

/*
Função que faz o parsing fo input do cliente.
Recebe uma ação e um buffer de char.
Não retorna nada.
*/
void parse_input(action *act,char buffer[BUFSZ]);


#endif