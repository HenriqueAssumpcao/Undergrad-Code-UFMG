#include "../include/common.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

// Função de impressão
void print_board(int board[NROWS][NCOLS]){
    /*
    Função que imprime um tabuleiro.
    Recebe uma matriz de inteiros.
    Não retorna nada.
    */
    size_t i,j;
    for(i = 0; i < NROWS; i++){
        for(j = 0; j < NCOLS; j++){
            if(board[i][j] == FLAGC_ID){
                printf(">\t\t");
            }
            else if(board[i][j] == OCCULTC_ID){
                printf("-\t\t");
            }
            else if(board[i][j] == BOMBC_ID){
                printf("*\t\t");
            }
            else if(board[i][j] == 0){
                printf("0\t\t");
            }
            else {
                printf("%d\t\t",board[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Funções de troca de mensagens
void send_loop(int *sock,const void *buff,size_t buff_size){
    /*
    Função que faz o loop de envio dos dados de um buffer para um dado socket.
    Recebe um socket, um buffer e o tamanho do buffer. Faz sucessivas operações de send até que todos os dados sejam enviados.
    Não retorna nada.
    */
	ssize_t total = 0,sent = 0;
    while(total < buff_size){
        sent = send(*sock,(char*)buff + total,buff_size - total,0);
        if(sent < 0){
            break;
        }
        total += sent;
    }
}
void send_action(action *act,int *sock){
    /*
    Função que envia uma ação para um dado socket.
    Recebe uma ação e um socket de forma dinâmica.
    Não retorna nada.
    */
    
    // Copia a ação para um buffer
    action act_buff;
    act_buff.type = (int)htonl((u_int32_t)act->type);
	act_buff.coordinates[0] = (int)htonl((u_int32_t)act->coordinates[0]);
	act_buff.coordinates[1] = (int)htonl((u_int32_t)act->coordinates[1]);

	size_t i,j;
	for(i = 0; i < NROWS; i++){
		for(j = 0; j < NCOLS; j++){
			act_buff.board[i][j] = (int)htonl((u_int32_t) act->board[i][j]);
		}
	}

    // Envia a ação
    send_loop(sock,(void*)&act_buff,sizeof(act_buff));
}

int recv_loop(int *sock,void *buff,size_t buff_size){
    /*
    Função que faz o loop de recebimento dos dados de um buffer para um dado socket.
    Recebe um socket, um buffer e o tamanho do buffer. Faz sucessivas operações de receive até que todos os dados sejam recebidos.
    Retorna 1 se todos os dados foram recebidos, e 0 caso contrário.
    */
	ssize_t total = 0,received = 0;
    while(total < buff_size){
        received = recv(*sock,(char*)buff + total,buff_size - total,0);
        if(received == -1 || (received == 0 && total < buff_size)){
            return 0;
        }
        total += received;
    }
    return 1;
}

void recv_action(action *act,int *sock){
    /*
    Função que recebe uma ação de um dado socket.
    Recebe uma ação e um socket de forma dinâmica.
    Não retorna nada.
    */
    // Recebe a ação em um buffer
    action act_buff;
    int recv_success = recv_loop(sock,(void*)&act_buff,sizeof(act_buff));

    if(recv_success == 0){ // Checa se houve erro no receive
        act->type = ERROR_TYPE_ID;
    }
    else{
        // Converte para host long.
        act->type = (int)ntohl((u_int32_t)act_buff.type);
        act->coordinates[0] = (int)ntohl((u_int32_t)act_buff.coordinates[0]);
        act->coordinates[1] = (int)ntohl((u_int32_t)act_buff.coordinates[1]);

        size_t i,j;
        for(i = 0; i < NROWS; i++){
            for(j = 0; j < NCOLS; j++){
                act->board[i][j] = (int)ntohl((u_int32_t)act_buff.board[i][j]);
            }
        }
    }
}