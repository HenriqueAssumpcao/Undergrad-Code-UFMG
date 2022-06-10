#include <iostream>
#include <math.h>
#ifndef POINT
#define POINT

struct Point{
    /*
    Estrutura representando um Ponto em 2D.
    Atributos:
        long int x: Valor da abscissa
        long int y: Valor da ordenada
    */
    long int x,y;
    Point(){}; // Método construtor padrão
    Point(long int x,long int y):x(x),y(y){}; // Método construtor
    ~Point(){}; // Método destrutor
};

double euclidean_distance(Point p1, Point p2){
    /*
    Função que calcula a distância euclidiana entre dois pontos.
    Entrada:
      Point p1,p2:
        Dois objetos do tipo ponto cuja distância deve ser calculada
    Saída:
      double sqrt(result):
        Retorna a distância euclidiana entre os pontos
    */
    long int x_tilde = p1.x - p2.x, y_tilde = p1.y - p2.y;
    long int result = pow(x_tilde,2) + pow(y_tilde,2);
    return sqrt(result);
}

#endif