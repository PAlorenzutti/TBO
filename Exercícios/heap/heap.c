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

static void PQ_fix_down(Heap *pq, int k) {
    int size = vector_size(pq->v) - 1;
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
    if (PQ_is_empty(pq)) {
        return NULL;
    }
    void *min = vector_get(pq->v, 1);
    int last_idx = vector_size(pq->v) - 1;
    vector_swap(pq->v, 1, last_idx);
    void* old_last = vector_pop_back(pq->v);
    // O dado removido pelo pop_back não deve ser destruído, pois é o 'min' que será retornado.
    // A função vector_destroy do vector não será chamada para este elemento.
    // No entanto, a implementação de vector_pop_back não destroi o dado, apenas o retorna.
    // E vector_destroy itera sobre os elementos restantes.
    // O elemento retornado por vector_get não é uma cópia, é o ponteiro.
    // O dado em 'min' e 'old_last' é o mesmo.
    (void)old_last;

    PQ_fix_down(pq, 1);

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

// Funções auxiliares para o heap_select que operam diretamente no Vector
static void _fix_up(Vector *v, int k, cmp_fn cmp) {
    while (k > 1 && cmp(vector_get(v, k/2), vector_get(v, k)) > 0) {
        vector_swap(v, k, k/2);
        k = k/2;
    }
}

static void _fix_down(Vector *v, int size, int k, cmp_fn cmp) {
    while (2 * k <= size) {
        int j = 2 * k;
        if (j < size && cmp(vector_get(v, j), vector_get(v, j + 1)) > 0) {
            j++;
        }
        if (cmp(vector_get(v, k), vector_get(v, j)) <= 0) {
            break;
        }
        vector_swap(v, k, j);
        k = j;
    }
}


/*
 * Usa o heap para ordenar parcialmente o vetor v, colocando os k menores
 * elementos nas k primeiras posições.
 * Esta função assume que o vetor 'v' é 1-indexado para a lógica do heap.
 * Se o seu vetor for 0-indexado, você precisará de um wrapper ou de
 * modificar a lógica para lidar com os índices.
 */
void heap_select(Vector *v, int k, cmp_fn cmp_fn) {
    int n = vector_size(v) - 1; // Assumindo 1-indexado

    if (k < 1 || k > n) {
        // k deve ser um valor válido
        return;
    }

    // 1. Constrói o heap (heapify)
    // Começa da metade do vetor e vai subindo, garantindo que todos os sub-árvores
    // sejam heaps.
    for (int i = n / 2; i >= 1; i--) {
        _fix_down(v, n, i, cmp_fn);
    }

    // 2. Extrai os k menores elementos
    // O menor elemento está sempre na raiz (índice 1).
    // Trocamos com o último elemento, diminuímos o tamanho considerado do heap
    // e corrigimos a propriedade do heap a partir da raiz.
    int size = n;
    for (int j = 0; j < k; j++) {
        vector_swap(v, 1, size);
        _fix_down(v, --size, 1, cmp_fn);
    }

    // 3. Move os k menores elementos para o início do vetor
    // Após o passo anterior, os k menores elementos estão nas últimas k posições
    // do vetor, mas em ordem decrescente.
    // Esta parte move eles para o início do vetor.
    for (int z = 0; z < k; z++) {
        vector_swap(v, z + 1, n - z);
    }
}
