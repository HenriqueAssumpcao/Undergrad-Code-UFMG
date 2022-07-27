import numpy as np  

def find_dangling_nodes(N:np.array) -> np.array:
    """
    Função que encontra todos os nós pendentes de um grafo.
    Recebe a matriz de incidência e retorna um array com os nós pendentes.
    """
    num_edges = np.array([np.sum(N[curr_node,:]) for curr_node in range(N.shape[0])])
    dangling_nodes = np.where(num_edges == 0)[0]
    
    return dangling_nodes

def matching_bfs(N:np.array,init_node:int,tgt_set:list,curr_matching:list,id2edge:dict) -> tuple:
    """
    Função que executa um caminhamento BFS específico para o matching, i.e., considerando apenas as arestas no emparelhamento
    quando saindo de nós pertencentes ao complemento do conjunto target (tgt_set).
    Recebe a matriz de incidência, o nó inicial para começar o caminhamento, o conjunto target ao qual o nó pertence,
    o emparelhamento atual e um dicionário que mapeia ids para arestas.
    Retorna uma tupla contendo as folhas da árvore BFS e as camadas da árvore.
    """
    # Inicializa a fila e os nós vizitados
    queue = []
    visited_nodes = []
    
    # Adiciona o nó inicial e um identificador b'0' para agir como um FLAG identificando cada nível do caminhamento
    queue.append(init_node)
    queue.append(b'0')
    visited_nodes.append(init_node)
    
    bfs_layers = []
    
    curr_layer = []
    leaves = []
    
    # Inicializa o caminhamento
    while queue != [b'0']:
        curr_node = queue.pop(0)
        # Caso o nó seja a flag, indica que estamos mudando de nível
        if curr_node == b'0':
            if len(curr_layer) > 0:
                bfs_layers.append(curr_layer)
            curr_layer = []
            queue.append(b'0')
            continue
        
        # Identifica as arestas que incidem em curr_node
        inc_edges = np.where(N[curr_node,:] == 1)[0]
        
        if curr_node in tgt_set:
            # Caso o curr_node seja do conjunto A, então olha para todos os vizinhos
            curr_neighbors = [id2edge[e_i][1] for e_i in inc_edges]
        else:
            # Caso o curr_node seja de B, então olha somento para os vizinhos por meio de arestas emparelhadas
            curr_neighbors = [id2edge[e_i][0] for e_i in inc_edges if e_i in curr_matching]
        
        non_visited_counter = 0 # Contador para identificar folhas
        # Itera sobre os vizinhos
        for curr_neighbor in curr_neighbors:
            if curr_neighbor not in visited_nodes:
                # Caso um vizinho não esteja vizitado, o adiciona a fila e continua
                visited_nodes.append(curr_neighbor)
                queue.append(curr_neighbor)
                curr_layer.append((curr_node,curr_neighbor))
                non_visited_counter += 1
                
        if non_visited_counter == 0:
            # Se o nó atual só possui vizinhos que já foram vizitados, ele é uma folha da árvore BFS
            leaves.append(curr_node)
    
       
    return [np.array(leaves),bfs_layers]

