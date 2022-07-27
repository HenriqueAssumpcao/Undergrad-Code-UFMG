import numpy as np  
import sys  
from matching_functions import min_cost_perfect_matching

if __name__ == "__main__":
    num_rows = 0  
    N = []

    for lin_num,line in enumerate(sys.stdin):
        if lin_num == 0:
            line_content = line.split()
            num_nodes = int(line_content[0])
            num_rows = 2* num_nodes
            num_cols = int(line_content[1])
        elif lin_num == num_rows+1:
            edge_weights = np.array([int(i) for i in line.split()]).reshape(-1)
        else:
            N.append([int(i) for i in line.split()])

        if lin_num == num_rows+1:
            break  
    N = np.array(N)
    tgt_set = set([*range(num_nodes)])
    comp_set = set([*range(num_nodes,num_nodes*2)])

    edge2id = {}
    id2edge = {}
    for (edge_id,edge) in enumerate(range(N.shape[1])):
        curr_nodes = tuple(np.where(N[:,edge] == 1)[0])
        edge2id[curr_nodes] = edge_id
        id2edge[edge_id] = curr_nodes

    results = min_cost_perfect_matching(N,tgt_set,comp_set,edge_weights,edge2id,id2edge)

    if results[0] == 1:
        print(results[1])
        np.savetxt(sys.stdout, results[2].reshape(1,-1), fmt='%d')
        certificate_str = ""
        for i in range(len(results[3])):
            if i < len(results[3])-1:
                certificate_str += str(results[3][i]) + " "
            else:
                certificate_str += str(results[3][i])
        print(certificate_str)

    else:
        print(results[0])
        np.savetxt(sys.stdout, results[1][0].reshape(1,-1), fmt='%d')
        np.savetxt(sys.stdout, results[1][1].reshape(1,-1), fmt='%d')