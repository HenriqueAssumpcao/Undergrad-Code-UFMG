#include <iostream>
#include <list>
#include <vector>
#include "client.h"
#include "point.h"

#ifndef STORE
#define STORE
class Store{
    /*
    Estrutura representando uma Loja.
    Atributos privados:
        int id: id da loja, definido por sua posição no arquivo de entrada
        int stock: estoque da loja
        Point p: ponto representando a localização geográfica da loja
        std::list<Client> preferences: lista decrescente de preferências da loja
    */
    private:
    int id,stock;
    Point p;
    std::list<Client> preferences;
    public:
    Store(){}; // Método construtor padrão
    Store(int id,int stock,int x,int y):id(id),stock(stock),p(x,y){}; // Método construtor

    void set_preferences(std::vector<Client> sorted_clients){ 
        // Método que copia os elementos de um vetor de clientes ordenados para a lista de preferências da loja
        for(unsigned int i = 0; i < sorted_clients.size(); i++){
            this->preferences.push_back(sorted_clients[i]);
        }
    }

    Client get_next_client(){
        // Método que retorna o primeiro cliente da lista de preferências da loja e então o remove da lista
        Client curr_client = this->preferences.front();
        this->preferences.pop_front();
        return curr_client;
    }

    int get_id(){ // Método get
        return this->id;
    }
    Point get_pos(){ // Método get
        return this->p;
    }
    void decrease_stock(int val){ 
        // Método que subtrai um valor dado como entrada do estoque da loja. Entradas negativas equivalem a aumentar o estoque da loja.
        this->stock -= val;
    }
    int get_stock(){ // Método get
        return this->stock;
    }
    bool is_pref_empty(){
        // Método que retorna um valor booleano indicando se a lista de preferências da loja esta vazia.
        return this->preferences.size() == 0;
    }

    ~Store(){}; // Método destrutor
};
#endif