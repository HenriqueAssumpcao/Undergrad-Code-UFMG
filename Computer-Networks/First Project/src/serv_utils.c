#include "../include/serv_utils.h"
#include "../include/common.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Funções de leitura de arquivos
void read_inpt_file(const char* file_name, int board[NROWS][NCOLS]){
    /*
    Função que lê o tabuleiro de entrada de um arquivo.
    Recebe um nome de arquivo e um tabuleiro.
    Não retorna nada.
    */
    FILE *file_ptr = fopen(file_name,"r");
    if(file_ptr == NULL){
        exit(EXIT_FAILURE);
    };
    
    char buffer[BUFSZ] = "",*token;
    int row_counter = 0,col_counter = 0;

    while(fgets(buffer,BUFSZ,file_ptr) != NULL){ // Lê as celulas do arquivo e as armazena no tabuleiro passado como argumento,
        token = strtok(buffer,",");
        while(token != NULL){
            board[row_counter][col_counter] = atoi(token);

            token = strtok(NULL,",");
            col_counter++;
        }
        col_counter = 0;
        row_counter++;
    }

    fclose(file_ptr);
}

// Funções de inicialização
void init_clt_board(int clt_board[NROWS][NCOLS]){
    /*
    Função que inicializa o tabuleiro do cliente.
    Recebe um tabuleiro.
    Não retorna nada.
    */
    size_t i,j;
    for(i = 0; i < NROWS; i++){
        for(j = 0; j < NCOLS; j++){ // Inicializa todas as entradas com o ID de célula oculta
            clt_board[i][j] = OCCULTC_ID;
        }
    }
}

// Funções auxiliares
int check_win_condition(const int board[NROWS][NCOLS]){
    /*
    Função que checa se um tabuleiro está em estado de vitória.
    Recebe um tabuleiro.
    Retorna 1 caso esteja, e 0 caso contrário.
    */
	size_t i,j;
	for(i = 0; i < NROWS; i++){
		for(j = 0; j < NCOLS; j++){
			if(board[i][j] != BOMBC_ID && board[i][j] == OCCULTC_ID){
                return 0;
            }
		}
	}
    return 1;
}

// Funções de processamento de ações
void process_action(action *act,int clt_board[NROWS][NCOLS],const int serv_board[NROWS][NCOLS]){
    /*
    Função que processa a ação do cliente recebida pelo servidor.
    Recebe uma ação, e dois tabuleiros, um do cliente e outro do servidor, respectivamente.
    Não retorna nada.
    */
    if(act->type == START_ID){
        // Start
        memcpy(act->board,clt_board,sizeof(act->board));
        act->type = STATE_ID;
    }
    else if(act->type == RESET_ID){
        // Reset
        init_clt_board(clt_board);
        memcpy(act->board,clt_board,sizeof(act->board));
        act->type = STATE_ID;
        printf("starting new game\n");
    }
    else if(act->type == REVEAL_ID){
        // Reveal
        if(serv_board[act->coordinates[0]][act->coordinates[1]] == BOMBC_ID){
            // Game-over
            memcpy(act->board,serv_board,sizeof(act->board));
            act->type = GAME_OVER_ID;
        }
        else{
            // Reveal válido
            clt_board[act->coordinates[0]][act->coordinates[1]] = serv_board[act->coordinates[0]][act->coordinates[1]];

            if(check_win_condition(clt_board) == 1){ // Checa se o jogo foi ganho
                memcpy(act->board,serv_board,sizeof(act->board));
                act->type = WIN_ID;
            }
            else{
                memcpy(act->board,clt_board,sizeof(act->board));
                act->type = STATE_ID;
            }
        }
    }
    else if(act->type == FLAG_ID){
        // Flag
        clt_board[act->coordinates[0]][act->coordinates[1]] = FLAGC_ID;
        memcpy(act->board,clt_board,sizeof(act->board));
        act->type = STATE_ID;
    }
    else if(act->type == REMOVE_FLAG_ID){
        // Remove flag
        if(clt_board[act->coordinates[0]][act->coordinates[1]] == FLAGC_ID){
            clt_board[act->coordinates[0]][act->coordinates[1]] = OCCULTC_ID;
        }
        memcpy(act->board,clt_board,sizeof(act->board));
        act->type = STATE_ID;
    }
    else{
        // Caso de erro
        act->type = ERROR_TYPE_ID;
    }
}