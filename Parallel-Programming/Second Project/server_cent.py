import sys
import socket
import threading
from concurrent import futures

import grpc
import functions_pb2,functions_pb2_grpc

MAX_WORKERS = 10

class ServCentral(functions_pb2_grpc.ServCentralServicer):
    """
    Definição da classe que implementa os métodos executados pelo servidor central
    """
    def __init__(self,stop_event):
        """
        Método __init__
        Inputs:
          stop_event: objeto Event da biblioteca threading que será utilizado para terminar o servidor caso o cliente deseje
        """
        self._id2serv = {} # Dicionário que mapeia IDs para strings identificadoras de servidores
        self._stop_event = stop_event

    def register(self,request,context):
        """
        Método que registra as chaves de um servidor par no dicionário de chaves para ids de servidores
        Inputs:
          request(StrRepeatedIntMessage): objeto request do gRPC que armazena a string identificadora do servidor par,
           e a lista de chaves armazenadas pelo servidor par
           context: atributo de contexto que não é utilizado diretamente pelo método
        Output:
          IntMessage: retorna o número de novas chaves adicionadas no dicionário do servidor central ao servidor par, i.e.,
          a quantidade de chaves do servidor par que já não eram chaves no dicionário do servidor central
        """
        num_processed_keys = 0
        # Percorre a lista de chaves do servidor par
        for key in request.int_attrs:
            # Se a chave já não existe no servidor central, adiciona ela no dicionário com o valor igual a string identificadora do servidor par
            if key not in self._id2serv.keys():
                self._id2serv[key] = request.str_attr
            num_processed_keys += 1

        return functions_pb2.IntMessage(int_attr=num_processed_keys)
    
    def map(self,request,context):
        """
        Método que realiza o processo de mapeamento do servidor central
        Inputs:
          request(IntMessage): objeto request do gRPC que armazena a chave a ser consultada
          context: atributo de contexto que não é utilizado diretamente pelo método
        Output:
          StrMessage: retorna a string identificadora do servidor que possui a chave fornecida como input, ou a string vazia caso tal chave não esteja
          no dicionário do servidor central
        """
        reply = None
        if request.int_attr in self._id2serv.keys():
            reply = self._id2serv[request.int_attr]
        return functions_pb2.StrMessage(str_attr=reply)
    
    def terminate(self,request,context):
        """
        Método que faz o procedimento de término do servidor
        Inputs:
          request(VoidMessage): objeto request do gRPC que apenas age como uma flag para realizar o término
          context: atributo de contexto que não é utilizado diretamente pelo método
        Returns:
          IntMessage: retorna a quantidade de chaves armazenadas para o cliente
        """
        self._stop_event.set() # Ativa o evento de parada
        num_stored_keys = len(self._id2serv)
        return functions_pb2.IntMessage(int_attr=num_stored_keys)

if __name__ == '__main__':
    """
    Seção main
    Funcionamento: processa os argumentos da linha de comando, inicializa o servidor e então espera por conexões de clientes. Caso algum cliente
    envie uma ação de término, encerra
    """
    port_number = sys.argv[1]
    # Monta o identificador do servidor
    service_id = socket.getfqdn() + ":" + port_number  
    # Inicializa o servidor GRPC
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=MAX_WORKERS))
    stop_event = threading.Event()
    functions_pb2_grpc.add_ServCentralServicer_to_server(ServCentral(stop_event=stop_event), server)
    server.add_insecure_port("0.0.0.0:" + port_number)
    # Loop principal, note que o servidor espera até que ocorra um stop_event, que só ocorre quando algum cliente envia um terminate
    server.start()
    stop_event.wait()
    server.stop(None)

