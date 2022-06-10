#include <iostream>
#include <stdlib.h>
#include <map>
#include "point.h"

#ifndef CLIENT
#define CLIENT
std::map<std::string, int> state2score = {{"MG",0},{"PR",10},{"SP",20},{"SC",30},{"RJ",40},{"RN",50},{"RS",60}};
std::map<std::string, int> pay2score = {{"DINHEIRO",1},{"DEBITO",2},{"CREDITO",3}}; 
const int UNMATCHED_CLIENT = -1; // Valor para identificar um cliente que ainda não foi pareado a uma loja
class Client{
    /*
    Classe representando um Cliente.
    Atributos privados:
        int id: id do cliente, definido por sua posição no arquivo de entrada
        int age: idade do cliente
        string state: UF do cliente
        string pay_method: método de pagamento do cliente
        Point p: ponto representando a localização geográfica do cliente
        float ticket: valor do ticket do cliente
    */
    private:
    int id,age;
    std::string state,pay_method;
    Point p;
    float ticket;
    public:
    Client(){}; // Método construtor padrão
    Client(int id, int age,std::string state,std::string pay_method, int x, int y):id(id),age(age),state(state),pay_method(pay_method),p(x,y){
        /*
        Método construtor, que também faz o cálculo do valor do ticket do cliente.
        */
        this->ticket = ((float)(abs(60-this->age) + state2score[this->state]))/pay2score[this->pay_method];
    };
    float get_ticket() const{ // Método get
        return this->ticket;
    }
    Point get_pos() const{ // Método get
        return this->p;
    }
    int get_id() const{ // Método get
        return this->id;
    }

    ~Client(){};  
};
bool client_bigger(Client &c1,Client &c2){
    /*
    Método que define a operação de > para um par de clientes. Segue a seguinte lógica:
        1. Caso um cliente possua um ticket menor, ele terá uma preferência menor.
        2. Caso os tickets sejam iguais, o cliente com menor id terá preferência
    Ou seja, c1 > c2 se ticket de c1 > ticket de c2 ou se id de c1 < id de c2.
    */
    if(c1.get_ticket() == c2.get_ticket()){
        return c1.get_id() < c2.get_id();
    }
    else{
        return c1.get_ticket() > c2.get_ticket();
    }
}

#endif