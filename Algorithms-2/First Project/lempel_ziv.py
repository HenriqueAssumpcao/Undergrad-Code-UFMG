import numpy as np
from trie_tree import TrieTree


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