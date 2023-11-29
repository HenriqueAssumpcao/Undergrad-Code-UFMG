/*
TRABALHO PRÁTICO 1 - FUNDAMENTOS DE SISTEMAS PARALELOS E DISTRIBUÍDOS (UFMG - 2023/2)
ALUNO: HENRIQUE SOARES ASSUMPÇÃO E SILVA (CIÊNCIA DA COMPUTAÇÃO)
MATRÍCULA: 2020006620
*/

#include "spend_time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

// CONSTANTES E VARIÁVEIS GLOBAIS
#define MAX_THREADS 1000 // Número máximo de threads dado no enunciado
#define NUM_RECURSOS 8 // Número total de recursos dado no enunciado
#define BUF_SZ 512 // Tamanho do buffer utilizado para ler do stdin
#define EMPTY_ID -1 // Valor que identifica se uma posição do array de recursos está vazia

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Variável global mutex que controla o travamento e a liberação de recursos
pthread_cond_t cond_trava = PTHREAD_COND_INITIALIZER; // Variável global de condição que é utilizada em conjunto com a mutex

// STRUCT RECURSOS
typedef struct recurso{
    /* 
    Definição da struct que armazena as informações de um recurso:
    . unsigned short is_free: indica se o recurso está livre
    */
    unsigned short is_free;
}recurso;

recurso recursos[NUM_RECURSOS]; // Variável Global de recursos

void init_recursos(){
    /* 
    Função que inicializa os valores de um objeto do tipo thread_args.
    Funcionamento: inicializa o id da thread e o vetor de ids de recursos com o valore EMPTY_ID, os tempos com o valor 0.
    */
    size_t i;
    for(i = 0; i < NUM_RECURSOS; i++){
        recursos[i].is_free = 1;
    }
}

// STRUCT ARGUMENTOS DE THREAD
typedef struct thread_args{
    /* 
    Definição da struct que armazena os argumentos da thread. Possui os seguintes atributos:
    . unsigned short tid: id da thread
    . size_t f_time: tempo livre em décimos de segundos
    . size_t c_time: tempo crítico em décimos de segundos
    . unsigned short recursos_ids[NUM_RECURSOS]: vetor que armazena os ids dos recursos requistados pela thread
    */
    unsigned short tid;
    size_t f_time,c_time;
    short recursos_ids[NUM_RECURSOS];

}thread_args;
void init_targs(thread_args *targs){
    /* 
    Função que inicializa os valores de um objeto do tipo thread_args.
    Funcionamento: inicializa o id da thread e o vetor de ids de recursos com o valor EMPTY_ID, e os tempos com o valor 0.
    */
    size_t i;
    targs->tid = EMPTY_ID;
    targs->f_time = 0;
    targs->c_time = 0;
    for(i = 0;i < NUM_RECURSOS; i++){
        targs->recursos_ids[i] = EMPTY_ID;
    }
}
__thread thread_args *targs; // Variável global dos argumentos de thread que é local a nivel de thread, i.e., as threads não compartilham essa variável entre si.

