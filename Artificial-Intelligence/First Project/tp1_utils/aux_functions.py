from .data_structures import Node,Matrix,State

def is_goal(node:Node) -> int:
    """
    Função que checa se um nó possui o estado alvo.
    Recebe um objeto de tipo Node e retorna 1 caso positivo e 0 caso negativo.
    
    """
    state = node.state
    if state.arr.content == [1,2,3,4,5,6,7,8,0]:
        return 1
    return 0

def expand_node(node:Node) -> list:
    """
    Função sucessora de um dado nó para o problema do 8-puzzle.
    Recebe um objeto de tipo Node e retorna uma lista contendo os novos estados atingíveis a partir do estado do nó.
    
    """
    new_states = []
    frame_row = node.state.frame_row
    frame_col = node.state.frame_col
    # Up
    if(frame_row > 0):
        up_arr = Matrix(node.state.arr.content.copy(),node.state.arr.nrows,node.state.arr.ncols)
        up_arr.set(frame_row - 1,frame_col,0)
        up_arr.set(frame_row,frame_col,node.state.arr.get(frame_row - 1,frame_col))
        new_states.append(State(up_arr,frame_row - 1,frame_col))
    # Down
    if(frame_row < 2):
        down_arr = Matrix(node.state.arr.content.copy(),node.state.arr.nrows,node.state.arr.ncols)
        down_arr.set(frame_row + 1,frame_col,0)
        down_arr.set(frame_row,frame_col,node.state.arr.get(frame_row + 1,frame_col))
        new_states.append(State(down_arr,frame_row + 1,frame_col))
    # Left
    if(frame_col > 0):
        left_arr = Matrix(node.state.arr.content.copy(),node.state.arr.nrows,node.state.arr.ncols)
        left_arr.set(frame_row,frame_col-1,0)
        left_arr.set(frame_row,frame_col,node.state.arr.get(frame_row,frame_col-1))
        new_states.append(State(left_arr,frame_row,frame_col - 1))
    # Right
    if(frame_col < 2):
        right_arr = Matrix(node.state.arr.content.copy(),node.state.arr.nrows,node.state.arr.ncols)
        right_arr.set(frame_row,frame_col+1,0)
        right_arr.set(frame_row,frame_col,node.state.arr.get(frame_row,frame_col+1))
        new_states.append(State(right_arr,frame_row,frame_col + 1))

    return new_states

def process_input(init_arr:list,nrows:int,ncols:int) -> Node:
    """
    Função que processa o input do usuário.
    Recebe uma lista contendo o estado inicial do tabuleiro,
    e dois inteiros indicando a quantidade de linhas e colunas do tabuleiro, respectivamente.
    Retorna um objeto de tipo Node contendo os dados de input.
    
    """
    arr = Matrix(init_arr,nrows,ncols)
    frame_row = 0
    frame_col = 0
    for i in range(nrows):
        for j in range(ncols):
            if(arr.get(i,j) == 0):
                frame_row = i
                frame_col = j

    return Node(State(arr,frame_row,frame_col),parent_node=None,depth=0,state_hash=str(init_arr))

def print_state(state:State):
    """
    Função que imprime um estado.
    Recebe um objeto de tipo State e não retorna nada.
    
    """
    for i in range(state.arr.nrows):
        for j in range(state.arr.ncols):
            if(state.arr.get(i,j) != 0):
                print(state.arr.get(i,j),end=' ')
            else:
                print(" ",end=' ')
        print("\n",end='')

def print_path(node:Node):
    """
    Função que imprime o caminho de um nó até a raiz.
    Recebe um objeto de tipo State e não retorna nada.
    """
    node_path = [node]
    curr_parent = node.parent_node
    while(curr_parent != None):
        node_path.insert(0,curr_parent)
        curr_parent = curr_parent.parent_node
    for node in node_path:
        print_state(node.state)
        print("")