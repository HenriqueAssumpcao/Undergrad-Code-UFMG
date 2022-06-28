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