def find_perfect_matching(N:np.array,tgt_set:set,comp_set:set,edge2id:dict,id2edge:dict) -> tuple:
    """
    Função que busca por um emparelhamento perfeito em um grafo bipartido.
    Recebe a matriz de incidência, os conjuntos de vértices de cada classe, e dois dicionários,
    um mapeando arestas para ids e outro mapeando ids para arestas.
    Retorna uma tupla com dois elementos (E1,E2). E1 é um booleano que identifica se o algoritmo conseguiu encontrar
    um emparelhamento perfeito e E2 é um conjunto que comprova o resultado E1.
    """
    
    # Primeiramente checa se existem nós pendentes no grafo
    dangling_nodes = find_dangling_nodes(N)
    
    # Caso positivo, não existe emparelhamento perfeito
    if len(dangling_nodes) > 0:
        dangling_tgt = [i for i in dangling_nodes if i in tgt_set]
        
        if len(dangling_tgt) > 0:
            return (False,dangling_tgt)
        else:
            return (False,list(tgt_set))    
    
    num_nodes = len(tgt_set)
    is_matched = np.zeros(2*num_nodes,dtype=bool)
    matching = []

    for curr_node in tgt_set: # Itera sobre todos os nós do tgt_set (um dos conjuntos de nós do grafo bipartido)
        
        inc_edges = np.where(N[curr_node,:] == 1)[0] # Recebe as arestas que incidem no nó
        
        # Checa se existem vizinhos do nó que não estão emparelhados
        non_matched_neighbors = []
        for e_i in inc_edges:
            curr_neighbor = [tgt_node for tgt_node in np.where(N[:,e_i] == 1)[0] if tgt_node != curr_node and is_matched[tgt_node]==0]
            if len(curr_neighbor) == 1:
                non_matched_neighbors.append(curr_neighbor[0])
        # Caso positivo, emparelha o nó com um de seus vizinhos e passa para o próximo
        if len(non_matched_neighbors) > 0:
            tgt_neighbor = non_matched_neighbors[0]
            matching.append(edge2id[tuple(sorted((curr_node,tgt_neighbor)))])
            is_matched[curr_node] = 1
            is_matched[tgt_neighbor] = 1
        else: 
            # Caso contrário, passa para o procedimento de realizar um BFS para encontrar um emparelhamento maior
            # Realiza um BFS a partir de curr_node, considerando apenas as arestas emparelhadas quando caminhando de nós
            # que pertecem ao outro conjunto de nós
            bfs_leaves,layers = matching_bfs(N,curr_node,tgt_set,matching,id2edge)
            
            # Checa dentre os nós folha da árvore bfs aqueles que não pertencem ao conjunto original
            tgt_nodes = [node for node in bfs_leaves if node not in tgt_set]
            if len(tgt_nodes) > 0:
                # Caso existam, pega o primeiro dentre esses nós
                tgt_node = tgt_nodes[0]
                
                # Reconstrói o caminho deste nó até a raiz
                path = []
                for layer in layers[::-1]:
                    for (u,v) in layer:
                        if v == tgt_node:
                            path.append(edge2id[tuple(sorted((u,v)))])
                            tgt_node = u
                # Identifica quais arestas já emparelhadas estão no caminho e quais não estão
                edge_ids_to_remove = [idx for idx in range(len(matching)) if matching[idx] in path]
                edges_to_add = [edge for edge in path if edge not in matching]
                
                # Remove as arestas identificadas do emparelhamento
                for (offset,idx) in enumerate(edge_ids_to_remove):
                    matching.pop(idx-offset)
                # Adiciona as novas arestas do caminho ao emparelhamento
                for new_edge in edges_to_add:
                    matching.append(new_edge)
                    u,v = id2edge[new_edge]
                    is_matched[u] = True
                    is_matched[v] = True
            else:
                # Caso não exista nós folha que pertençam a outra classe, significa que o grafo não admite emparelhamento perfeito
                # e portanto retorna o conjunto de vértices em tgt_set que é maior qu seu conjunto de vizinhos
                unbalanced_set = []
                for layer in layers:
                    for (u,v) in layer:
                        tgt_node = u if u in tgt_set else v
                        unbalanced_set.append(tgt_node)
                return (False,list(set(unbalanced_set)))
            
    return (True,matching)


