#ifndef UTILS
#define UTILS
#include <iostream>
#include <string>
#include "buffer.h"
bool compare_strings_alphabet(const std::string &s1, const std::string &s2, int compare_mode){
    /*
    Função que compara duas strings de acordo com a ordem alfabética. 
    Recebe:
        const std::string &s1: 
            Primeira string a ser comparada.
        const std::string &s2: 
            Segunda string a ser comparada.
        int compare_mode:
            Valor inteiro que identifica qual operação deverá ser feita. Valores possíveis são:
              1: >
              2: <
    Retorna:
        Valor booleano representando o resultado a operação realizada, ou seja, retorna true se a operação é verdade e false caso contrário.
        
    */
    int min_size;
    if(s1.size() > s2.size()){
        min_size = s2.size();
    }
    else{
        min_size = s1.size();
    }
    switch(compare_mode){
        case 1: // greater
        for(int i = 0; i < min_size; i++){
            if((int)s1[i] > (int)s2[i]){
                return true;
            }
            else if((int)s1[i] < (int)s2[i]){
                return false;
            }
        }
        if(s1.size() > s2.size()){
            return true;
        }
        return false;
        break;

        case 2: //smaller
        for(int i = 0; i < min_size; i++){
            if((int)s1[i] < (int)s2[i]){
                return true;
            }
            else if((int)s1[i] > (int)s2[i]){
                return false;
            }
        }
        if(s1.size() < s2.size()){
            return true;
        }
        return false;
        break;
    }
    return false;
};
const std::string UNINIT_FLAG = "-1"; // Chave padrão para inicialização de objetos do tipo Item
template <class T>
struct Item{
    /*
    Struct da estrutura Item.
    Atributos:
      T data:
        Atributo de tipo T que representa os dados armazenados por um item
      std::string key:
        String que representa a chave identificadora de um item
        
    */
    T data;
    std::string key;
    Item(std::string key):key(key){};
    Item():key(UNINIT_FLAG){};
    void copy(Item<T> &it); // Método vazio para cópia de valores data. Deve ser especificado para cada tipo
};
template<>
void Item<Buffer>::copy(Item<Buffer> &it){
    /*
    Especificação do método copy para o tipo Buffer. Chama o método copy_content da classe Buffer
    Recebe:
      Item<Buffer> &it:
        Objeto de tipo Item<Buffer> que representa um segundo buffer do qual se quer copiar o conteúdo
    Não retorna nenhum valor
    */
    this->key = it.key;
    this->data.copy_content(it.data);
}

#endif