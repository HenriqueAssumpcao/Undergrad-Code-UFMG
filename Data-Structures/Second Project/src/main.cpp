#include <iostream>
#include <string>
#include <fstream>
#include "utils.h"
#include "sorting.h"

int main(int argc, char *argv[]){
    std::ifstream infile(argv[1]); // Lê o arquivo indicado pela linha de comando
    if(!infile){
        // Caso o arquivo não exista, encerra o programa
        std::cerr << "Error: Unable to open file!" << std::endl;
        return 0;
    }

    unsigned int sort_mode = atoi(argv[2]); // Identifica qual configuração será utilizada pelo programa
    int  num_lines = atoi(argv[3]); // Identifica o número de linhas a serem lidas do arquivo com os dados
    if(sort_mode > 4 || sort_mode == 0){ // Identifica se a configuração é valida, caso contrário encerra o programa
        std::cout << "Error: Sort mode must be between 1 and 4!"; 
        return 0;
    }
    
    Consc* inputs = new Consc[num_lines]; 
    std::string delim = " ", token, txt;
    int pos = 0,counter = 0;
    while(std::getline(infile,txt) && counter < num_lines){ // Lê o arquivo e armazena os dados no array inputs
        pos = txt.find(delim);
        token = txt.substr(0,pos);
        txt.erase(0,pos + delim.length());
        inputs[counter] = Consc(txt,token);
        counter++;
    }
    infile.close();

    switch(sort_mode){ // Expressão switch que identifica a configuração e executa as respectivas operações de ordenação
        case 1:
        heap_sort(inputs,&num_lines);
        quick_sort(inputs,num_lines);
        // função sort_sub_groups deveria ir aqui!
        for(int i = 0; i < num_lines; i++){
            inputs[i].print();
        }
        break;

        case 2:
        radix_exchange_sort(inputs,num_lines);
        quick_sort(inputs,num_lines);
        // função sort_sub_groups deveria ir aqui!
        for(int i = 0; i < num_lines; i++){
            inputs[i].print();
        }
        break;

        case 3:
        heap_sort(inputs,&num_lines);
        merge_sort(inputs,0,num_lines - 1);
        for(int i = 0; i < num_lines; i++){
            inputs[i].print();
        }
        break;

        case 4: 
        radix_exchange_sort(inputs,num_lines);
        merge_sort(inputs,0,num_lines - 1);
        for(int i = 0; i < num_lines; i++){
            inputs[i].print();
        }
        break;
    }
    delete[] inputs;
}