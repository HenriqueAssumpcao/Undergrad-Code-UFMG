#include <iostream>
#ifndef POINT
#define POINT


struct Point{
    /*
    Estrutura representando um Ponto em 2D.
    Atributos:
        int x: Valor da abscissa
        int y: Valor da ordenada
    */
    int x,y;
    Point(){}; // Método construtor padrão
    Point(int x,int y):x(x),y(y){}; // Método construtor
    ~Point(){}; // Método destrutor
};

int grid_distance(Point p1, Point p2){
    /*
    Função que calcula a distância entre dois pontos.
    Essa função considera a possibilidade de se locomover na diagonal e não considera possíveis colizões.
    Recebe:
      Point p1 e Point p2
    Retorna:
      Distância entre os pontos
    */
    int m_x = abs(p1.x - p2.x);
    int m_y = abs(p1.y - p2.y);
    return std::max<int>(m_x,m_y) - 1; // O -1 vem do exemplo dado na documentação, pois não se considera o movimento de se locomover para a posição onde o outro ponto está no cálculo da distância
}

#endif