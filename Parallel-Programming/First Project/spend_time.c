/****************************************************************************
 * spend_time.c - implementação da função de marcação de tempo 
 *      (e funções auxiliares) para o primeiro exercício de programação
 *      da disciplina Fundamentos de Sistemas Paralelos e Distribuídos, 2023-1
 ****************************************************************************/
#include "spend_time.h" // protótipo de spend_time_ms

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long int start_ms = 0; // horário de início do programa, para controle do tempo

/****************************************************************************
 * void check_start(void): para simplificar o uso do código,
 * verifica se o horário de início da execução já foi registrado
 * (o valor de start_ms teria esse tempo em milisegundos).
 * Se start_ms é zero, o horário de início é obtido usando clock_gettime.
 **************************************************************************/
void check_start(void)
{
    if (start_ms == 0) {
        struct timespec now;
        clock_gettime(CLOCK_REALTIME,&now);
        start_ms = (1000 * now.tv_sec) + (0.000001 * now.tv_nsec);
    }
}

/****************************************************************************
 * void write_log(int tid, char* msg): recebe como parâmetros 
 * o identificador da thread e o string da mensgem de log. 
 * Escreve na saída essa informação acrescida de um valor de timestamp
 * em décimos de segundos decorridos desde o início da execução.
 * 
 * ESSE FORMATO NÃO DEVE SER ALTERADO, POIS SERÁ USADO NA CORREÇÃO AUTOMÁTICA
 **************************************************************************/
void write_log(int tid, char* msg)
{
    struct timespec now;
    long   now_ms;
    long   time_passed_ds;
    char   log_msg[1024];

    check_start();
    clock_gettime(CLOCK_REALTIME,&now);
    now_ms = (1000.0 * now.tv_sec) + (0.000001 * now.tv_nsec);
    time_passed_ds = round( ((double)(now_ms-start_ms)) / 100.0 );
    if (snprintf(log_msg,sizeof(log_msg),
                "%3ld:%d:%s",time_passed_ds,tid,msg)<0) {
        perror("snprintf");
        exit(1);
    }
    printf("%s\n",log_msg);
}

/****************************************************************************
 * void spend_time_ms(int tid, char* info, int time_ms): para 
 * permitir simular a operação de threads que executem tarefas computacionais
 * mais complexas, essa função suspende uma thread por um espaço de tempo 
 * determinado (em milissegundos). Para gerar uma mensagem de log no início
 * e no final desse intervalo, recebe como parâmetros também o identificador 
 * da thread, e uma string que vai identificar o intervalo de tempo.
 * OBS: se info é NULL, escreve apenas um "E" no retorno.
 **************************************************************************/
void spend_time(int tid, char* info, int time_ds)
{
    struct timespec zzz;
    char msg[1024], *fmt;

    zzz.tv_sec  = time_ds/10;
    zzz.tv_nsec = (time_ds%10) * 100L * 1000L * 1000L;


    if (info) {
        if (snprintf(msg,sizeof(msg),"%s [%d",info,time_ds)<0) {
            perror("snprintf");
            exit(1);
        } 
        write_log(tid,msg);
    }

    nanosleep(&zzz,NULL);

    fmt = (info)? "%s ]" : "E";
    if (snprintf(msg,sizeof(msg),fmt,info)<0) {
        perror("snprintf");
        exit(1);
    }
    
    write_log(tid,msg);
}

/* Teste: a função main a seguir executa três chamadas consecutivas de
 * spend_time para verificar se o comportamento é o esperado.
 * Para testar, basta descomentar a função main, executar "make spend_time"
 * e depois executar o programa resultante.
 */

// int main(int argc, char* argv[])
// {
//     spend_time(1,"A",10);
//     spend_time(2,"B",0);
//     spend_time(3,NULL,13);
//     spend_time(4,"D",17);
// }


/* Saída esperada do programa de teste:
 *	
 *
  0:1:A [10
 10:1:A ]
 10:2:B [0
 10:2:B ]
 23:3:E
 23:4:D [17
 40:4:D ]
 *
 * O programa deve terminar 4 segundos após ser disparado.
 */
