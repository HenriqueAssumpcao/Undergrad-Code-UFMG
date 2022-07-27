import numpy as np

def minkowski_distance(x:np.array,y:np.array,p:int) -> float:
    """
    Função que implementa a distância de minkowski entre dois pontos x e y.
    Recebe os pontos e um parâmetro p que controla a distância.
    """
    diff_power = np.power(np.abs(x - y),p)
    return np.power(np.sum(diff_power),1/p) 

def pairwise_minkowski_distance(X:np.array,p:int) -> np.array:
    """
    Função que implementa a distância de minkowski entre os pontos em uma matriz de pontos X.
    Recebe a matriz e um parâmetro p.
    """
    diff_power = np.power(np.abs(X[:,None,:] - X[None,:,:]),p)
    return np.power(np.sum(diff_power,axis=-1),1/p)

def cluster_distance(x_i:np.array,clusters:list,p:int) -> float:
    """
    Função que calcula a distância de um ponto a um determinado cluster, definida como a menor distância dentre as distâncias do ponto para todos os pontos no cluster.
    Recebe o ponto, o cluster e o parâmetro p da distância.
    """
    cluster_distances = np.array([minkowski_distance(x_i,c_i,p) for c_i in clusters])
    return cluster_distances

def compute_cluster_radius(points:np.array,clusters:list,p:int) -> float:
    """
    Função que computa o raio máximo de um cluster.
    Recebe um conjunto de pontos, o cluster e o parâmetro p da distância.
    """
    radiuses = np.zeros(len(points))
    for idx,point in enumerate(points):
        radiuses[idx] = np.min(cluster_distance(point,clusters,p))
    return np.max(radiuses) 
        
def approx_k_clustering(points:np.array,num_clusters:int,p:int,seed:int) -> tuple:
    """
    Função que implementa o algoritmo 2-aproximado para o problema dos k-centros.
    Recebe um conjunto de pontos, o número k de clusters, o parâmetro p da distância e uma semente aleatória.
    """
    if num_clusters > len(points):
        return points,np.array([*range(len(points))])
    
    np.random.seed(seed)
    rand_start = points[np.random.randint(0,len(points))]
    clusters = [rand_start]

    # Computa os clusters
    while len(clusters) < num_clusters:
        radiuses = np.zeros(len(points))

        for idx,point in enumerate(points):
            radiuses[idx] = np.min(cluster_distance(point,clusters,p))

        max_rad_idx = np.argmax(radiuses)
        tgt_point = points[max_rad_idx,:]

        clusters.append(tgt_point)

    # Classifica os pontos
    point_classes = np.zeros(len(points))

    for idx,point in enumerate(points):
        cluster_dists = cluster_distance(point,clusters,p)
        point_classes[idx] = np.argmin(cluster_dists)
    
    return clusters,point_classes