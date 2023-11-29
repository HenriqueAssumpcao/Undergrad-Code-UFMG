/****************************************************************************
 * spend_time.h - declaração da função de marcação de tempo 
 *         para o primeiro exercício de programação da disciplina 
 *         Fundamentos de Sistemas Paralelos e Distribuídos, 2023-2
 ****************************************************************************/

/****************************************************************************
 * void spend_time(int tid, char* info, int time_ds): para 
 * permitir simular a operação de threads que executem tarefas computacionais
 * mais complexas, que gastem um tempo significativo para completar.
 * Os parâmetros fornecidos por cada thread devem ser: 
 * - sua identificação, tid,
 * - um string de informação, info (p.ex., qual a tarefa sendo executada),
 * - quanto tempo essa tarefa deve durar, time_ds (em décimos de segundos). 
 **************************************************************************/

void spend_time(int tid, char* info, int time_ds);
