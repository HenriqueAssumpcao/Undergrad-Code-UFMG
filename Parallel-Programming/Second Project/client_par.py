import os
import sys
import socket

import grpc 
import functions_pb2,functions_pb2_grpc


def peer_send_msg(stub,action_id:str,args:list):
    """
    Função que envia a mensagem de um cliente par ao servidor par
    Inputs:
      stub: stub para comunicação com o servidor par
      action_id: id da ação a ser feita pelo cliente
      args: argumentos da ação
    Não retorna nada
    Funcionamento: primeiro identifica qual ação deve ser feita e então procede de acordo com o enunciado
    """
    if(action_id == 'I'):
        key = int(args[0])
        content = args[1].strip()
        
        response = stub.insert_key(functions_pb2.IntStrMessage(int_attr=key,str_attr=content))
        print(f"{response.int_attr}") 
    elif(action_id == 'C'):
        key = int(args[0])

        response = stub.query(functions_pb2.IntMessage(int_attr=key))
        print(f"{response.str_attr}")
    elif(action_id == 'A'):
        content = args[0]

        response = stub.activate(functions_pb2.StrMessage(str_attr=content))
        print(f"{response.int_attr}")
    elif(action_id == 'T'):
        response = stub.terminate(functions_pb2.VoidMessage())
        print(f"{response.int_attr}")

if __name__ == '__main__':
    """
    Seção main
    Funcionamento: processa os argumentos da linha de comando, se conecta ao servidor e executa o loop while do cliente,
    onde a cada iteração a entrada padrão é processada e uma mensagem é enviada ao servidor
    """
    service_id = sys.argv[1]
    host_name,port = service_id.split(':')
    # Estabele a conexão com o servidor e gera o stub
    channel = grpc.insecure_channel(service_id)
    stub = functions_pb2_grpc.ServPeerStub(channel)
    # Loop while principal
    while True:
        # Parsing da entrada
        line = sys.stdin.readline()
        if not line: # Se detecta EOF termina o loop
            break
        parsed_line = line.split(',',maxsplit=2)
        action_id = parsed_line.pop(0).strip()
        # Envia a mensagem
        try:
            peer_send_msg(stub,action_id=action_id,args=parsed_line)
        except:
            # Em caso de erro no envio da mensagem, imprime mensagem de erro e encerra
            print("error: unable to connect to server")
            break
        # Termina caso a entrada seja o comando término
        if action_id == 'T':
            break