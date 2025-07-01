#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "PQ.h"

// TODO: Aqui você deve implementar uma fila com prioridade mínima para
//       ordenar os eventos por tempo, isto é, o evento com o menor tempo tem
//       a maior prioridade. Veja as funções definidas em 'event.h' para
//       a manipulação das estruturas de evento. A princípio, as funções já
//       existentes em 'event.h' são suficientes para implementar a simulação,
//       isto é, você não precisa implementar mais nada lá.
//
//       Você é livre para implementar a fila com prioridade da forma que quiser
//       mas é recomendado usar um min-heap para obter um bom desempenho. As
//       simulações maiores geram uma quantidade grande de eventos: um
//       limite superior frouxo (mas seguro) para o número máximo simultâneo de
//       eventos é N^3, aonde N é o número de partículas.

// TODO: Crie a struct pq.
struct pq{
    Event **elements;
    int size;
    int allocated;
};

/*
 * Cria uma nova fila de prioridade mínima com o limite de elementos informado.
 */
PQ* PQ_create(int max_N) {
    // TODO: Implemente a criação da fila que suporta no máximo o número de
    //       de eventos informados no parâmetro.
    PQ *pq = (PQ*)malloc(sizeof(PQ));

    pq->elements = (Event**)malloc(sizeof(Event*) * max_N + 1);
    pq->allocated = max_N + 1;
    pq->size = 0;

    return pq;
}

/*
 * Libera a memória da fila.
 */
void PQ_destroy(PQ *pq) {
    // TODO: Implemente essa função que libera toda a memória da fila,
    //       destruindo inclusive os eventos que estavam na fila.
    if(pq->size >= 0){
        for(int i = 1; i <= pq->size; i++){
            destroy_event(pq->elements[i]);
        }
    }

    free(pq->elements);

    free(pq);
}

void PQ_swap_elements(PQ *pq, int i, int j){
    //troca os dois elementos;
    Event *temp = pq->elements[i];
    pq->elements[i] = pq->elements[j];
    pq->elements[j] = temp;
}

void PQ_fix_up(PQ *pq){
    int k = pq->size;
    
    //enquanto o tamanho for maior do que 1;
    //enquanto o pai for maior do que o filho;
    while(k > 1 && get_time(pq->elements[k]) < get_time(pq->elements[k/2])){
        //se o filho for menor do que o pai, troca os dois;
        PQ_swap_elements(pq, k, k/2);

        //troca o índice pro índice do novo pai (o que era menor antes);
        k = k/2;
    }
}

/*
 * Insere o evento na fila segundo o seu tempo.
 */
void PQ_insert(PQ *pq, Event *e) {
    // TODO: Implemente essa função que insere o evento dado na fila segundo
    //       o tempo do evento.
    //       Assuma que 'e' não é nulo. É importante testar overflow (inserção
    //       em uma fila que já contém o número máximo de eventos) para evitar
    //       dores de cabeça com acessos inválidos na memória.

    //insere elemento ao final do vetor;
    pq->elements[++pq->size] = e;

    //conserta do último para cima;
    PQ_fix_up(pq);
}

void PQ_fix_down(PQ *pq, int k){
    while(2 * k <= pq->size){
        int j = 2 * k;

        if(j < pq->size && get_time(pq->elements[j + 1]) < get_time(pq->elements[j])){
            j++;
        }

        if(get_time(pq->elements[k]) < get_time(pq->elements[j])){
            break;
        }

        PQ_swap_elements(pq, k, j);

        k = j;
    }
}

/*
 * Remove e retorna o evento mais próximo.
 */
Event* PQ_delmin(PQ *pq) {
    // TODO: Implemente essa função que remove o evento com o menor tempo da
    //       fila e o retorna.

    //pega o menor evento de todos;
    Event *min = pq->elements[0];

    //troca o menor elemento com o último do array;
    PQ_swap_elements(pq, 0, pq->size - 1);

    //diminui o tamanho do vetor;
    pq->size--;

    //fix_down a partir do primeiro elemento do vetor;
    PQ_fix_down(pq, 0);

    return min;
}

/*
 * Testa se a fila está vazia.
 */
bool PQ_is_empty(PQ *pq) {
    // TODO: Implemente essa função.
    return pq->size == 0;
}

/*
 * Retorna o tamanho da fila.
 */
int PQ_size(PQ *pq) {
    // TODO: Implemente essa função.
}
