import sys 
import numpy as np
def check_unlimited(curr_A:np.array,curr_c_tab:np.array,tgt_columns:np.array,curr_basis:np.array,round_tolerance:int):
    """
    Função que checa se uma PL em FPI é ilimitada, i.e., se existe uma coluna com coeficiente positivo(negativo no tableau)
    que possui somente valores não-positivos.
    
    Recebe a matriz A, o vetor c, um vetor tgt_columns com as colunas que possuem coeficientes negativos no tableau,
    um vetor curr_basis que contém os índices das colunas que formam a base atual e um valor inteiro que indica a tolerância
    de arredondamento.
    
    Retorna uma tupla indicando o resultado da avaliação, e caso a PL seja ilimitada também retorna o certificado de ilimitada.
    """
    for col in tgt_columns:
        if (np.round(curr_A[:,col],round_tolerance) <= 0).all():
            unlimited_certificate = np.zeros(curr_A.shape[1],dtype=np.float64)
            unlimited_certificate[col] = 1
            unlimited_certificate[curr_basis] = (-1 * np.round(curr_A[:,col],round_tolerance))
            return ("ilimitada",unlimited_certificate)
        
    return ("teste_negativo")


def simplex(A:np.array,b:np.array,c:np.array,aux_mode:bool=False,round_tolerance:int=8):
    """
    Função que implementa o Simplex. Recebe uma PL do tipo:
    Max <c,x>
    s.t. Ax <= b
    x >= 0
    e então retorna o resultado da avaliação do simplex.
    
    Recebe uma matriz A, vetores b e c, uma variável booleana aux_mode indicando se deve ser executado o simplex na PL
    ou na PL auxiliar, e uma variável inteira round_tolerance indicando a tolerância de arredondamento
    """
    if not aux_mode:
        # Inicialmente cria os elementos do tableu (A_tab,curr_basis,c_tab)
        A_tab = np.concatenate([A,np.eye(A.shape[0])],axis=1,dtype=np.float64) # Adds identity to A
        curr_basis = np.array([*range(A.shape[1],A_tab.shape[1])])
        c_tab = -1 * np.concatenate([c,np.zeros([A.shape[0],1])],axis=0,dtype=np.float64) # Adds 0s to c and multiplies by -1
        curr_c_tab = c_tab.copy()
        
        # Checa se a PL é trivialmente inviável, i.e., se existe uma linha não-negativa associada a um valor de b estritamente negativo
            
        for line_idx in range(len(b)):
            if ((A[line_idx,:] >= 0).all() and b[line_idx] < 0):
                unviable_certificate = np.zeros(len(b))
                unviable_certificate[line_idx] = 1
                return ("inviavel",unviable_certificate)
            
    
        curr_A = A_tab.copy()
        curr_b = b.copy()
    
        # Checa se a PL possui b's negativos, caso positivo procede para executar a PL auxiliar
        neg_flag = False
        if (b < 0).any():
            neg_flag = True
            # Chama o simplex no modo auxiliar(aux_mode = True)    
            curr_basis,unviable_certificate,optimal_value,viable_sol = simplex(A_tab.copy(),b.copy(),c_tab.copy(),aux_mode=True,round_tolerance=round_tolerance)
            
            # Checa se a PL é inviável
            if np.round(optimal_value,round_tolerance) < 0 :
                if unviable_certificate @ b > 0:
                    unviable_certificate = -1 * unviable_certificate
                return ("inviavel",unviable_certificate)
            

            # Assumindo que A é viável, checamos se a sub-matriz quadrada básica é singular, caso positivo não é possível utilizar uma base
            # que contém as colunas da matriz original que são L.D., e portanto devemos utilizar como base inicial as demais colunas da PL em FPI
            if np.round(np.linalg.det(A_tab[:,curr_basis]),round_tolerance) == 0:
                curr_basis = np.array([*range(A.shape[1],A_tab.shape[1])])
                
            # Agora, devemos transformar a PL para a forma canônica
            curr_basis_inv = np.linalg.inv(A_tab[:,curr_basis])
            curr_c_tab = c_tab + ((-1 * c_tab[curr_basis]).T @ curr_basis_inv @ A_tab).T
            curr_A = curr_basis_inv @ A_tab
            curr_b = curr_basis_inv @ b
            # Abaixo realizamos uma operação para melhorar a estabilidade numérica, i.e., curr_A contém a matriz atual
            # pré-multiplicada pela matriz inversa w.r.t. base escolhida pelo resultado do simplex na PL auxiliar, e sabemos que
            # na j-ésima linha da matriz, as entradas correspondentes à colunas da base que não são a que equivale a j devem ser 0
            # enquanto a entrada da coluna que equivale a j deve ser 1. Isso já é garantido em tese pela operação de inversão, porém
            # tal operação pode acarretar em valores muito próximos de zero que afetam o desempenho numérico do programa
            
            for i in range(len(curr_A)):
                curr_A[i,curr_basis] = 0
                curr_A[i,curr_basis[i]] = 1
            curr_c_tab[curr_basis] = 0
               
    else:
        # Criando a PL auxiliar. Primeiro, multiplicamos cada linha correspondente a um valor de b negativo por -1
        neg_flag = True
        
        neg_b_idxs = np.where(b < 0)[0]
        b[neg_b_idxs] = -1*b[neg_b_idxs]
        A[neg_b_idxs,:] = -1*A[neg_b_idxs,:]
        
        # Criamos a PL auxiliar
        A_tab = np.concatenate([A,np.eye(A.shape[0])],axis=1,dtype=np.float64) 
        curr_basis = np.array([*range(A.shape[1],A_tab.shape[1])])
        
        # Criamos o c da PL auxiliar
        c = np.zeros(A.shape[1])
        c_tab = np.concatenate([np.zeros(A.shape[1]),np.ones(A.shape[0])],axis=0,dtype=np.float64)
        
        # Passamos a PL para a forma canônica 
        curr_basis_inv = np.eye(len(curr_basis)) # since the basis is the identity, so is its inverse
        curr_c_tab = c_tab + ((-1 * c_tab[curr_basis]).T  @ A_tab).T
        
        # Operação para estabilidade numérica, i.e., garantir que os c's da base são de fato zero e não um numero muito pequeno
        curr_c_tab[curr_basis] = 0
        
        curr_A = A_tab.copy()
        curr_b = b.copy()
            
            
    curr_iteration = 0
    
    # Se todos os coeficientes do tableu forem positivos(i.e. os coeficientes da função de custo são todos negativos)
    # e o vetor b é inteiramente positivo, o simplex não entratá em loop e portanto devemos computar a inversa para obter os resultados finais
    if (c_tab >= 0).all() and not neg_flag:
        curr_basis_inv = np.linalg.inv(A_tab[:,curr_basis])
    
    # Loop principal do Simplex, que executa enquanto o vetor de coeficientes possuir algum elemento negativo
    while (np.round(curr_c_tab,round_tolerance) < 0).any():

        # Checa por possíveis valores negativos e os corrige se necessário
        possible_neg_values = np.where(np.round(curr_b,round_tolerance) < 0)[0]
        if len(possible_neg_values) > 0:
                
            curr_b[possible_neg_values] = -1 * curr_b[possible_neg_values]
            curr_A[possible_neg_values,:] = -1 * curr_A[possible_neg_values,:]

        # Encontra as colunas que correspondem a coeficientes negativos
        tgt_columns = np.where(np.round(curr_c_tab,round_tolerance) < 0)[0]
        
        # Checamos se a PL é ilimitada
        if not aux_mode:
            check_result = check_unlimited(curr_A,curr_c_tab,tgt_columns,curr_basis,round_tolerance)
            if check_result[0] == "ilimitada":
                if curr_iteration == 0:
                    curr_basis_inv = np.linalg.inv(A_tab[:,curr_basis])
                if not neg_flag:
                    viable_sol = curr_basis_inv @ b

                    final_viable_sol = np.zeros(curr_A.shape[1]).reshape(-1,1)
                    final_viable_sol[curr_basis] = viable_sol
                else:
                    final_viable_sol = viable_sol
                
                return (check_result[0],final_viable_sol[:A.shape[1]].reshape(-1),check_result[1][:A.shape[1]].reshape(-1))
                    
        # Escolhemos a primeira coluna possível para pivotear (Regra de Bland)
        column_to_pivot = tgt_columns[0]

        # Buscamos qual elemento da coluna deve ser pivoteado
        pivot_idx = 0
        prev_div = np.inf
        curr_div = 0

        for line_idx in range(A_tab.shape[0]):
            curr_element = np.round(curr_A[:,column_to_pivot][line_idx],round_tolerance)
            if curr_element > 0:
                curr_div = curr_b[line_idx]/curr_element
                if curr_div < prev_div:
                    prev_div = curr_div
                    pivot_idx = line_idx
                    
        # Adicionamos a coluna para a base na posição adequada
        curr_basis[pivot_idx] = column_to_pivot
        
        # Computamos a matriz inversa da base
        curr_basis_inv = np.linalg.inv(A_tab[:,curr_basis])

        # Atualizamos o vetor de pesos e a matriz A
        curr_c_tab = c_tab + ((-1 * c_tab[curr_basis]).T @ curr_basis_inv @ A_tab).T

        curr_A = np.round(curr_basis_inv @ A_tab,round_tolerance)
        
        # Operação de estabilidade numérica (descrita previamente)
        for i in range(len(curr_A)):
            curr_A[i,curr_basis] = 0
            curr_A[i,curr_basis[i]] = 1
        
        curr_c_tab[curr_basis] = 0
        
        # Por fim, atualizamos b
        curr_b = np.round(curr_basis_inv @ b,round_tolerance)
            
        curr_iteration += 1
          
    # Parte final do simplex
    # Calculamos o certificado de ótimo, a solução final e o valor de ótimo
    optimal_certificate = (-1 * c_tab[curr_basis]).T @ curr_basis_inv
    
    solution = curr_basis_inv @ b
    
    final_solution = np.zeros(curr_A.shape[1]).reshape(-1,1)
    final_solution[curr_basis] = solution

    optimal_value = optimal_certificate @ b
   
    # Se estivermos na PL auxiliar, retornamos a base, o certificado e o valor, caso contrário retornamos
    # uma tupla com as informações requisitadas pelo enunciado
    if aux_mode: 
        # Caso especial: se a PL é viável, mas a solução encontrada pelo simplex na PL auxiliar utiliza colunas que não existem
        # na matriz original (i.e. a PL auxiliar adiciona colunas novas, se o ótimo final usa alguma dessas colunas e o ótimo é 0,
        # significa que a PL é viável). Como a base sugerida contém colunas que não existem na PL original, devemos substituir essas
        # por colunas que existem na PL.
        
        if np.round(optimal_value,round_tolerance) == 0: # Só faz essa checagem se a PL não for inviável
            problematic_cols = list(np.where(curr_basis > (A.shape[1]-1))[0])

            if len(problematic_cols) > 0:
                possible_cols = [*range(0,A.shape[1])]
                orig_num_cols = A.shape[1] - A.shape[0]
                
                for prob_col_idx in problematic_cols: # Itera sobre cada uma das colunas problematicas(colunas artificiais)
                    prob_col = curr_basis[prob_col_idx]
                    
                    for col in possible_cols: # Itera sobre cada uma das colunas possíveis
                        # Checamos se existem outras colunas que podem ser C.L. da coluna atual, caso positivo devemos ou pular para a próxima coluna problemática ou descartar o candidato atual
                        if col >= orig_num_cols:
                            tgt_col = col + A.shape[0]
                        
                            if tgt_col in curr_basis:
                                if col == possible_cols[-1]:
                                    problematic_cols.append(prob_col_idx)
                                    break
                                else:
                                    continue
                                    
                        cond1 = col not in curr_basis # Se a coluna não estiver na base
                        cond2 = np.round(curr_A[prob_col-curr_A.shape[1],col],round_tolerance) != 0 # Se ela não possui valor nulo na matriz em estado ótimo da auxiliar
                        if cond1 and cond2:
                            curr_basis[prob_col_idx] = col
                            curr_basis_inv = np.linalg.inv(A_tab[:,curr_basis])
                            curr_A = np.round(curr_basis_inv @ A_tab,round_tolerance)
                            break
                            
        optimal_certificate[neg_b_idxs] = -1 * optimal_certificate[neg_b_idxs] # garante que o certificado esteja de acordo com o padrão das aulas
        return (curr_basis,optimal_certificate,optimal_value.item(),np.round(final_solution[:A.shape[1]],round_tolerance))
    else:
        return ("otima",optimal_value.item(),final_solution[:A.shape[1]].reshape(-1),optimal_certificate.reshape(-1))



