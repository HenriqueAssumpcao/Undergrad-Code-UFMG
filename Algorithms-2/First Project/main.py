import numpy as np
import argparse

class TrieNode():
    """
    Classe que implementa um nó da árvore Trie.
    """
    def __init__(self,node_content=None,child_nodes:list=[],index:int=None):
        """
        Método construtor. Recebe uma variável que indica o conteúdo do nó, uma variável que representa a lista de nós filhos e 
        uma variável inteira indicando o índice do nó em questão. Na implementação escolhida, somente nós que representam
        a última letra de uma palavra possuem índice diferente de None.
        """
        self.content = node_content
        self.child_nodes = child_nodes
        self.index = index
        
    
    def search_node(self,content):
        """
        Método que busca dentre os nós filhos por um determinado valor de conteúdo.
        """
        tgt_nodes = [node for node in self.child_nodes if node.content == content]
        if len(tgt_nodes) == 0:
            return None
        else:
            return tgt_nodes[0]

class TrieTree:
    """
    Classe que implementa uma árvore Trie.
    """
    def __init__(self):
        """
        Método construtor. Não recebe nenhuma entrada e inicializa as variáveis internas da árvore.
        """
        self.root = TrieNode(None,[],None)
        self.max_idx = -1
        
    
    def insert(self,node,str_list:list,custom_index=None):
        """
        Método que realiza a inserção de uma palavra na árvore Trie. Recebe uma lista de caracteres(string em forma de lista),
        e então percorre a árvore criando os novos nós necessários e adicionando um índice para o último nó da palavra.
        """
        str_size = len(str_list)
        if str_size > 0:
            curr_element = str_list.pop(0)
            str_size -= 1
            
            search_result = node.search_node(curr_element)

            if search_result == None:
                idx = None
                new_node = TrieNode(node_content=curr_element,child_nodes=[],index=idx)
                self.insert(new_node,str_list,custom_index=custom_index)
                node.child_nodes.append(new_node)
                
            elif str_size > 0:
                self.insert(search_result,str_list,custom_index=custom_index)
        else:
            if custom_index == None:
                self.max_idx += 1
                node.index = self.max_idx
            else:
                node.index = custom_index

    def search_word(self,node,str_list:list):  
        """
        Método que procura se uma dada palavra está na árvore trie a partir de um dado nó.
        Caso encontrada, a função retorna uma tuple com uma variável booleana verdadeira e o índice equivalente à palavra.
        Caso contrário, retorna uma tuple (False,None).
        """
        str_size = len(str_list)
        if str_size > 0:
            curr_element = str_list.pop(0)
            str_size -= 1
            search_result = node.search_node(curr_element)

            if search_result == None:
                return (False,None)
                
            elif str_size > 0:
                return self.search_word(search_result,str_list)
            else:
                if search_result.index != None:
                    return (True,search_result.index)
                else:
                    return (False,None)
            
        else:
            return (False,None)   
        

def int_to_bin(x:int,n_bits:int) -> str:
    """
    Função que converte um inteiro para uma string binária de tamanho n_bits.
    """
    return format(x,'b').zfill(n_bits)


def lz78_encode(binpt_string:str) -> str:
    """
    Função que implementa o encoding de acordo com o algoritmo de Lempel-Ziv. 
    """
    # Primeiramente, criamos a árvore Trie e inserimos o caractere vazio nela
    t = TrieTree() 
    t.insert(t.root,[""],0)
    
    # Agora inicializamos algumas variáveis auxiliares
    curr_word = ""
    trie_idx = 1

    encoding = []
     
    str_size = len(binpt_string) - 1
    
    # Loop Principal do programa
    for curr_pos,bin_char in enumerate(binpt_string):
        # Pegamos o prefixo e a palavra atual
        prefix = curr_word
        curr_word += bin_char 
        
        # Buscamos pela palavra na trie
        is_in_trie,idx = t.search_word(t.root,list(curr_word))
        
        # Formatação para garantir bom comportamento
        prefix_list = [""] if prefix == "" else list(prefix)
        
        # Caso o padrão não esteja na trie
        if not is_in_trie:
            # Adicionamos o padrão e buscamos pelo índice associado ao prefixo do padrão
            t.insert(t.root,list(curr_word),trie_idx)
            _,prefix_idx = t.search_word(t.root,prefix_list)
            
            # O encoding então recebe o par (indice_prefixo,novo caractere)
            encoding.append([prefix_idx,bin_char])
            curr_word = ""

            trie_idx += 1
        
        # Caso o padrão esteja na trie mas esse seja o final da string, devemos adicioná-lo ao encoding de qualquer forma
        if is_in_trie and curr_pos == str_size:
            _,prefix_idx = t.search_word(t.root,prefix_list)
            encoding.append([prefix_idx,bin_char])
    
    # O encoding binário final então é a concatenação dos encodings obtidos, lembrando sempre de considerar a mudança 
    # no número de bits utilizado como referência a cada passo, e.g., o primeiro caractere sempre utilizará apenas um bit
    # para referenciar o endereço 0, mas caracteres mais a frente utilizaram mais bits para referenciar o 0 e etc.
    binary_encoding = ""
    for trie_idx,curr_code in enumerate(encoding):
        num_bits = int(np.ceil(np.log2(trie_idx+1)))
        binary_encoding += int_to_bin(curr_code[0],num_bits) + curr_code[1]

    return binary_encoding

