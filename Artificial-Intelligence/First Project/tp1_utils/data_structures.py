class Matrix:
    """
    Estrutura representando uma Matriz.
    
    """
    def __init__(self,content:list,nrows:int,ncols:int):
        """
        Método init que recebe uma lista indicando o array que deve ser transformado em matriz,
        e dois inteiros indicando o número de linhas e colunas, respectivamente.
        
        """
        self.content = content
        self.nrows = nrows
        self.ncols = ncols

    def check_input(self,row:int,col:int):
        """
        Método que checa se um input de acesso a posição da matriz é válido.
        Recebe dois inteiros indicando os índices a serem acessados e da raise em um Erro caso sejam invalidos.
        
        """
        if col >= self.ncols or col < 0:
            raise ValueError
        if row >= self.nrows or row < 0:
            raise ValueError

    def get(self,row:int,col:int):
        """
        Método get.
        Recebe dois inteiros indicando os índices e retorna o elemento correspondente da matriz.
        
        """
        self.check_input(row,col)
        return self.content[(row * self.nrows) + col]

    def set(self,row:int,col:int,value:int):
        """
        Método set.
        Recebe dois inteiros indicando os índices e um valor a ser armazenado.
        Não retorna nada.
        
        """
        self.check_input(row,col)
        self.content[(row * self.nrows) + col] = value

    def copy(self):
        """
        Método copy.
        Retorna uma cópia da matriz.
        
        """
        return Matrix(self.content.copy(),self.nrows,self.ncols)

class State:
    """
    Estrutura representando um estado.
    
    """
    def __init__(self,state_arr:Matrix=None,frame_row:int=None,frame_col:int=None):
        """
        Método init que recebe uma matriz com o tabuleiro, e dois inteiros indicando a posição vazia no tabuleiro.
        
        """
        self.arr = state_arr
        self.frame_row = frame_row
        self.frame_col = frame_col

    def copy(self):
        """
        Método copy.
        Retorna uma cópia do estado.
        
        """
        return State(self.arr.copy(),self.frame_row,self.frame_col)

class Node:
    """
    Estrutura representando um nó.
    
    """
    def __init__(self,state:State=None,parent_node=None,depth:int=0,cost:int=0,state_hash:str=None):
        """
        Método init que recebe um estado, um nó pai, um inteiro indicando a profundidade,
        um inteiro indicando o custo do nó, e uma string indicando o hash do estado.
        
        """
        self.state = state
        self.depth = depth
        self.parent_node=parent_node
        self.cost = cost
        self.hash = state_hash
        
    def __lt__(self,other):
        """
        Método que sobrecarrega a operação de menor que. 
        Recebe um objeto que possua atributo custo. 
        Retorna 1 se o custo do Nó atual for menor que o do objeto recebido como entrada, e 0 caso contrário.
        
        """
        if other == None:
            return False
        return self.cost < other.cost

    def __eq__(self,other):
        """
        Método que sobrecarrega a operação de igualdade.
        Recebe um objeto que possua atributo hash. 
        Retorna 1 se os hashs são iguais, e zero caso contrário.
        
        """
        if other == None:
            return False
        return self.hash == other.hash

    def copy(self):
        """
        Método copy.
        Retorna uma cópia do nó.
        
        """
        return Node(self.state.copy(),parent_node=self.parent_node,
                    depth=self.depth,cost=self.cost,state_hash=self.hash)


