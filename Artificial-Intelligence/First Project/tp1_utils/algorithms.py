from .data_structures import Node,Matrix,State
from .aux_functions import is_goal,expand_node
import heapq

# BFS
def bfs(root_node:Node) -> tuple:
    """
    Função que realiza a Busca BFS.
    Recebe um objeto de tipo Node indicando o nó raiz da busca.
    Retorna uma tupla (#1,#2,#3), onde #1 é um valor booleano indicando se a busca foi bem sucedida, 
    #2 é um objeto Node que em caso de sucesso é o nó alvo e em caso de falha é o próprio nó raiz,
    #3 é um inteiro indicando o número de nós explorados.
    
    """
    if is_goal(root_node):
        return (1,root_node,1)

    curr_node = root_node.copy()
    curr_hash = str(curr_node.state.arr.content)
    
    frontier = [curr_node]
    frontier_hash = [curr_hash]
    explored_hash = set()

    while(frontier):
        curr_node = frontier.pop(0)
        explored_hash.add(frontier_hash.pop(0))

        new_states = expand_node(curr_node)
        for new_state in new_states:
            new_child_hash = str(new_state.arr.content)
            if new_child_hash not in explored_hash and new_child_hash not in frontier_hash:
                new_child = Node(state=new_state,parent_node=curr_node,depth=curr_node.depth+1)
                if is_goal(new_child):
                    return (1,new_child,len(explored_hash))
                
                frontier.append(new_child)
                frontier_hash.append(new_child_hash)

    return (0,root_node,len(explored_hash))

# Dijkstra
def dks(root_node:Node) -> tuple:
    """
    Função que realiza a Busca DKS.
    Recebe um objeto de tipo Node indicando o nó raiz da busca.
    Retorna uma tupla (#1,#2,#3), onde #1 é um valor booleano indicando se a busca foi bem sucedida, 
    #2 é um objeto Node que em caso de sucesso é o nó alvo e em caso de falha é o próprio nó raiz,
    #3 é um inteiro indicando o número de nós explorados.
    
    """
    if is_goal(root_node):
        return (1,root_node,1)

    curr_node = root_node.copy()

    frontier = [curr_node]
    explored_hash = set()

    while(frontier):
        curr_node = frontier.pop(0)
        explored_hash.add(curr_node.hash)

        if is_goal(curr_node):
            return (1,curr_node,len(explored_hash))

        new_states = expand_node(curr_node)
        for new_state in new_states:
            new_child_hash = str(new_state.arr.content)
            new_child = Node(state=new_state,parent_node=curr_node,
                             depth=curr_node.depth+1,state_hash=new_child_hash)
            new_child.cost = curr_node.depth+1
            
            if new_child_hash not in explored_hash:  
                if new_child not in frontier:
                    heapq.heappush(frontier,new_child)
                else:
                    node_idx = frontier.index(new_child)
                    if(frontier[node_idx].cost > new_child.cost):
                        frontier[node_idx] = new_child

    return (0,root_node,len(explored_hash))

# Iterative Deepening
def limited_dfs(root_node:Node,max_depth:int) -> tuple:
    """
    Função que realiza a Busca DFS limitada.
    Recebe um objeto de tipo Node indicando o nó raiz da busca e um inteiro indicando a profundidade máxima.
    Retorna uma tupla (#1,#2,#3,#4), onde #1 é um valor booleano indicando se a busca foi bem sucedida, 
    #2 é um objeto Node que em caso de sucesso é o nó alvo e em caso de falha é o próprio nó raiz,
    #3 é um inteiro indicando o número de nós explorados, e #4 é um booleano indicando se todos os nós foram explorados, i.e., se houve corte.
    Caso o nó alvo seja encontrado, o valor de #4 é irrelevante.
    
    """
    if is_goal(root_node):
        return (1,root_node,1,0)

    curr_node = root_node.copy()
    frontier = [curr_node]

    explored_counter = 0
    explored_all_states = 1

    while(frontier):
        curr_node = frontier.pop()
        explored_counter += 1
        
        if is_goal(curr_node):
            return (1,curr_node,explored_counter,explored_all_states)
            
        if curr_node.depth > max_depth:
            explored_all_states = 0
        else:
            new_states = expand_node(curr_node)
            for new_state in new_states:
                new_child = Node(state=new_state,parent_node=curr_node,depth=curr_node.depth+1)
                frontier.append(new_child)
                        
    return (0,root_node,explored_counter,explored_all_states)

def ids(root_node:Node) -> tuple:
    """
    Função que realiza a Busca IDS.
    Recebe um objeto de tipo Node indicando o nó raiz da busca.
    Retorna uma tupla (#1,#2,#3), onde #1 é um valor booleano indicando se a busca foi bem sucedida, 
    #2 é um objeto Node que em caso de sucesso é o nó alvo e em caso de falha é o próprio nó raiz,
    #3 é um inteiro indicando o número de nós explorados.
    
    """
    max_depth = 0
    explored_all_states = 0
    success = 0
    
    while(not success and not explored_all_states):
        success,out_node,num_explored_nodes,explored_all_states = limited_dfs(root_node,max_depth)
        max_depth += 1

    return (success,out_node,num_explored_nodes)

