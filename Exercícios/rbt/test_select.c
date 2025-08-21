#include <stdio.h>
#include <stdlib.h>
#include "rbt.h"

// Funções auxiliares para o teste
int cmp_int(const void *a, const void *b) {
    int int_a = *(int*)a;
    int int_b = *(int*)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

void key_print_int(const void *key) {
    printf("%d", *(int*)key);
}

void val_print_int(const void *val) {
    printf("[%d]", *(int*)val);
}

void destroy_int(void *a) {
    free(a);
}

int main() {
    RBT *rbt = rbt_init(cmp_int, destroy_int, destroy_int);

    printf("Inserindo elementos na árvore...\n");
    int keys[] = {10, 20, 30, 40, 50, 60, 70, 5, 15, 25};
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++) {
        int *key = malloc(sizeof(int));
        int *val = malloc(sizeof(int));
        *key = keys[i];
        *val = keys[i] * 10;
        rbt_insert(rbt, key, val);
    }

    printf("Árvore em ordem: \n");
    rbt_print(rbt, key_print_int, val_print_int);
    printf("\n\n");

    printf("Tamanho da árvore: %d\n\n", rbt_size(rbt));

    printf("--- Testando rbt_select ---\n");
    // As chaves em ordem são: 5, 10, 15, 20, 25, 30, 40, 50, 60, 70
    for (int k = 1; k <= rbt_size(rbt); k++) {
        Key key = rbt_select(rbt, k);
        if (key != NULL) {
            printf("O %d-ésimo menor elemento é: %d\n", k, *(int*)key);
        } else {
            printf("Não foi possível encontrar o %d-ésimo menor elemento.\n", k);
        }
    }
    
    printf("\n--- Teste com k inválido ---\n");
    Key key_zero = rbt_select(rbt, 0);
     if (key_zero == NULL) {
        printf("Teste com k = 0 (inválido) passou. Retornou NULL como esperado.\n");
    } else {
        printf("Teste com k = 0 (inválido) falhou. Deveria retornar NULL.\n");
    }

    Key key_too_large = rbt_select(rbt, rbt_size(rbt) + 1);
     if (key_too_large == NULL) {
        printf("Teste com k = %d (inválido) passou. Retornou NULL como esperado.\n", rbt_size(rbt) + 1);
    } else {
        printf("Teste com k = %d (inválido) falhou. Deveria retornar NULL.\n", rbt_size(rbt) + 1);
    }


    rbt_destroy(rbt);


    return 0;
}
