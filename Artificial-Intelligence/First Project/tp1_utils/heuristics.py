from .data_structures import Node,Matrix,State

def num_off_tiles(node:Node,goal:list = [1,2,3,4,5,6,7,8,0]) -> int:
    """
    Função que computa a heurística de número de peças fora de posição.
    Recebe um objeto de tipo Node, e uma lista contendo o estado alvo do jogo.
    Retorna um inteiro indicando a quantidade de peças fora de posição.
    
    """
    num_off_tiles = 0
    for idx,tile in enumerate(node.state.arr.content):
        if(tile != 0 and tile != goal[idx]):
            num_off_tiles += 1
    
    return num_off_tiles

def manhattan_dist(node:Node,goal:list = [1,2,3,4,5,6,7,8,0]) -> int:
    """
    Função que computa a heurística de manhattan.
    Recebe um objeto de tipo Node, e uma lista contendo o estado alvo do jogo.
    Retorna um inteiro indicando a soma das distâncias de manhattan entre as peças do estado do Nó e o do estado alvo.
    
    """
    board_arr = node.state.arr.content
    dist_arr = [abs(board_arr.index(i)%3 - goal.index(i)%3) + abs(board_arr.index(i)//3 - goal.index(i)//3) for i in range(1, 9)]
    return sum(dist_arr)