if __name__ == "__main__":

    num_rows = 0
    Ab = []
    # Lê a entrada do stdin
    for lin_num,line in enumerate(sys.stdin):
        if lin_num == 0:
            line_content = line.split()
            num_rows = int(line_content[0])
            num_cols = int(line_content[1])
        elif lin_num == 1:
            c = np.array([int(i) for i in line.split()]).reshape(-1,1)
        else:
            Ab.append([int(i) for i in line.split()])

        if lin_num == num_rows+1:
            break
            
    Ab = np.array(Ab)
    A = Ab[:,:-1].reshape(num_rows,num_cols)
    b = Ab[:,-1].reshape(-1,1)
    # Executa o simplex
    results = simplex(A,b,c,aux_mode=False,round_tolerance=10)
    print(results[0])

    if results[0] == "otima":
        print("{0:.7f}".format(results[1]))
        np.savetxt(sys.stdout, results[2].reshape(1,-1), fmt='%.7f')
        np.savetxt(sys.stdout, results[3].reshape(1,-1), fmt='%.7f')

    elif results[0] == "ilimitada":
        np.savetxt(sys.stdout, results[1].reshape(1,-1), fmt='%.7f')
        np.savetxt(sys.stdout, results[2].reshape(1,-1), fmt='%.7f')
    elif results[0] == "inviavel":
        np.savetxt(sys.stdout, results[1].reshape(1,-1), fmt='%.7f')
