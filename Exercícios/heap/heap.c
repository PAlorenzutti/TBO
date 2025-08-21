#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

struct Heap {
    Vector *v;
    cmp_fn cmp_fn;
};

/*
 * Cria uma nova fila de prioridade mínima.
 */
Heap* PQ_create(data_destroy destroy_fn, cmp_fn cmp_fn) {
    Heap *pq = (Heap*)malloc(sizeof(Heap));
    if (!pq) {
        return NULL;
    }

    // O heap será 1-indexado, então o vetor precisa de um elemento dummy no índice 0.
    pq->v = vector_init(destroy_fn, 1);
    vector_push_back(pq->v, NULL); // Elemento dummy no índice 0.
    pq->cmp_fn = cmp_fn;

    return pq;
}

/*
 * Libera a memória da fila.
 */
void PQ_destroy(Heap *pq) {
    if (pq) {
        vector_destroy(pq->v);
        free(pq);
    }
}

static void PQ_fix_up(Heap *pq, int k) {
    while (k > 1 && vector_compare(pq->v, k/2, k, pq->cmp_fn) > 0) {
        vector_swap(pq->v, k, k/2);
        k = k/2;
    }
}

/*
 * Insere o dado na fila.
 */
void PQ_insert(Heap *pq, void *data) {
    vector_push_back(pq->v, data);
    PQ_fix_up(pq, vector_size(pq->v) - 1);
}

static void PQ_fix_down(Heap *pq, int size, int k) {
    while (2 * k <= size) {
        int j = 2 * k;
        if (j < size && vector_compare(pq->v, j, j + 1, pq->cmp_fn) > 0) {
            j++;
        }
        if (vector_compare(pq->v, k, j, pq->cmp_fn) <= 0) {
            break;
        }
        vector_swap(pq->v, k, j);
        k = j;
    }
}

/*
 * Remove e retorna o menor elemento.
 */
void* PQ_delmin(Heap *pq) {
    if(PQ_is_empty(pq)){
        return NULL;
    }

    // Índice do último elemento válido (desconsiderando o dummy)
    // Coloca o último elemento na raiz
    vector_swap(pq->v, 1, vector_size(pq->v) - 1);

    // Remove o antigo último elemento (que era o mínimo)
    data_type min = vector_pop_back(pq->v);

    // Restaura a propriedade de heap a partir da raiz com o novo tamanho
    PQ_fix_down(pq, vector_size(pq->v) - 1, 1);

    return min;
}

/*
 * Testa se a fila está vazia.
 */
bool PQ_is_empty(Heap *pq) {
    return (vector_size(pq->v) - 1) == 0;
}

/*
 * Retorna o tamanho da fila.
 */
int PQ_size(Heap *pq) {
    return vector_size(pq->v) - 1;
}

/*
 * Imprime a fila.
 */
void PQ_print(Heap *pq, data_print print_fn) {
    printf("Heap: [");
    // Começa do 1 para ignorar o elemento dummy
    for (int i = 1; i < vector_size(pq->v); i++) {
        print_fn(vector_get(pq->v, i));
        if (i < vector_size(pq->v) - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

/*
 * Usa o heap para ordenar parcialmente o vetor v, colocando os k menores
 * elementos nas k primeiras posições.
 * Esta função assume que o vetor 'v' é 1-indexado para a lógica do heap.
 * Se o seu vetor for 0-indexado, você precisará de um wrapper ou de
 * modificar a lógica para lidar com os índices.
 */
void heap_select(Heap *pq, int k) {
    int n = vector_size(pq->v) - 1; // Assumindo 1-indexado

    if (k < 1 || k > n) {
        // k deve ser um valor válido
        return;
    }

    // 1. Constrói o heap (heapify)
    // Começa da metade do vetor e vai subindo, garantindo que todos os sub-árvores
    // sejam heaps.
    for (int i = 1; i <= n; i++) {
        PQ_fix_up(pq, i);
    }

    // 2. Extrai os k menores elementos
    // O menor elemento está sempre na raiz (índice 1).
    // Trocamos com o último elemento, diminuímos o tamanho considerado do heap
    // e corrigimos a propriedade do heap a partir da raiz.
    int size = n;

    for (int j = 0; j < k; j++) {
        vector_swap(pq->v, 1, size);

        PQ_fix_down(pq, --size, 1);
    }

    // 3. Move os k menores elementos para o início do vetor
    // Após o passo anterior, os k menores elementos estão nas últimas k posições
    // do vetor, mas em ordem decrescente.
    // Esta parte move eles para o início do vetor.
    for (int z = 0; z < k; z++) {
        vector_swap(pq->v, z + 1, n - z);
    }
}
