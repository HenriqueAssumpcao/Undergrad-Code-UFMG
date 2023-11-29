#include "../include/common.h"
#include "../include/clt_utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    /*
    Função main.
    Recebe os argumentos de entrada do cliente: endereço (IPv4 ou IPv6) e porto.
	Retorna 0 se for bem sucedida, e 1 caso contrário.
    */
	if(argc != 3){
		exit(EXIT_FAILURE);
	}
	char *addrstr = argv[1];
	char *portstr = argv[2];

	// Faz o parsing do endereço fornecido pelo cliente
	int sock;
	struct sockaddr_storage storage;
	if(parse_clt_addr(addrstr, portstr, &storage) == -1){
		exit(EXIT_FAILURE); // Erro no parsing do endereço do cliente
	}

	struct sockaddr *serv_addr = (struct sockaddr *)(&storage);

	// Inicializa a ação do cliente
	action act;
	char buffer[BUFSIZ];
	init_action(&act,NULL);

	// Conecta ao servidor
	sock = socket(storage.ss_family, SOCK_STREAM, 0);
	if(connect(sock, serv_addr, sizeof(storage)) == -1){
		exit(EXIT_FAILURE); // Erro na conexão
	}

	while(1){
		parse_input(&act,buffer);

		if(act.type == ERROR_TYPE_ID){ // Checa se o input tem formato errado, se sim ignora o input
			continue;
		}

		send_action(&act,&sock);

		if(act.type == EXIT_ID){ // Checa se o cliente enviou um exit, se sim sai do loop e encerra
			break;
		}
		
		recv_action(&act,&sock);

		if(act.type == ERROR_TYPE_ID){ // Checa se houve erro no recebimento da mensagem, se sim encerra
			break;
		}

		if(act.type == WIN_ID){ // Se recebeu win, imprime a mensagem e o tabuleiro, e também reseta o estado armazenado
			printf("YOU WIN!\n");
			print_board(act.board);
			init_action(&act,NULL);
		}
		else if(act.type == GAME_OVER_ID){ // Se recebeu game_over, imprime a mensagem e o tabuleiro, e também reseta o estado armazenado
			printf("GAME OVER!\n");
			print_board(act.board);
			init_action(&act,NULL);
		}
		else if(act.type == STATE_ID){ // Se recebeu algum estado, imprime o tabuleiro
			print_board(act.board);
		}
	}

	// Fecha a conexão e retorna sucesso
	close(sock);
	exit(EXIT_SUCCESS);
}