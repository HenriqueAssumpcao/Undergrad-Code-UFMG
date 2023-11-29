#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#include "point.h"
#include "store.h"
#include "utils.h"

int main(int argc, char *argv[]){
    // declação de variáveis e leitura dos dados de entrada
    int num_stores, km_limit_bike, num_drones, km_cost_bike, km_cost_truck;
    std::vector<Store> stores = read_data(argv,num_stores, km_limit_bike, num_drones, km_cost_bike, km_cost_truck);
    double** adj_matrix = new double*[num_stores]; // matriz de adjacências
    bool* is_visited = new bool[num_stores]; // vetor que irá informar se um dado nó já foi visitado
    int* mst_structure = new int[num_stores]; // vetor que irá armazenas a estrutura da MST(o valor na i-ésima posição indica qual nó esta conectado com o nó i)
    double* weights = new double[num_stores]; // vetor de pesos por nó

    // inicialização da matriz de adjacências
    for(unsigned int i = 0; i < stores.size(); i++){
        adj_matrix[i] = new double[num_stores];
        for(unsigned int j = 0; j < stores.size(); j++){
            if(j == i){
                adj_matrix[i][j] = 0;
            }
            else{
                adj_matrix[i][j] = euclidean_distance(stores[i].get_geo_loc(),stores[j].get_geo_loc());
            }
        }
        is_visited[i] = false;
        weights[i] = DBL_MAX;
    }
    // Escolha de um nó aleatório
    srand (time(NULL));
    int random_node = rand() % num_stores,curr_node;
    weights[random_node] = 0; // peso zero para o nó escolhido

    // Algoritmo de Prim
    for(int i = 0; i < num_stores - 1; i++){
        curr_node = min_weight_node(weights,is_visited,num_stores);
        is_visited[curr_node] = true;
        for(int j = 0; j < num_stores; j++){
            if((adj_matrix[curr_node][j] > 0) && (!is_visited[j]) && (adj_matrix[curr_node][j] < weights[j])){
                mst_structure[j] = curr_node;
                weights[j] = adj_matrix[curr_node][j];
            }
        }
    }
 
    // armazena os pesos das arestas da MST em um vetor, e então o ordena de forma crescente
    double count = 0;
    std::vector<double> mst_edge_weights;
    for (int i = 0; i < num_stores; i++){
        if (i != random_node){
            count += adj_matrix[i][mst_structure[i]];
            mst_edge_weights.push_back(adj_matrix[i][mst_structure[i]]);
        }
    }
    std::sort(mst_edge_weights.begin(), mst_edge_weights.end());

    // cálculo dos valores de interesse, i.e. c_c e c_m
    double bike_sum = 0, truck_sum = 0;
    long unsigned int edges_to_consider = mst_edge_weights.size() - num_drones;
    for(long unsigned int i = 0; i <= edges_to_consider; i++){
        if(mst_edge_weights[i] <= km_limit_bike){
            bike_sum += mst_edge_weights[i] * km_cost_bike;
        }
        else{
            truck_sum += mst_edge_weights[i] * km_cost_truck;
        }
    }

    // impressão dos resultados
    std::cout << std::fixed;
    std::cout << std::setprecision(3);
    std::cout << bike_sum << " " << truck_sum;

    // processos de desalocação da memória
    for(int i = 0; i < num_stores; i++){
        delete[] adj_matrix[i];
    }
    delete[] adj_matrix;
    delete[] is_visited;
    delete[] mst_structure;
    delete[] weights;
}
