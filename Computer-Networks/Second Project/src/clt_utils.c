#include "../include/clt_utils.h"
#include "../include/common.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

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

void init_clt_op(BlogOperation *op,int clt_id){
	/*
	Função que inicializa uma operação com valores padrão.
	Recebe uma operação e um id de cliente.
	Não retorna nada.
	*/
	op->client_id = clt_id;
	op->operation_type = EMPTY_ID;
	op->server_response = 0;
	strcpy(op->topic,"");
	strcpy(op->content,"");
}

void parse_input(BlogOperation *op,int *clt_id){
    /*
    Função que faz o parsing do input do cliente.
	Recebe uma operação e o id do cliente.
	Não retorna nada.
    */
    // Inicialização de variáveis
	char *token;
	char buffer[BUFSIZ];
	memset(buffer, 0, BUFSZ);

	// Recebe a entrada do stdin identificando o comando dado pelo cliente
	fgets(buffer, BUFSZ-1, stdin);

	buffer[strcspn(buffer,"\n")] = 0; // Remove os \n
	token = strtok(buffer," ");
	
	init_clt_op(op,*clt_id); // Inicializa a operação com valores padrão e o id do cliente
	
	// Checagem do tipo de operação a ser realizada e tratamento de possíveis erros no input
	if(token == NULL){ // Checa por entrada vazia
		op->operation_type = ERROR_TYPE_ID;
	}
	else if(strcmp(token,"publish") == 0){
		token = strtok(NULL," ");
		if(token != NULL && strcmp(token,"in") == 0){ // Checa se o próximo token é o identificador "in"
			token = strtok(NULL," ");
			if(token != NULL){ // Checa por entrada vazia 
				op->operation_type = NEW_POST_ID;
				strcpy(op->topic,token);

				memset(buffer, 0, BUFSZ);
				// Recebe o conteúdo do stdin e copia para a variável de operação
				fgets(buffer, BUFSZ-1, stdin);

				buffer[strcspn(buffer,"\n")] = 0;
				strcpy(op->content,buffer);
			}
			else{
				op->operation_type = ERROR_TYPE_ID;
			}
		}
		else{
			op->operation_type = ERROR_TYPE_ID;
		}
	}
	else if(strcmp(token,"subscribe") == 0){ 
		token = strtok(NULL," ");
		if(token != NULL){
			op->operation_type = SUBSCRIBE_ID;
			strcpy(op->topic,token);
		}
		else{
			op->operation_type = ERROR_TYPE_ID;
		}
	}
	else if(strcmp(token,"unsubscribe") == 0){
		token = strtok(NULL," ");
		if(token != NULL){
			op->operation_type = UNSUBSCRIBE_ID;
			strcpy(op->topic,token);
		}
		else{
			op->operation_type = ERROR_TYPE_ID;
		}
	}
	else if(strcmp(token,"list") == 0){
		token = strtok(NULL," ");
		if(token != NULL && strcmp(token,"topics") == 0){
			op->operation_type = LIST_TOPICS_ID;
		}
		else{
			op->operation_type = ERROR_TYPE_ID;
		}
	}
	else if(strcmp(token,"exit") == 0){
		op->operation_type = DISCONNECT_ID;
	}
	else{
		op->operation_type = ERROR_TYPE_ID;
	}
}

void *run_recv_thread(void* sock){
	/*
	Procedimento que será executado pela thread que faz o recebimento de dados pelo servidor.
	Recebe um ponteiro void para o socket do cliente.
	Não retorna nada.
	*/
	// Inicialização de variáveis
	int *clt_sock = (int*)sock;
	BlogOperation op;
	// Loop principal do receive
	while(1){
		recv_op(&op,clt_sock); // Recebe a operação do servidor.
		
		pthread_mutex_lock(&exit_mutex);
		if(exit_recvd == 1){ // Se o cliente deseja se desconectar, encerra o loop.
			break;
		}
		pthread_mutex_unlock(&exit_mutex);

		// Se o tipo recebido for list topics, publish in ou subscribe, imprime o conteúdo. Do contrário checa por erros.
		if(op.operation_type == LIST_TOPICS_ID || op.operation_type == NEW_POST_ID || op.operation_type == SUBSCRIBE_ID){
			printf("%s\n",op.content);
		}
		else if(op.operation_type == ERROR_TYPE_ID){ // Se um erro foi encontrado, altera o valor da variável global e encerra o loop.
		    pthread_mutex_lock(&error_mutex);
			error_recvd = 1;
			pthread_mutex_unlock(&error_mutex);
			break;
		}
	}
	pthread_exit(NULL);
}