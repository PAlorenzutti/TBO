#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include "vector.h"

typedef struct Heap Heap;

/*
 * Cria uma nova fila de prioridade mínima.
 */
Heap* PQ_create(data_destroy destroy_fn, cmp_fn cmp_fn);

/*
 * Libera a memória da fila.
 */
void PQ_destroy(Heap *pq);

/*
 * Insere o dado na fila.
 */
void PQ_insert(Heap *pq, void *data);

/*
 * Remove e retorna o menor elemento.
 */
void *PQ_delmin(Heap *pq);

/*
 * Testa se a fila está vazia.
 */
bool PQ_is_empty(Heap *pq);

/*
 * Retorna o tamanho da fila.
 */
int PQ_size(Heap *pq);

/*
 * Imprime a fila.
 */
void PQ_print(Heap *pq, data_print print_fn);

/*
 * Usa o heap para ordenar parcialmente o vetor v, colocando os k menores
 * elementos nas k primeiras posições.
 */
void heap_select(Heap *pq, int k);


#endif
