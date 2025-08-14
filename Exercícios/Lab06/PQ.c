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
    Event **events;
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

    pq->allocated = max_N + 1;
    pq->events = (Event**)malloc(sizeof(Event*) * pq->allocated);
    for (int i = 0; i < pq->allocated; i++) {
        pq->events[i] = NULL;
    }
    pq->size = 0;

    return pq;
}

/*
 * Libera a memória da fila.
 */
void PQ_destroy(PQ *pq) {
    // TODO: Implemente essa função que libera toda a memória da fila,
    //       destruindo inclusive os eventos que estavam na fila.
    for(int i = 1; i <= pq->size; i++){
        destroy_event(pq->events[i]);
    }

    free(pq->events);
    free(pq);
}


#define greater(A, B)   (compare(A, B) > 0)
#define exch(A, B)      { Event *t = A; A = B; B = t; }

static void PQ_fix_up(PQ *pq, int k) {
    Event **a = pq->events;
    while (k > 1 && greater(a[k/2], a[k])) {
        exch(a[k], a[k/2]);
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
    //incrementa pq->size antes de inserir, pois, por exemplo, pq->size começa com 0;
    pq->events[++pq->size] = e;

    //conserta do último para cima, no caso a partir de pq->size;
    PQ_fix_up(pq, pq->size);
}

static void PQ_fix_down(PQ *pq, int k) {
    Event **a = pq->events;
    while (2 * k <= pq->size) {
        int j = 2 * k;
        if (j < pq->size && greater(a[j], a[j+1])) {
            j++;
        }
        if (!greater(a[k], a[j])) {
            break;
        }
        exch(a[k], a[j]);
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
    Event *min = pq->events[1];

    //troca o menor elemento com o último do array usando a macro exch;
    exch(pq->events[1], pq->events[pq->size]);

    //diminui o tamanho do vetor;
    pq->size--;

    //fix_down a partir do primeiro elemento do vetor;
    PQ_fix_down(pq, 1);

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
    return pq->size;
}
