#include <iostream>
#include <fstream>
#include <algorithm> 

void read_data(long long int** &grid, long long int** &scores, int &num_rows, int& num_trees){
    /*
    Função que lê os dados de entrada.
    Entrada:
      long long int** &grid: 
        matriz que irá conter as disponibilidades, passada por referência
      long long int** &scores:
        matriz que será utilizada para construir um caminho ótimo, passada por referência
      int &num_rows:
        inteiro identificando o número de fileiras(F), passado por referência
      int& num_trees:
        inteiro identificando o número de macieiras por fileira(W), passado por referência
    Não retorna nenhum valor
    */
    // Obtem os valores de F e W
    std::cin >> num_rows >> num_trees;
    // Inicializa as matrizes de grid e score
    long long int **new_grid = new long long int*[num_rows];
    long long int **new_scores = new long long int*[num_rows];
    for(int i = 0; i < num_rows;i++){
        new_grid[i] = new long long int[num_trees];
        new_scores[i] = new long long int[num_trees];
    }
    // Lê a disponibilidade de cada macieira
    long long int temp;
    for(int i = 0; i < num_rows; i++){ // Loop while que lê cada linha do arquivo contendo os dados
          for(int j = 0; j < num_trees; j++){
            std::cin >> temp;
            new_grid[i][j] = new_scores[i][j] = temp;
          }
    }
    grid = new_grid;
    scores = new_scores;
}

void calculate_scores(long long int** scores, int num_rows, int num_trees){
    /*
    Função que calcula os scores para cada macieira, de acordo com a equação de bellman para a solução dinâmica do problema.
    Entrada:
      long long int** &scores:
        matriz que será utilizada para construir um caminho ótimo
      int num_rows:
        inteiro identificando o número de fileiras(F)
      int num_trees:
        inteiro identificando o número de macieiras por fileira(W)
    Não retorna nenhum valor
    */
    long long int max_score,s1,s2,s3;
    for(int i = 1; i < num_rows; i++){
        for(int j = 0; j < num_trees; j++){
            if(j == 0){
                s1 = scores[i-1][j];
                s2 = scores[i-1][j+1];
                s3 = -1;
            }
            else if(j == num_trees-1){
                s1 = scores[i-1][j];
                s2 = -1;
                s3 = scores[i-1][j-1];
            }
            else{
                s1 = scores[i-1][j];
                s2 = scores[i-1][j+1];
                s3 = scores[i-1][j-1];
            }
            max_score = std::max(s1,s2);
            max_score = std::max(max_score,s3);
            scores[i][j] += max_score;
        }
    }
}

void build_opt_path(int* &path,long long int &total_score,long long int** scores, long long int** grid, int num_rows, int num_trees){
    /*
    Função que constroi o caminho ótimo para o problema
    Entrada:
      int* &path:
        vetor que armazenará os índices de cada macieira que compõe o caminho ótimo, passado por referência
      long long int &total_score:
        inteiro que armazenará a quantidade máxima de frutas que poderão ser colhidas pela colheitadera, passado por referência
      long long int** &grid: 
        matriz que contém as disponibilidades, passada por referência
      long long int** &scores:
        matriz que contém os scores da equação de bellman, passada por referência
      int num_rows:
        inteiro identificando o número de fileiras(F)
      int num_trees:
        inteiro identificando o número de macieiras por fileira(W)
    Não retorna nenhum valor
    */
    long long int max_score,s1,s2,s3;
    int max_score_idx;
    for(int i = num_rows-1; i >= 0; i--){
        if(i == num_rows-1){
            max_score = -1;
            for(int j = 0; j < num_trees; j++){
                if(scores[i][j] > max_score){
                    max_score = scores[i][j];
                    max_score_idx = j;
                }
            }
            path[i] = max_score_idx;
        }
        else{
            if(path[i+1] == 0){
                s1 = scores[i][path[i+1]];
                s2 = scores[i][path[i+1]+1];
                s3 = -1;
            }
            else if(path[i+1] == num_trees-1){
                s1 = scores[i][path[i+1]];
                s2 = -1;
                s3 = scores[i][path[i+1]-1];
            }
            else{
                s1 = scores[i][path[i+1]];
                s2 = scores[i][path[i+1]+1];
                s3 = scores[i][path[i+1]-1];
            }
            max_score = std::max(s1,s2);
            max_score = std::max(max_score,s3);
            if(max_score == s1){
                path[i] = path[i+1];
            }
            else if(max_score == s2){
                path[i] = path[i+1] + 1;
            }
            else{
                path[i] = path[i+1] - 1;
            }
        }
        total_score += grid[i][path[i]];
    }   
}


int main(){
    
    long long int** grid; // Matriz contendo a disponibilidade para cada macieira
    long long int** scores; // Matriz que será utilizada para calcular o caminho ótimo de forma dinâmica
    int num_rows,num_trees; // Valores inteiros indicando F e W respectivamente
    // Lê os dados do arquivo de entrada
    read_data(grid,scores,num_rows,num_trees);
    // Atualiza os valores na matriz scores de acordo com a equação de bellman para o problema
    calculate_scores(scores,num_rows,num_trees);
    // Constroi a solução ótima a partir das matrizes obtidas 
    int* path = new int[num_rows];
    long long int total_score = 0;
    build_opt_path(path,total_score,scores,grid,num_rows,num_trees);
    // Imprime os resultados e deleta as variáveis alocadas dinânicamente
    std::cout << total_score << std::endl;
    for(int i = 0; i < num_rows; i++){
        std::cout << path[i] << " ";
        delete[] grid[i];
        delete[] scores[i];
    }
    delete[] grid;
    delete[] scores;
    delete[] path;
    
    return 0;
}
