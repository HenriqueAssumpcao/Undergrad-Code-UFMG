#ifndef SORTING
#define SORTING
#include <iostream>
#include <string>
#include "utils.h"
// Compare strings
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
              3: =
              4: <=
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

        case 3: //equal
        for(int i = 0; i < min_size; i++){
            if((int)s1[i] != (int)s2[i]){
                return false;
            }
        }
        if(s1.size() ==  s2.size()){
            return true;
        }
        break;

        case 4: // lesser or equal
        for(int i = 0; i < min_size; i++){
            if((int)s1[i] < (int)s2[i]){
                return true;
            }
            else if((int)s1[i] > (int)s2[i]){
                return false;
            }
        }
        if(s1.size() < s2.size() || s1.size() > s2.size()){
            return false;
        }
        return true;
        break;
    }
    return false;
};

// quicksort
void partition(int left, int right, int *i, int *j, Consc *vec){
    /*
    Função que realiza a partição de um array para o quicksort.
    Recebe:
        int left:
            Valor inteiro que indica a posição do array a ser tomada como início para a operação de partição.
        int right:
            Valor inteiro que indica a posição do array a ser tomada como final para a operação de partição.
        int *i:
            Ponteiro para inteiro que auxilia na partição.
        int *j:
            Ponteiro para inteiro que auxilia na partição.
        Consc *vec:
            Array de objetos do tipo Consc no qual será realizada a partição.
    Não retorna nenhum valor.
    */
    Consc w;
    *i = left, *j = right;
    std::string x;
    x = vec[(*i + *j)/2].name;
    do{
        while(compare_strings_alphabet(x,vec[*i].name,1)){
            (*i)++;
        }
        while(compare_strings_alphabet(x,vec[*j].name,2)){
            (*j)--;
        }
        if(*i <= *j){
            w = vec[*i];
            vec[*i] = vec[*j];
            vec[*j] = w;
            (*i)++;
            (*j)--;
        }
    }while(*i <= *j);

};
void sort(int left, int right, Consc *vec){
    /*
    Função que realiza a ordenação de um vetor de acordo com o algoritmo quicksort.
    Recebe:
        int left:
            Valor inteiro que identifica o início da porção do array a ser ordenada.
        int right:
            Valor inteiro que identifica o final da porção do array a ser ordenada.
        Consc *vec:
            Array de objetos do tipo Consc que será ordenado.
    Não retorna nenhum valor.
    */
    int i,j;
    partition(left,right,&i,&j,vec);
    if (left < j){
        sort(left,j,vec);
    }
    if(i < right){
        sort(i,right,vec);
    }
}
void quick_sort(Consc *vec, int n){
    /*
    Função que realiza o quicksort sobre todo um array de entrada. Apenas chama a função sort com os valores left = 0, right = n-1.
    Recebe:
        Consc *vec:
            Array de objetos do tipo Consc que será ordenado.
        int n:
            Valor inteiro que identifica o tamanho do array.
    Não retorna nenhum valor.
    */
    sort(0,n-1,vec);
}

// heapsort
void remake(int left, int right, Consc *vec){
    /*
    Função que confere se um determinado array segue a estrutura de um heap.
    Recebe:
        int left:
            Valor inteiro que identifica o início da porção do array a ser conferida.
        int right:
            Valor inteiro que identifica o final da porção do array a ser conferida.
        Consc *vec:
            Array de objetos do tipo Consc.
    Não retorna nenhum valor.
    */
    int i,j;
    Consc x;
    i = left;
    j = i * 2;
    x = vec[i];
    while(j <= right){
        if(j < right){
            if(vec[j].data < vec[j+1].data){
                j++;
            }
        }
        if(x.data >= vec[j].data){
            break;
        }
        vec[i] = vec[j];
        i = j;
        j = i * 2;
    }
    vec[i] = x;
}

void create_heap(Consc *vec, int n){
    /*
    Função que transforma o array em um heap válido.
    Recebe:
        Consc *vec:
            Array de objetos do tipo Consc.
        int n:
            Valor inteiro que identifica o tamanho do array.
    Não retorna nenhum valor.
    */
    int left;
    left = n/2 + 1;
    while(left > 0){
        left--;
        remake(left,n,vec);
    }
}

void heap_sort(Consc *vec, int *n){
    /*
    Função que ordena um array de acordo com o algoritmo heap sort.
    Recebe:
        Consc *vec:
            Array de objetos do tipo Consc.
        int *n:
            Ponteiro para inteiro que identifica o tamanho do array.
    Não retorna nenhum valor.
    */
    int left,right;
    Consc x;
    create_heap(vec,*n - 1);
    left = 0;
    right = (*n) - 1;
    while(right > 0){
        x = vec[0];
        vec[0] = vec[right];
        vec[right] = x;
        right--;
        remake(left,right,vec);
    }
}

