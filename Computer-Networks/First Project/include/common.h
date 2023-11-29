#ifndef COMMON_HEADER
#define COMMON_HEADER

#define BUFSZ 1024

#define ERROR_TYPE_ID -4
#define EMPTY_ID -5

#define BOMBC_ID -1
#define OCCULTC_ID -2
#define FLAGC_ID -3

#define START_ID 0
#define REVEAL_ID 1
#define FLAG_ID 2
#define STATE_ID 3
#define REMOVE_FLAG_ID 4
#define RESET_ID 5
#define WIN_ID 6
#define EXIT_ID 7
#define GAME_OVER_ID 8

#define NROWS 4
#define NCOLS 4

typedef struct action{
    int type;
    int coordinates[2];
    int board[NROWS][NCOLS];
}action;

// Função de impressão
/*
Função que imprime um tabuleiro.
Recebe uma matriz de inteiros.
Não retorna nada.
*/
void print_board(int board[NROWS][NCOLS]);

// Funções de troca de mensagens

/*
Função que envia uma ação para um dado socket.
Recebe uma ação e um socket de forma dinâmica.
Não retorna nada.
*/
void send_action(action *act,int *sock);

/*
Função que recebe uma ação de um dado socket.
Recebe uma ação e um socket de forma dinâmica.
Não retorna nada.
*/
void recv_action(action *act,int *sock);

#endif