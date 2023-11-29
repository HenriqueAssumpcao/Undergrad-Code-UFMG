import sys
import socket
import threading
from concurrent import futures

import grpc
import functions_pb2,functions_pb2_grpc

MAX_WORKERS = 10 # Número máximo de threads no POOL de threads do servidor par


class ServPeer(functions_pb2_grpc.ServPeerServicer):
    """
    Definição da classe que implementa os métodos executados pelo servidor par
    """
    def __init__(self,service_id:str,stop_event:threading.Event,activation_flag:bool):
        """
        Método __init__.
        Inputs:
          service_id: id identificador do servidor par
          stop_event: objeto Event da biblioteca threading que será utilizado para terminar o servidor caso o cliente deseje
          activation_flag: flag indicando se o servidor deve implementar o método de ativação para algum servidor central
        """
        self._service_id = service_id
        self._id2str = {} # Dicionário que mapeia IDs para Strings
        self._stop_event = stop_event
        self._activation_flag = activation_flag

    def insert_key(self,request,context):
        """
        Método que insere uma chave no dicionário
        Inputs:
          request(IntStrMessage): objeto request do gRPC que armazena a chave e o conteúdo a serem inseridos no dicionário
          context: atributo de contexto que não é utilizado diretamente pelo método
        Output:
          IntMessage: retorna uma mensagem ao cliente com o valor 0 se a chave foi inserida no dicionário, e -1 caso ela já exista
        """
        key = request.int_attr
        content = request.str_attr
        success = 0

        if(key in self._id2str.keys()):
            success = -1
        else:
            self._id2str[key] = content

        return functions_pb2.IntMessage(int_attr=success)
    
    def query(self,request,context):
        """
        Método que consulta por uma chave no dicionário
        Inputs:
          request(IntMessage): objeto request do gRPC que armazena a chave a ser consultada no dicionário
          context: atributo de contexto que não é utilizado diretamente pelo método
        Output:
          StrMessage: retorna uma mensagem ao cliente com o valor armazenado na chave, ou com a string vazia caso a chave não exista
        """
        key = request.int_attr
        reply = None

        if(key in self._id2str.keys()):
            reply = self._id2str[key]

        return functions_pb2.StrMessage(str_attr=reply)
    
    def activate(self,request,context):
        """
        Método que faz o procedimento de ativação com algum servidor central
        Inputs:
          request(IntMessage): objeto request do gRPC que armazena o id identificador do servidor central.
          context: atributo de contexto que não é utilizado diretamente pelo método
        Output:
          IntMessage: caso a flag de ativação seja 0, retorna o valor 0 ao cliente. Caso seja 1, retorna o número de chaves que foram
          inseridas com sucesso no servidor central, ou -1 caso ocorra algum erro de conexão, e.g., o cliente tenta ativar w.r.t. a algum
          servidor central que não existe
        """
        if self._activation_flag:
            try:
                # Estabelece conexão com o servidor central
                central_serv_id = request.str_attr
                central_channel = grpc.insecure_channel(central_serv_id)
                central_stub = functions_pb2_grpc.ServCentralStub(central_channel)
                # Envia o id do servidor par atual e a lista de chaves armazenadas
                stored_keys = [*self._id2str.keys()]
                response = central_stub.register(functions_pb2.StrRepeatedIntMessage(str_attr=self._service_id,int_attrs=stored_keys))
                return functions_pb2.IntMessage(int_attr=response.int_attr)
            except: # Erro na conexão, nesse caso retorna o valor -1
                return functions_pb2.IntMessage(int_attr=-1)
        else:
            return functions_pb2.IntMessage(int_attr=0)
    
    def terminate(self,request,context):
        """
        Método que faz o procedimento de término do servidor.
        Inputs:
          request(VoidMessage): objeto request do gRPC que apenas age como uma flag para realizar o término
          context: atributo de contexto que não é utilizado diretamente pelo método
        Returns:
          IntMessage: retorna o valor 0 para o cliente.
        """
        self._stop_event.set() # Ativa o evento de parada
        return functions_pb2.IntMessage(int_attr=0)

if __name__ == '__main__':
    """
    Seção main
    Funcionamento: processa os argumentos da linha de comando, inicializa o servidor e então espera por conexões de clientes. Caso algum cliente
    envie uma ação de término, encerra
    """
    port_number = sys.argv[1]
    activation_flag = True if len(sys.argv) > 2 else False
    # Monta o identificador do servidor
    service_id = socket.getfqdn() + ":" + port_number 
    # Inicializa o servidor GRPC
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=MAX_WORKERS))
    stop_event = threading.Event()
    functions_pb2_grpc.add_ServPeerServicer_to_server(ServPeer(service_id=service_id,stop_event=stop_event,activation_flag=activation_flag), server)
    server.add_insecure_port("0.0.0.0:" + port_number)
    # Loop principal, note que o servidor espera até que ocorra um stop_event, que só ocorre quando algum cliente envia um terminate
    server.start()
    stop_event.wait()
    server.stop(None)
