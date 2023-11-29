#include "../include/common.h"
#include "../include/serv_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#define BACKLOG_SIZE 1

int main(int argc, char **argv) {
    /*
    Função main.
    Recebe os argumentos de entrada do servidor: protocolo (v4 ou v6), porto e nome do arquivo para ler o tabuleiro do servidor.
    Retorna 0 em caso de sucesso, e 1 em caso de erro.
    */
    if(argc != 5){
        exit(EXIT_FAILURE);
    }
    char *protocol = argv[1];
    char *portstr = argv[2];
    char *filename = argv[4];
    printf("%s\n",filename);

    // Inicializa o endereço do servidor
    struct sockaddr_storage storage;
    if(parse_serv_addr(protocol, portstr, &storage) == -1){
        exit(EXIT_FAILURE); // Erro na entrada na linha de comando
    }

    // Inicializa o socket do servidor e evita que o endereço seja travado após o fechamento do servidor (enable = 1)
    int serv_sock = socket(storage.ss_family, SOCK_STREAM, 0); 
    int enable = 1;
    if(setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1){
        exit(EXIT_FAILURE); // Erro no enable do socket
    }

    // Bind e Listen do servidor
    struct sockaddr *serv_addr = (struct sockaddr *)(&storage);
    bind(serv_sock, serv_addr, sizeof(storage));

    if(listen(serv_sock, BACKLOG_SIZE) == -1){
        exit(EXIT_FAILURE); // Erro no Listen
    }

    // Inicializa o tabuleiro do cliente com todas as células ocultas
    int serv_board[NROWS][NCOLS], clt_board[NROWS][NCOLS];
    
    // Lê o tabuleiro do servidor de um arquivo
    read_inpt_file(filename,serv_board);
    print_board(serv_board);

    // Loop principal do servidor
    while (1) {
        // Aceita a conexão de um cliente
        struct sockaddr_storage clt_storage;
        struct sockaddr *clt_addr = (struct sockaddr *)(&clt_storage);
        socklen_t clt_addr_len = sizeof(clt_storage);

        int clt_sock = accept(serv_sock, clt_addr, &clt_addr_len);
        printf("client connected\n");
        
        action act; // Ação a ser trocada com o cliente
        init_clt_board(clt_board); // Inicializa o tabuleiro do cliente

        // Loop de processamento de um cliente
        while(1){
            recv_action(&act,&clt_sock);

            if(act.type == ERROR_TYPE_ID){ // Checa se houve erro no recebimento da mensagem, se sim sai do loop e fecha a conexão
                break;
            }
            else if(act.type == EXIT_ID){ // Checa se o cliente desconectou, se sim sai do loop e fecha a conexão
                printf("client disconnected\n");
                break;
            }

            process_action(&act,clt_board,serv_board);

            if(act.type == ERROR_TYPE_ID){ // Checa se a mensagem recebida é inválida, se sim sai do loop e fecha a conexão
                break;
            }

            send_action(&act,&clt_sock);

            if(act.type == WIN_ID || act.type == GAME_OVER_ID){ // Checa se o cliente teve win ou game_over
                init_clt_board(clt_board); // Reseta o tabuleiro do cliente
            }
        }
        close(clt_sock);
    }

    exit(EXIT_SUCCESS);
}

