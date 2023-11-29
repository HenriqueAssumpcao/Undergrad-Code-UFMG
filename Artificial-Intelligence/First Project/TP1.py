"""
TRABALHO PRÁTICO 1 - INTRODUÇÃO A INTELIGÊNCIA ARTIFICIAL, UFMG, 2023/2
ALUNO: HENRIQUE SOARES ASSUMPÇÃO E SILVA
MATRÍCULA: 2020006620
CURSO: CIÊNCIA DA COMPUTAÇÃO
"""

import sys

from tp1_utils.data_structures import Matrix,Node,State
from tp1_utils.aux_functions import process_input,print_path
from tp1_utils.algorithms import bfs,ids,dks,grs,ass,hcs
from tp1_utils.heuristics import manhattan_dist,num_off_tiles

if __name__ == "__main__":
    """
    Main do programa. Recebe os argumentos da linha de comando, checa se são válidos, e então executa o respectivo algoritmo.
    """
    argv = sys.argv[1:]

    if len(argv) < 10 or len(argv) > 11:
        raise ValueError("Incorrect number of arguments passed!")

    alg_id = argv[0]
    init_state = [int(i) for i in argv[1:10]]

    print_opt = False
    if len(argv) == 11 and argv[-1] == "PRINT":
        print_opt = True

    root_node = process_input(init_state,3,3)

    if(alg_id == "B"):
        _,goal_node,num_nodes = bfs(root_node)
    elif(alg_id == "I"):
        _,goal_node,num_nodes = ids(root_node)
    elif(alg_id == "U"):
        _,goal_node,num_nodes = dks(root_node)
    elif(alg_id == "G"):
        _,goal_node,num_nodes = grs(root_node,num_off_tiles)
    elif(alg_id == "A"):
        _,goal_node,num_nodes = ass(root_node,manhattan_dist)
    elif(alg_id == "H"):
        max_side_steps = 20
        goal_node,num_nodes = hcs(root_node,max_side_steps,num_off_tiles)
    else:
        raise ValueError("Invalid Algorithm Id! Please input one of the following: B,I,U,G,A,H")


    print(goal_node.depth)
    if print_opt:
        print("")
        print_path(goal_node)