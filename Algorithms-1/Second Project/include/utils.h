#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <float.h>
#include "Store.h"
#ifndef UTILS
#define UTILS
std::vector<Store> read_data(char *argv[],int &num_stores,int &km_limit_bike,int &num_drones,int &km_cost_bike,int &km_cost_truck){
    /*
    Função que lê os dados de entrada.
    Entrada:
      char *argv[]:
        argumentos da linha de comando
      int &num_stores:
        variável que irá armazenar o número de lojas
      int &km_limit_bike
        variável que irá armazenar o limite de km percorridos por motocicletas
      int &num_drones
        variável que irá armazenar o número de drones
      int &km_cost_bike
        variável que irá armazenar o custo por kilometro de transporte por motocicletas
      int &km_cost_truck
        variável que irá armazenar o número de lojas
    Saída:
      std::vector<Store> Stores:
        vetor contendo objetos de tipo Store, com as informações de cada loja
    */
    std::ifstream infile(argv[1]); // Lê o arquivo indicado pela linha de comando
    if(!infile){
        // Caso o arquivo não exista, encerra o programa
        std::cerr << "Error: Unable to open file!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string txt; // Variável que irá armazenar todo o texto de uma dada linha do arquivo
    std::string delim = " ", token; // Variáveis auxiliares para o parsing das strings
    int pos = 0; // Variável auxiliar para o parsing das strings
    int counter = 0,store_id_counter = 0; // Contador geral do loop e contador específico para o id dos clientes

    int curr_x,curr_y;
    std::vector<Store> stores;
    while(std::getline(infile,txt)){ // Loop while que lê cada linha do arquivo contendo os dados
        if(counter == 0){ // Lê as variáveis informadas na primeira linha
            pos = txt.find(delim);
            token = txt.substr(0, pos);
            num_stores = std::stoi(token);
            txt.erase(0, pos + delim.length());

            pos = txt.find(delim);
            token = txt.substr(0, pos);
            km_limit_bike = std::stoi(token);
            txt.erase(0, pos + delim.length());

            pos = txt.find(delim);
            token = txt.substr(0, pos);
            num_drones = std::stoi(token);
            txt.erase(0, pos + delim.length());

            pos = txt.find(delim);
            token = txt.substr(0, pos);
            km_cost_bike = std::stoi(token);
            txt.erase(0, pos + delim.length());

            pos = txt.find(delim);
            token = txt.substr(0, pos);
            km_cost_truck = std::stoi(token);
            txt.erase(0, pos + delim.length());
        }
        else if(counter > 0 && store_id_counter < num_stores){ // Lê as informações de cada loja
            
            pos = txt.find(delim);
            token = txt.substr(0, pos);
            curr_x = std::stoi(token);
            txt.erase(0, pos + delim.length());

            pos = txt.find(delim);
            token = txt.substr(0, pos);
            curr_y = std::stoi(token);
            txt.erase(0, pos + delim.length());

            stores.push_back(Store(store_id_counter,Point(curr_x,curr_y)));
            store_id_counter++;
        }

        counter++;
    }
    infile.close();
    return stores;
}


int min_weight_node(double weights[], bool is_mst[], int num_stores){
    /*
    Função que calcula o índice do nó com menor peso.
    Entrada:
      double* weights:
        array de pesos dos nós
      bool* is_visted:
        array informando quais nós já foram visitados
      int num_stores:
        valor inteiro indicando o número de lojas
    Saída:
      int min_idx:
        índice do nó com menor peso
    */
    double min_weight = DBL_MAX;
    int min_idx;

    for(int i = 0; i < num_stores; i++){
        if((is_mst[i] == false) && (weights[i] < min_weight)){
            min_weight = weights[i];
            min_idx = i;
        }
    }
    return min_idx;
}

#endif 