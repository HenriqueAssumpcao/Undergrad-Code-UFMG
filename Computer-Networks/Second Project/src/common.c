#include "../include/common.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

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

void send_op(const BlogOperation *op, int *sock){
    /*
    Função que envia uma operação. 
    Recebe uma operação e um socket.
    Não retorna nada.
    */
    // Inicializa um buffer, copia os valores para o buffer e então envia pelo socket.
    BlogOperation op_buff;

    op_buff.client_id = op->client_id;
    op_buff.operation_type = op->operation_type;
    op_buff.server_response = op->server_response;
    strcpy(op_buff.topic,op->topic);
    strcpy(op_buff.content,op->content);

    send_loop(sock,(void*)&op_buff,sizeof(op_buff));

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

void recv_op(BlogOperation *op,int *sock){
    /*
    Função que recebe uma operação. 
    Recebe uma operação e um socket.
    Não retorna nada.
    */
    // Inicializa as variáveis e recebe os dados pelo socket.
    BlogOperation op_buff;

    int recv_success = recv_loop(sock,(void*)&op_buff,sizeof(op_buff));

    if(recv_success == 0){ // Se ocorreu erro no recebimento, indica isso por meio do valor de operation_type
        op->operation_type = ERROR_TYPE_ID;
    }
    else{ // Se não, copia os valores do buffer para a operação passada como parâmetro.
        op->client_id = op_buff.client_id;
        op->server_response = op_buff.server_response;
        op->operation_type = op_buff.operation_type;
        strcpy(op->content,op_buff.content);
        strcpy(op->topic,op_buff.topic);
    }
}
