#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

// Função de comparação para inteiros (min-heap)
int compare_ints(const void *a, const void *b) {
    int int_a = *(int*)a;
    int int_b = *(int*)b;

    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

// Função para "destruir" um inteiro (não faz nada, já que é um tipo primitivo)
// Mas é necessária para a interface do vector/heap.
void destroy_int(void *data) {
    // Para dados alocados dinamicamente, free(data) seria chamado aqui.
    // Como estamos usando ponteiros para inteiros que estão na stack da main,
    // não precisamos fazer nada. Se estivéssemos alocando cada int com malloc,
    // precisaríamos de um free. Para simplificar, vamos apenas desalocar o ponteiro
    // que a PQ aloca para guardar o valor.
    free(data);
}

// Função para imprimir um inteiro
void print_int(const void *data) {
    if (data) {
        printf("%d", *(int*)data);
    } else {
        printf("NULL");
    }
}

int main() {
    printf("Iniciando teste do Heap...\n");

    // Criando a fila de prioridades
    // A função destroy_int será chamada para cada elemento quando PQ_destroy for chamada.
    Heap *pq = PQ_create(destroy_int, compare_ints);

    if (!pq) {
        fprintf(stderr, "Falha ao criar a fila de prioridades.\n");
        return 1;
    }

    printf("Heap criado. Está vazio? %s. Tamanho: %d\n",
           PQ_is_empty(pq) ? "Sim" : "Não", PQ_size(pq));

    // Inserindo elementos
    printf("\nInserindo os números: 5, 3, 8, 1, 9, 4\n");
    int values[] = {5, 3, 8, 1, 9, 4};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++) {
        int *p_val = malloc(sizeof(int));
        if(!p_val) {
            fprintf(stderr, "Falha ao alocar memória para o inteiro.\n");
            PQ_destroy(pq);
            return 1;
        }
        *p_val = values[i];
        PQ_insert(pq, p_val);
        printf("Inserido %d. Tamanho atual do heap: %d\n", *p_val, PQ_size(pq));
    }

    printf("\nInserção concluída. Estado atual do heap:\n");
    PQ_print(pq, print_int);

    printf("\nInserção concluída. Está vazio? %s. Tamanho: %d\n",
           PQ_is_empty(pq) ? "Sim" : "Não", PQ_size(pq));

    // Removendo elementos em ordem de prioridade
    printf("\nRemovendo elementos do heap (devem sair em ordem crescente):\n");
    while (!PQ_is_empty(pq)) {
        int *min_val = (int*)PQ_delmin(pq);
        if (min_val) {
            printf("Menor elemento removido: %d. Tamanho restante: %d\n", *min_val, PQ_size(pq));
            if (!PQ_is_empty(pq)) {
                printf("Estado atual do heap:\n");
                PQ_print(pq, print_int);
            }
            // O dado retornado por PQ_delmin é o mesmo que foi inserido.
            // A função destroy_int cuidará de liberar a memória dele quando
            // o heap for destruído, mas como o removemos, temos que fazer isso manualmente.
            // No entanto, a nossa implementação de PQ_destroy itera sobre os elementos
            // que AINDA ESTÃO no heap. Os que foram removidos precisam ser liberados.
            free(min_val);
        }
    }

    printf("\nTodos os elementos foram removidos. Está vazio? %s. Tamanho: %d\n",
           PQ_is_empty(pq) ? "Sim" : "Não", PQ_size(pq));

    // Destruindo o heap
    printf("\nDestruindo o heap...\n");
    PQ_destroy(pq);
    printf("Heap destruído.\n");

    printf("\nTeste concluído com sucesso!\n");

    printf("\n--- Teste do Heap Select ---\n");
    Heap *pq_select = PQ_create(destroy_int, compare_ints);

    int select_values[] = {9, 5, 2, 7, 3, 8, 1, 6, 4};
    int num_select = sizeof(select_values) / sizeof(select_values[0]);

    printf("Vetor original para heap_select: ");
    for(int i = 0; i < num_select; i++) {
        int *p_val = malloc(sizeof(int));
        *p_val = select_values[i];
        PQ_insert(pq_select, p_val);
        print_int(p_val);
        if (i < num_select - 1) printf(", ");
    }
    printf("\n");
    
    printf("Heap antes do heap_select:\n");
    PQ_print(pq_select, print_int);

    int k = 4;
    printf("Executando heap_select para encontrar os %d menores elementos...\n", k);
    heap_select(pq_select, k);

    printf("Vetor interno do heap após heap_select (os %d primeiros devem ser os menores):\n", k);
    // Imprime o vetor interno para ver o resultado
    PQ_print(pq_select, print_int);

    PQ_destroy(pq_select);
    printf("--- Fim do Teste do Heap Select ---\n");


    return 0;
}
