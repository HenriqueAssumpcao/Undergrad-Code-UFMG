#ifndef UTILS
#define UTILS
#include <iostream>
#include <string>
struct Consc{
    /*
    Struct da estrutura Consc. 
    Atributos:
        int data:
            Valor inteiro que armazena os dados binários atrelados a uma consciência da mega-net.
        std::string binary_data:
            Representação dos dados binarios em string.
        std::string name:
            String que representa o nome do indivíduo em questão.     
    */
    int data;
    std::string binary_data;
    std::string name;
    Consc(std::string binary_data, std::string name):binary_data(binary_data),name(name){
    /*
        Método construtor, que armazena os dados binários em forma de string em binary_data, o nome em name, e transforma os dados binários em string para inteiro.    
    */
        data = std::stoi(binary_data,0,2);
    };
    Consc() = default; // método inicializador default.
    ~Consc(){}; // método destrutor.
    void print(){
    /*
    Função que imprime os valores armazenados em name e binary_data de uma instância de Consc.
    Não recebe nem retorna nenhum valor. 
    */
        std::cout << this->name << " " << this->binary_data << std::endl;
    }
};
#endif