// merge sort
void merge(Consc *vec, int left, int middle, int right, bool order_by_data=false){
    /*
    Função que realiza a operação de merge do algoritmo mergesort.
    Recebe:
        Consc *vec:
            Array de objetos do tipo Consc.
        int left:
            Valor inteiro que identifica o início da porção do array a ser considerada para a operação merge.
        int middle:
            Valor inteiro que identifica o meio da porção do array a ser considerada para a operação merge.
        int right:
            Valor inteiro que identifica o final da porção do array a ser considerada para a operação merge.
        bool order_by_data:
            Valor booleano que identifica com respeito a qual atributo da estrutura Consc deve ser feita a ordenação. 
            Por padrão é false, indicando que a ordenação será feita com respeito ao campo NAME.
    Não retorna nenhum valor.
    */
    int size_left = middle - left + 1;
    int size_right = right - middle; 
    Consc *left_arr = new Consc[size_left],
    *right_arr = new Consc[size_right];

    for(int i = 0; i < size_left; i++){
        left_arr[i] = vec[left + i];
    }
    for(int j = 0; j < size_right; j++){
        right_arr[j] = vec[middle + 1 + j];
    }

    int idx_left = 0, idx_right = 0, idx_merge = left;
    if(order_by_data){
        while(idx_left < size_left && idx_right < size_right){
            if(left_arr[idx_left].data <= right_arr[idx_right].data){
                vec[idx_merge] = left_arr[idx_left];
                idx_left++;
            }
            else{
                vec[idx_merge] = right_arr[idx_right];
                idx_right++;
            }
            idx_merge++;
        }
    }
    else{
        while(idx_left < size_left && idx_right < size_right){
            if(compare_strings_alphabet(left_arr[idx_left].name,right_arr[idx_right].name,4)){
                vec[idx_merge] = left_arr[idx_left];
                idx_left++;
            }
            else{
                vec[idx_merge] = right_arr[idx_right];
                idx_right++;
            }
            idx_merge++;
        }
    }
    while(idx_left < size_left){
        vec[idx_merge] = left_arr[idx_left];
        idx_merge++;
        idx_left++;
    }
    while(idx_right < size_right){
        vec[idx_merge] = right_arr[idx_right];
        idx_merge++;
        idx_right++;
    }
}

void merge_sort(Consc *vec, int begin, int end, bool order_by_data=false){
    /*
    Função que realiza o algoritmo mergesort.
    Recebe:
        Consc *vec:
            Array de objetos do tipo Consc.
        int begin:
            Valor inteiro que identifica o início do array.
        int end:
            Valor inteiro que identifica o final do array.
        bool order_by_data:
            Valor booleano que identifica com respeito a qual atributo da estrutura Consc deve ser feita a ordenação. 
            Por padrão é false, indicando que a ordenação será feita com respeito ao campo NAME.
    Não retorna nenhum valor.
    */
    if(begin >= end){
        return;
    }
    int middle = begin + (end - begin) / 2;
    merge_sort(vec,begin,middle,order_by_data);
    merge_sort(vec,middle+1,end,order_by_data);
    merge(vec,begin,middle,end,order_by_data);
}
// // sort sub groups using quick sort
// void sort_sub_groups(Consc *vec, int n){
//     /*
//     Função que ordena grupos de mesmo nome dentro de um array de acordo com seus dados binários, i.e., ordena sub-arrays com valor NAME iguais de acordo com o campo DATA.
//     Utiliza o algoritmo merge sort para a ordenação, tendo em vista sua
//     Recebe:
//         Consc *vec:
//             Array de objetos do tipo Consc.
//         int n:
//             Valor inteiro que identifica o tamanho do array.
//     Não retorna nenhum valor.
//     */
//     int j;
//     for(int i = 0; i < n; i++){
//         j = i;
//         while((j < (n-1)) && (vec[i].name == vec[j+1].name)){
//             j++;
//         }
//         merge_sort(vec,i,j,true);
//         i = j;
//     }
// }
// radix exchange sort

int max_value(Consc *vec, int n){
    /*
    Função que encontra o valor máximo do atributo DATA de um array.
    Recebe:
        Consc *vec:
            Array de objetos do tipo Consc.
        int n:
            Valor inteiro que identifica o tamanho do array.
    Não retorna nenhum valor.
    */
    int max = vec[0].data;
    for(int i = 0; i < n; i++){
        if(vec[i].data > max){
            max = vec[i].data;
        }
    }
    return max;
}

void count_sort(Consc *vec, int n, int exp){
    /*
    Função que realiza o counting sort sobre um array de objetos do tipo Consc.
    Recebe:
        Consc *vec:
            Array de objetos do tipo Consc.
        int n:
            Valor inteiro que identifica o tamanho do array.
        int exp:
            Valor inteiro que indentifica qual dígito considerar dos dados do array.
    Não retorna nenhum valor.
    */
    Consc *output =  new Consc[n];
    int  count[10] = {0};
    for(int i = 0; i < n; i++){
        count[(vec[i].data / exp) % 10]++;
    }
    
    for(int i = 1; i < 10; i++){
        count[i] += count[i - 1];
    }

    for(int i = n - 1; i >= 0; i--){
        output[count[(vec[i].data / exp) % 10] - 1] = vec[i];
        count[(vec[i].data / exp) % 10]--;
    }
    
    for(int i = 0; i < n; i++){
        vec[i] = output[i];
    }
}

void radix_exchange_sort(Consc *vec, int n){
    /*
    Função que ordena um array de objetos do tipo Consc de acordo com o algoritmo radix exchange sort.
    Chama a função count_sort para cada dígito do valor máximo encontrado no array.
    Recebe:
        Consc *vec:
            Array de objetos do tipo Consc.
        int n:
            Valor inteiro que identifica o tamanho do array.
    Não retorna nenhum valor.
    */
    int max = max_value(vec,n);
    for(int exp = 1; max / exp > 0; exp *= 10){
        count_sort(vec,n,exp);
    }
}
#endif