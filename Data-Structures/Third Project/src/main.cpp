#include <iostream>
#include <string>
#include <fstream>
#include "utils.h"
#include "buffer.h"
#include "tree.h"


int main(int argc, char *argv[]){
    std::ifstream infile(argv[1]); // Lê o arquivo indicado pela linha de comando
    if(!infile){
        // Caso o arquivo não exista, encerra o programa
        std::cerr << "Error: Unable to open file!" << std::endl;
        return 0;
    }
    int num_people;
    infile >> num_people; // Lê o número de pessoas da primeira linha do arquivo
    if(num_people <= 0){
        return 0;
    }
    binary_tree<Buffer> tree;

    std::string txt; // Variável que irá armazenar todo o texto de uma dada linha do arquivo
    std::string delim = " ", token; // Variáveis auxiliares para o parsing das strings
    int pos = 0; // Variável auxiliar para o parsing das strings

    int counter = 0;
    while(std::getline(infile,txt)){ // Loop while que lê cada linha do arquivo contendo os dados
            if(txt.size() == 0){
                continue;
            }
            if(counter < num_people){ // Caso o usuário esteja inserindo os dados de um indivíduo
                pos = txt.find(delim);
                token = txt.substr(0,pos);
                txt.erase(0,pos + delim.length());
                Item<Buffer> new_item;
                new_item.key = token;
                new_item.data.push_back(std::stoi(txt));
                tree.insert(new_item);
            }
            else{ // Caso o usuário queira encontrar um indivíduo e obter a soma de seus dados
                Item<Buffer> curr_item = tree.search_and_remove(txt);
                if(curr_item.key == NOT_FOUND_FLAG){
                    continue;
                }
                int curr_value, sum=0;
                while(!curr_item.data.is_empty()){
                    curr_value = std::stoi(std::to_string(curr_item.data.pop_front()),0,2);
                    sum+=curr_value;
                }
                std::cout << curr_item.key << " " << sum << std::endl;

            }
            if(counter == num_people - 1){ // Imprime a árvore após todas as inserções
                tree.print();
                std::cout << std::endl;
            }
            counter++;
        }
    tree.print();
    infile.close();
}