def lz78_decode(encoded_str:str) -> str:
    """
    Função que implementa o decoding de acordo com o algoritmo de Lempel-Ziv. 
    """    
    
    # Primeiro criamos um dicionario para mapear indices para palavras
    decode_dict = {}
    decode_dict[0] = ""
    
    binary_decoding = ""

    str_size = len(encoded_str) - 1

    trie_idx = 1

    curr_pos = 0
    word_count = 0

    # Loop principal
    while curr_pos < str_size:
        # Calculamos a quantidade de bits necessária para os endereços no passo atual
        num_bits = max(1,int(np.ceil(np.log2(word_count+1))))
        
        # Recuperamos o índice de referência(índice que referencia algum padrão já observado), e também a letra atual
        ref_idx = int(encoded_str[curr_pos:curr_pos+num_bits],2)
        curr_word = encoded_str[curr_pos+num_bits:curr_pos+num_bits+1]

        # Buscamos pela palavra associada ao índice de referência (prefixo)
        prefix = decode_dict[ref_idx]

        # Adicionamos o prefixo e a letra ao encoding final
        binary_decoding += prefix + curr_word
        
        # Inserimos o prefixo e a letra no dicionario
        decode_dict[trie_idx] = prefix + curr_word
        trie_idx += 1


        curr_pos += num_bits + 1
        word_count += 1
    return binary_decoding



if __name__ == "__main__":
    # Recebe os arguemntos da linha de comando
    parser = argparse.ArgumentParser()
    parser.add_argument("-c",type=str,help="Indica o nome do arquivo para compressão")
    parser.add_argument("-d",type=str,help="Indica o nome do arquivo para descompressão")
    parser.add_argument("-o",type=str,help="Indica o nome do arquivo que será salvo")

    args = parser.parse_args()

    # Caso se queira comprimir
    if args.c != None:

        outfile_suffix = ".z78"
        
        # Carregamos os bits do arquivo
        loaded_bytes = np.fromfile(args.c, dtype = "uint8")
        bit_array = np.unpackbits(loaded_bytes)

        bstr = "".join([str(i) for i in bit_array])

        # Realizamos o encoding dos bits
        encoded_str = lz78_encode(bstr)

        # Decidimos o nome do arquivo que será salvo
        if args.o != None:
            outfile_name = args.o + outfile_suffix
        else:
            outfile_name = args.c.split(".")[0] + outfile_suffix

        # Convertemos a string de bits para bytes e então salvamos o arquivo 
        #(Note que, como salvaremos os bytes no arquivo txt, ao abrir o respectivo arquivo em um leitor de texto vizualiaremos apenas os simbolos que correspondem aos bytes, 
        #e não os bytes em formato binário)
        np.packbits(np.array([int(i,2) for i in encoded_str])).tofile(outfile_name)

    elif args.d != None:
        outfile_suffix = ".txt"

        loaded_bytes = np.fromfile(args.d, dtype = "uint8")
        bit_array = np.unpackbits(loaded_bytes)

        encoded_str = "".join([str(i) for i in bit_array])

        decoded_str = lz78_decode(encoded_str)

        if args.o != None:
            outfile_name = args.o + outfile_suffix
        else:
            outfile_name = args.d.split(".")[0] + "_dec"+ outfile_suffix

        np.packbits(np.array([int(i,2) for i in decoded_str])).tofile(outfile_name)