// OPERAÇÕES PRINCIPAIS
int checa_disponibilidade(short *recursos_ids){
    /* 
    Função que checa se TODOS os recursos requisitados por uma thread estão disponíveis.
    Recebe o vetor de ids de recursos.
    Retorna -1 se algum recurso está ocupado, e 0 caso todos os recursos requisitados estejam disponíveis.
    Funcionamento: itera sob o vetor de ids de recursos da thread até encontrar algum recurso ocupado, e nesse caso retorna -1. Caso todos os recursos
    requisitados estejam livres, retorna 0.
    */
    size_t i = 0;
    while(recursos_ids[i] != EMPTY_ID && i < NUM_RECURSOS){
        if(recursos[recursos_ids[i]].is_free == 0){
            return -1;
        }
        i++;
    }
    return 0;
}
void trava_recursos(short *recursos_ids){
    /* 
    Função que trava os recursos requisitados por uma thread.
    Recebe um vetor com os ids dos recursos requisitados.
    Não retorna nada.
    Funcionamento: Primeiro a thread da lock na variável global mutex, e então entra em um loop while em que cada iteração checa se todos os
    recursos requisitados pela thread estão disponíveis. Caso não estejam, a thread chama um wait na variável condicional cond_trava. Caso estejam,
    a thread prossegue para travar os recursos, i.e., alterar os atributos dos objetos de tipo recurso na variável global de recursos, e então
    da unlock na mutex.
    Note que esse comportamente garante exclusão mútua: Se duas threads requisitam o mesmo recurso, a primeira que dar lock na mutex necessáriamente
    conseguirá tal recurso, devido ao loop while de checagem de disponibilidade com a chamada de wait caso os recursos estejam ocupados.
    Ademais, essa solução também evita espera ocupada: a thread tenta travar os recursos, e caso não consiga ela da wait e espera até que alguém a acorde.
    */
    pthread_mutex_lock(&mutex);
    while(checa_disponibilidade(recursos_ids) != 0){
        pthread_cond_wait(&cond_trava,&mutex);
    }
    size_t i = 0;
    while(recursos_ids[i] != EMPTY_ID && i < NUM_RECURSOS){
        recursos[recursos_ids[i]].is_free = 0;
        i++;
    }
    pthread_mutex_unlock(&mutex);
}
void libera_recursos(){
    /* 
    Função que libera os recursos sendo utilizados por uma thread.
    Não recebe nem retorna nada.
    Funcionamento: da lock na mutex, altera os atributos dos objetos de tipo recurso da variável global de recursos para sinalizar
    que os recursos estão livres. Após isso, chama um broadcast na variável condiconal cond_trava, isto é, a thread acorda todas as threads
    que estavam esperando os recursos serem liberados, e ao fim da unlock na mutex.
    Note que não necessariamente uma thread que é acordada por esse broadcast vai ter seus recursos livres, pois a thread que chamou o broadcast pode não
    ser a thread que está ocupando os recursos, mas dessa forma garantimos que as threads não irão esperar indefinidamente, isto é, toda thread que libera
    um recurso avisa as outras.
    */
    pthread_mutex_lock(&mutex);
    size_t i = 0;
    while(targs->recursos_ids[i] != EMPTY_ID && i < NUM_RECURSOS){
        recursos[targs->recursos_ids[i]].is_free = 1;
        i++;
    }
    pthread_cond_broadcast(&cond_trava);
    pthread_mutex_unlock(&mutex);
}
void *run_thread(void *args){
    /* 
    Função executada por uma thread.
    Recebe um ponteiro do tipo void para os argumentos da thread.
    Funcionamento: Primeiro executa o tempo livre da thread, então tenta travar os recursos requisitados. Após conseguir travar, executa o tempo
    crítico da thread, e então libera os recursos e finaliza a thread.
    */
    targs = (thread_args*)args;

    spend_time(targs->tid,NULL,targs->f_time);
    trava_recursos(targs->recursos_ids);
    spend_time(targs->tid,"C",targs->c_time);
    libera_recursos();

    pthread_exit(NULL);
}

// MAIN
int main(){
    /* 
    Função main.
    Funcionamento: inicializa os recursos, e então entra em um loop que lê inputs do stdin até encontrar um EOF, e, após processar o input,
    cria a respectiva thread. Ao final do loop, da join em todas as threads criadas, e então libera a variável mutex e a variável de condição.
    */

    pthread_t threads_arr[MAX_THREADS]; // Vetor de threads
    thread_args targs_arr[MAX_THREADS]; // Vetor de argumentos para as threads
    
    init_recursos();

    char buffer[BUF_SZ] = "",*token;
    size_t t_counter,arg_counter,rec_counter;
    while(fgets(buffer,BUF_SZ,stdin) != NULL){
        init_targs(&targs_arr[t_counter]);

        token = strtok(buffer," ");
        arg_counter = 0; // Contador de argumentos em cada linha
        rec_counter = 0; // Contador para indexar o vetor de requisitos de cada thread
        while( token != NULL ) {
            if(arg_counter == 0){
                targs_arr[t_counter].tid = (unsigned short)atoi(token); // Id da thread
            }
            else if(arg_counter == 1){
                targs_arr[t_counter].f_time = (size_t)atoi(token); // Tempo livre
            }
            else if(arg_counter == 2){
                targs_arr[t_counter].c_time = (size_t)atoi(token); // Tempo crítico
            }
            else{
                targs_arr[t_counter].recursos_ids[rec_counter] = (unsigned short)atoi(token); // Ids de recursos
                rec_counter++;
            }
            token = strtok(NULL, " ");
            arg_counter++;
        }
        pthread_create(&threads_arr[t_counter],NULL,run_thread,(void*)&targs_arr[t_counter]);
        t_counter++;
    }

    size_t i;
    for(i = 0; i < t_counter; i++){
        pthread_join(threads_arr[i],NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_trava);

    return 0;
}