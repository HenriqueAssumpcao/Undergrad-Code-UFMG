#include <iostream>
#include "point.h"
#ifndef STORE
#define STORE
class Store{
    /*
    Estrutura que representa uma loja.
    Atributos:
      int id: 
        id identificador
      Point geo_loc:
        Ponto identificando a geolocalização da loja
    */
    private:
    int id;
    Point geo_loc;
    public:
    Store(){}; // método construtor padrão
    Store(int id, Point p):id(id),geo_loc(p){}; // método construtor
    // métodos get e set
    void set_id(int id){ 
        this->id = id;
    }
    int get_id(){
        return this->id;
    }
    void set_geo_loc(Point p){
        this->geo_loc = p;
    }
    Point get_geo_loc(){
        return this->geo_loc;
    }
    ~Store(){}; // método destrutor padrão
};

#endif