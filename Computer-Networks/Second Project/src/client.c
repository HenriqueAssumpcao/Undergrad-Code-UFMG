#include "../include/clt_utils.h"
#include "../include/common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int error_recvd = 0; // Variável global que indica se ocorreu erro na thread de receive.
int exit_recvd = 0; // Variável global que indica se o cliente recebeu um comando de exit.

pthread_mutex_t error_mutex = PTHREAD_MUTEX_INITIALIZER; // Variável mutex para controlar acesso à variável error_recvd
pthread_mutex_t exit_mutex = PTHREAD_MUTEX_INITIALIZER; // Variável mutex para controlar acesso à variável exit_recvd

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

	// Conecta ao servidor
	sock = socket(storage.ss_family, SOCK_STREAM, 0);
	if(connect(sock, serv_addr, sizeof(storage)) == -1){
		exit(EXIT_FAILURE); // Erro na conexão
	}

	BlogOperation op;
	init_clt_op(&op,EMPTY_ID);

	// Primeiro send e receive para estabelecer a conexão e descobrir o client_id
	op.operation_type = NEW_CONNECTION_ID;
	send_op(&op,&sock);
	recv_op(&op,&sock);
	int clt_id = op.client_id;

	// Criação da thread de receive
	pthread_t recv_thread;
	pthread_create(&recv_thread,NULL,run_recv_thread,(void*)&sock);

	// Loop principal de send do cliente
	while(1){
		// Checa por erro na recepção usando exclusão mútua
		pthread_mutex_lock(&error_mutex);
		if(error_recvd == 1){
			break;
		}
		pthread_mutex_unlock(&error_mutex);

		parse_input(&op,&clt_id);
		if(op.operation_type == ERROR_TYPE_ID){
			continue;
		}
		send_op(&op,&sock);
		// Checa se o cliente que se desconectar
		if(op.operation_type == DISCONNECT_ID){
			pthread_mutex_lock(&exit_mutex);
			exit_recvd = 1;
			pthread_mutex_unlock(&exit_mutex);

			break;
		}
	}
	pthread_join(recv_thread,NULL); // Espera até que a thread de receive termine.

	// Fecha a conexão, destroi as mutex e retorna sucesso
	close(sock);

    pthread_mutex_destroy(&error_mutex);
	pthread_mutex_destroy(&exit_mutex);

	exit(EXIT_SUCCESS);
}