def min_cost_perfect_matching(N:np.array,tgt_set:set,comp_set:set,edge_weights:np.array,edge2id:dict,id2edge:dict) -> tuple:
    """
    Função que busca por um emparelhamento perfeito de custo mínimo em um grafo bipartido.
    Recebe a matriz de incidência, os conjuntos de vértices de cada classe,um array de pesos das arestas, e dois dicionários,
    um mapeando arestas para ids e outro mapeando ids para arestas.
    Retorna uma tupla (E1,E2) com dois elementos. E1 é um inteiro que vale 1 caso exista emparelhamento perfeito de custo mínimo
    ou -1 caso contrário. E2 contém um certificado do resultado de E1.
    """
    # Checa primeiramente se existe algum emparelhamento perfeito
    is_there_pm,matching_result = find_perfect_matching(N,tgt_set,comp_set,edge2id,id2edge)

    # Caso positivo
    if is_there_pm:
        # Inicializa os pesos dos vértices (formulação dual) como sendo metade do menor peso das arestas
        init_weight = np.min(edge_weights)
        node_weights = init_weight/2 * np.ones(N.shape[0])
        is_there_pm_min = False

        # Inicializa a matriz de incidência do grafo auxiliar H
        N_h = np.zeros(N.shape)

        while(not is_there_pm_min):    
            N_h = 0 * N_h
            # Preenche com as arestas que são satisfeitas com igualdade (y_u + y_v = c_uv)

            tgt_edges = np.where(N.T @ node_weights == edge_weights)[0]
            for tgt_edge in tgt_edges:
                (u,v) = id2edge[tgt_edge]
                N_h[u,tgt_edge] = 1
                N_h[v,tgt_edge] = 1

            # Idenfica quais nós pertencem ao grafo auxiliar H não são pendentes
            h_nodes = np.where(np.sum(N_h,axis=1) > 0)[0]

            is_there_pm_min,matching_result = find_perfect_matching(N_h,tgt_set,comp_set,edge2id,id2edge)
            

            # Se não existe emparelhamento perfeito mínimo
            if not is_there_pm_min:

                # Inicializa o eps como infinito
                viable_eps = np.inf
                pos_idx = 1 if matching_result[0] in tgt_set else 0 # variável indicando qual posição da aresta representa o vizinho (se o nó está em A, como por padrão as arestas são sempre de A para B, deve pegar o da posição 1)

                # Computa todos os vizinhos do conjunto desbalanceado atual em H
                inc_edges_h = [np.where(N_h[i,:] == 1)[0] for i in matching_result]
                matching_result_neighbors_h = set([id2edge[e_i][pos_idx] for neighbors in inc_edges_h for e_i in neighbors])

                # Itera sobre cada um dos nós no conjunto
                for curr_node in matching_result:
                    # Encontra os vizinhos de S que estão exclusivamente em G (N_G(S))
                    inc_edges_g = np.where(N[curr_node,:] == 1)[0]
                    exclusive_g_edges = [e_i for e_i in inc_edges_g if id2edge[e_i][pos_idx] not in matching_result_neighbors_h]
                    neighbor_edge_weights = edge_weights[exclusive_g_edges]
                    # Caso S possua vizinhos somente em G
                    if len(exclusive_g_edges) > 0:
                        # Computa todos os epsilons possíveis e então escolhe o menor (viável)
                        epsilons = [edge_weights[e_i] - node_weights[curr_node] - node_weights[id2edge[e_i][pos_idx]] for e_i in exclusive_g_edges]
                        curr_eps = np.min(epsilons)
                    else:
                        # Caso contrário o epsilon é infinito
                        curr_eps = np.inf

                    if viable_eps > curr_eps and curr_eps > 0 :
                        # Se o epsilon atual for menor que o epsilon viável global, trocamos o epsilon viável global por ele
                        viable_eps = curr_eps
                # Ao final, obtemos o maior epsilon possível que ainda é viável em G

                # Agora aumentamos o peso dos nós do conjunto por epsilon e diminuímos o de seus vizinhos em H por epsilon
                visited_nodes = []
                for curr_node in matching_result:
                    inc_edges_h = np.where(N_h[curr_node,:] == 1)[0]
                    curr_neighbors_h = ([id2edge[e_i][pos_idx] for e_i in inc_edges_h])

                    node_weights[curr_node] += viable_eps
                    for neighbor in curr_neighbors_h:
                        if neighbor not in visited_nodes:
                            node_weights[neighbor] -= viable_eps
                            visited_nodes.append(neighbor)

        # Ao final do loop, obtemos um emparelhamento perfeito de custo mínimo
        final_matching = np.zeros(N.shape[1])
        final_matching[matching_result] = 1
        matching_cost = np.dot(edge_weights,final_matching)

        return (1,matching_cost,final_matching,node_weights)
    else:
        # Formata a resposta de acordo com o formato esperado pelo enunciado
        inc_edges = [np.where(N[i,:] == 1)[0] for i in matching_result]
        neighbors = np.array(list(set([id2edge[e_i][1] for neighbors in inc_edges for e_i in neighbors])))
        
        tgt_set_sol = np.zeros(len(tgt_set))
        comp_set_sol = np.zeros(len(comp_set))

        tgt_set_sol[matching_result] = 1
        
        if len(neighbors) > 0:
            comp_set_sol[neighbors - len(comp_set)] = 1

            
        return (-1,[tgt_set_sol,comp_set_sol])
