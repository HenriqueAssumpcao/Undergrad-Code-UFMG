#include "../include/clt_utils.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

// Funções de parsing de IP e Porto
int parse_clt_addr(const char *addrstr, const char *portstr,
              struct sockaddr_storage *storage) {
    /*
    Função que faz o parsing do endereço fornecido pelo cliente.
    Recebe duas strings representando o endereço e o porto, respectivamente, e um struct sockaddr_storage.
    Retorna 0 se o parsing foi bem sucedido, e -1 caso contrário.
    */
    if (addrstr == NULL || portstr == NULL) {
        return -1;
    }

    uint16_t port = (uint16_t)atoi(portstr); 
    if (port == 0) {
        return -1;
    }
    port = htons(port);

    struct in_addr inaddr4;
    if (inet_pton(AF_INET, addrstr, &inaddr4)) {
        // Checa se o endereço fornecido é um endereço IPv4
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_port = port;
        addr4->sin_addr = inaddr4;
        return 0;
    }

    struct in6_addr inaddr6;
    if (inet_pton(AF_INET6, addrstr, &inaddr6)) {
        // Checa se o endereço fornecido é um endereço IPv6
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = port;
        memcpy(&(addr6->sin6_addr), &inaddr6, sizeof(inaddr6));
        return 0;
    }

    return -1;
}

// Funções auxiliares
int typestr2id(const char* typestr){
    /*
    Função que converte uma string indicando o tipo de operação de um jogo e a converte para um inteiro.
	Recebe uma string.
	Retorna um inteiro.
    */
	if(strcmp(typestr,"start") == 0){
		return START_ID;
	}
	else if(strcmp(typestr,"reveal") == 0){
		return REVEAL_ID;
	}
	else if(strcmp(typestr,"flag") == 0){
		return FLAG_ID;
	}
	else if(strcmp(typestr,"remove_flag") == 0){
		return REMOVE_FLAG_ID;
	}
	else if(strcmp(typestr,"reset") == 0){
		return RESET_ID;
	}
	else if(strcmp(typestr,"exit") == 0){
		return EXIT_ID;
	}
	else{
		// Se o comando não é reconhecido, retorna um identificador de erro.
		// Note que isso inclui comandos que só o servidor pode usar, e.g., state,win,game_over
		printf("error: command not found\n");
		return ERROR_TYPE_ID;
	}
};

// Funções de inicialização
void init_action(action *act, int board[NROWS][NCOLS]){
    /*
    Função que inicializa um objeto do tipo ação.
    Recebe uma ação e um tabuleiro.
    Não retorna nada.
    */
    // Inicializa os valores com ID de vazio.
	act->coordinates[0] = EMPTY_ID;
	act->coordinates[1] = EMPTY_ID;
	act->type = EMPTY_ID;

    if(board != NULL){
        memcpy(act->board,board,sizeof(act->board));
    }
    else{ // Se o tabuleiro é NULL, também inicializa o tabuleiro da ação com ID de vazio.
        size_t i,j;
        for(i = 0; i < NROWS; i++){
            for(j = 0; j < NCOLS; j++){
                act->board[i][j] = EMPTY_ID;
            }
        }
    }
}

// Funções de processamento de ações
void parse_input(action *act,char buffer[BUFSZ]){
    /*
    Função que faz o parsing fo input do cliente.
	Recebe uma ação e um buffer de char.
	Não retorna nada.
    */
	char *token;
	memset(buffer, 0, BUFSZ);

	fgets(buffer, BUFSZ-1, stdin);
	
	buffer[strcspn(buffer,"\n")] = 0; // remove o \n do final do buffer
	token = strtok(buffer," ");

	act->type = typestr2id(token);

	if(act->type == REVEAL_ID || act->type == FLAG_ID || act->type == REMOVE_FLAG_ID){
		// Se for um comando válido que também passa coordenadas, recebe as coordenadas e então checa por erros nas mesmas
		token = strtok(NULL,",");
		act->coordinates[0] = atoi(token);

		token = strtok(NULL,"");
		act->coordinates[1] = atoi(token);

		if(act->coordinates[0] < 0 || act->coordinates[0] >= NROWS || act->coordinates[1] < 0 || act->coordinates[1] >= NCOLS){
			printf("error: invalid cell\n");
			act->type = ERROR_TYPE_ID;
		}
		else if(act->type == REVEAL_ID && act->board[act->coordinates[0]][act->coordinates[1]] >= 0){
			printf("error: cell already revealed\n");
			act->type = ERROR_TYPE_ID;
		}
		else if(act->type == FLAG_ID && act->board[act->coordinates[0]][act->coordinates[1]] == FLAGC_ID){
			printf("error: cell already has a flag\n");
			act->type = ERROR_TYPE_ID;
		}
		else if(act->type == FLAG_ID && act->board[act->coordinates[0]][act->coordinates[1]] >= 0){
			printf("error: cannot insert flag in revealed cell\n");
			act->type = ERROR_TYPE_ID;
		}
	}
}