# A*
def ass(root_node:Node,heuristic_func) -> tuple:
    """
    Função que realiza a Busca ASS.
    Recebe um objeto de tipo Node indicando o nó raiz da busca, e uma função heurística que deve receber um nó e retornar um valor numérico.
    Retorna uma tupla (#1,#2,#3), onde #1 é um valor booleano indicando se a busca foi bem sucedida, 
    #2 é um objeto Node que em caso de sucesso é o nó alvo e em caso de falha é o próprio nó raiz,
    #3 é um inteiro indicando o número de nós explorados.
    
    """
    if is_goal(root_node):
        return (1,root_node,1)
        
    curr_node = root_node.copy()
    curr_node.cost = curr_node.depth + heuristic_func(curr_node)

    frontier = [curr_node]
    explored_hash = set()

    while(frontier):
        curr_node = frontier.pop(0)
        explored_hash.add(curr_node.hash)
        
        if is_goal(curr_node):
            return (1,curr_node,len(explored_hash))

        new_states = expand_node(curr_node)
        for new_state in new_states:
            new_child_hash = str(new_state.arr.content)
            new_child = Node(state=new_state,parent_node=curr_node,
                             depth=curr_node.depth+1,state_hash=new_child_hash)
            new_child.cost = new_child.depth + heuristic_func(new_child)

            if new_child_hash not in explored_hash:
                if new_child not in frontier:
                    heapq.heappush(frontier,new_child)
                else:
                    node_idx = frontier.index(new_child)
                    if(frontier[node_idx].depth > new_child.depth):
                        frontier[node_idx] = new_child

    return (0,root_node,len(explored_hash))

# Greedy
def grs(root_node:Node,heuristic_func) -> tuple:
    """
    Função que realiza a Busca GRS.
    Recebe um objeto de tipo Node indicando o nó raiz da busca, e uma função heurística que deve receber um nó e retornar um valor numérico.
    Retorna uma tupla (#1,#2,#3), onde #1 é um valor booleano indicando se a busca foi bem sucedida, 
    #2 é um objeto Node que em caso de sucesso é o nó alvo e em caso de falha é o próprio nó raiz,
    #3 é um inteiro indicando o número de nós explorados.
    
    """
    if is_goal(root_node):
        return (1,root_node,1)

    curr_node = root_node.copy()
    curr_node.cost = heuristic_func(curr_node)

    frontier = [curr_node]
    explored_hash = set()

    while(frontier):
        curr_node = frontier.pop(0)
        explored_hash.add(curr_node.hash)

        if is_goal(curr_node):
            return (1,curr_node,len(explored_hash))

        new_states = expand_node(curr_node)
        for new_state in new_states:
            new_child_hash = str(new_state.arr.content)
            new_child = Node(state=new_state,parent_node=curr_node,
                             depth=curr_node.depth+1,state_hash=new_child_hash)
            new_child.cost = heuristic_func(new_child)
            
            if new_child_hash not in explored_hash:
                if new_child not in frontier:
                    heapq.heappush(frontier,new_child)

    return (0,root_node,len(explored_hash))

# HCS
def hcs(root_node:Node,max_side_steps:int,cost_func) -> tuple:
    """
    Função que realiza a Busca HCS.
    Recebe um objeto de tipo Node indicando o nó raiz da busca, um inteiro indicando a quantidade máxima de passos laterais, 
    e uma função de custo que deve receber um nó e retornar um valor numérico indicando o quão longe o nó está do estado alvo.
    Retorna uma tupla (#1,#2), #1 é um objeto Node que é um mínimo local w.r.t. função heurística,
    e #2 é um inteiro indicando a quantidade de nós explorados.
    
    """
    curr_node = root_node.copy()
    curr_node.cost = cost_func(curr_node)

    explored_counter = 0
    side_steps = 0
    
    while True:
        new_states = expand_node(curr_node)
        explored_counter += 1

        best_child = None
        best_child_cost = float("inf")
        for new_state in new_states:
            new_child = Node(state=new_state,parent_node=curr_node,depth=curr_node.depth+1)
            new_child.cost = cost_func(new_child)

            if new_child.cost < best_child_cost:
                best_child = new_child
                best_child_cost = new_child.cost
                
        if best_child.cost > curr_node.cost:
            return (curr_node,explored_counter)
        elif best_child.cost == curr_node.cost:
            side_steps += 1
            if side_steps == max_side_steps:
                return (curr_node,explored_counter)
        else:
            side_steps = 0
            
        curr_node = best_child
