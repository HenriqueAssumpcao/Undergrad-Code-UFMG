#ifndef SERV_UTILS_HEADER
#define SERV_UTILS_HEADER

#include "common.h"
#include <arpa/inet.h>
// Funções de parsing de IP e Porto

/*
Função que inicializa o endereço do servidor.
Recebe duas strings representando o tipo de protocolo e o porto, respectivamente, e um struct sockaddr_storage.
Retorna 0 se o parsing foi bem sucedido, e -1 caso contrário.
*/
int parse_serv_addr(const char *proto, const char *portstr,
                         struct sockaddr_storage *storage);

// Funções de leitura de arquivos

/*
Função que lê o tabuleiro de entrada de um arquivo.
Recebe um nome de arquivo e um tabuleiro.
Não retorna nada.
*/
void read_inpt_file(const char* file_name, int board[NROWS][NCOLS]);

// Funções de inicialização

/*
Função que inicializa o tabuleiro do cliente.
Recebe um tabuleiro.
Não retorna nada.
*/
void init_clt_board(int clt_board[NROWS][NCOLS]);

// Funções auxiliares

/*
Função que checa se um tabuleiro está em estado de vitória.
Recebe um tabuleiro.
Retorna 1 caso esteja, e 0 caso contrário.
*/
int check_win_condition(const int board[NROWS][NCOLS]);

// Funções de processamento de ações

/*
Função que processa a ação do cliente recebida pelo servidor.
Recebe uma ação, e dois tabuleiros, um do cliente e outro do servidor, respectivamente.
Não retorna nada.
*/
void process_action(action *act,int clt_board[NROWS][NCOLS],const int serv_board[NROWS][NCOLS]);



#endif