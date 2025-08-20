#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

// Protótipos das funções auxiliares para o tipo char
int char_cmp(const void *key1, const void *key2);
void char_destroy(void *key);
void char_print(const void *key);

// Protótipos das funções de teste
void test_rank(BinaryTree *bt);
void test_floor_and_ceiling(BinaryTree *bt);

int main() {
    // Inicializa a árvore com as funções de comparação e destruição para caracteres
    BinaryTree *bt = bt_init(char_destroy, NULL, char_cmp);

    printf("--- Inserindo chaves para recriar a arvore corretamente ---\n");
    // Ordem de inserção CORRETA com 'S' como raiz
    char keys[] = {'S', 'E', 'X', 'A', 'R', 'C', 'H', 'M'};
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++) {
        char *ptr = malloc(sizeof(char));
        *ptr = keys[i];
        bt_put(bt, ptr, NULL);
    }

    printf("Arvore inicial (com 'S' como raiz):\n");
    bt_print(bt, char_print);
    printf("Tamanho da arvore: %d\n\n", bt_size(bt));

    // --- Seção de Testes ---
    test_rank(bt);
    test_floor_and_ceiling(bt);

    // --- Teste delmin ---
    printf("\n--- Testando bt_delmin ---\n");
    printf("Arvore antes de deletar o minimo ('A'):\n");
    bt_print(bt, char_print);
    bt_delmin(bt);
    printf("Arvore depois de deletar o minimo:\n");
    bt_print(bt, char_print);
    printf("\n");

    // --- Teste delmax ---
    printf("--- Testando bt_delmax ---\n");
    printf("Arvore antes de deletar o maximo ('X'):\n");
    bt_print(bt, char_print);
    bt_delmax(bt);
    printf("Arvore depois de deletar o maximo:\n");
    bt_print(bt, char_print);
    printf("\n");
    
    printf("Tamanho final da arvore: %d\n", bt_size(bt));

    // Libera toda a memória alocada para a árvore
    bt_destroy(bt);
    
    return 0;
}

// Funções auxiliares (sem alteração)
int char_cmp(const void *key1, const void *key2) {
    const char *c1 = key1;
    const char *c2 = key2;
    return *c1 - *c2;
}

void char_destroy(void *key) {
    if (key) {
        free(key);
    }
}

void char_print(const void *key) {
    if (key) {
        const char *v = key;
        printf("%c", *v);
    } else {
        printf("NULL");
    }
}

/**
 * @brief Testa a função bt_rank com a estrutura de árvore correta.
 */
void test_rank(BinaryTree *bt) {
    printf("--- Testando bt_rank (com 'S' como raiz) ---\n");
    // Chaves na árvore, em ordem: A, C, E, H, M, R, S, X
    
    char test_key_1 = 'R';
    // Chaves menores que 'R': {'A', 'C', 'E', 'H', 'M'}. Esperado: 5
    printf("Rank da chave '%c': %d (Esperado: 5)\n", test_key_1, bt_rank(bt, &test_key_1));
    
    char test_key_2 = 'S';
    // Chaves menores que 'S': {'A', 'C', 'E', 'H', 'M', 'R'}. Esperado: 6
    printf("Rank da chave '%c': %d (Esperado: 6)\n", test_key_2, bt_rank(bt, &test_key_2));
    
    char test_key_3 = 'A';
    // Nenhuma chave menor que 'A'. Esperado: 0
    printf("Rank da chave '%c': %d (Esperado: 0)\n", test_key_3, bt_rank(bt, &test_key_3));
    
    char test_key_4 = 'Z'; 
    // Todas as 8 chaves são menores que 'Z'. Esperado: 8
    printf("Rank da chave '%c': %d (Esperado: 8)\n", test_key_4, bt_rank(bt, &test_key_4));
}

/**
 * @brief Testa as funções bt_floor e bt_ceiling com a estrutura de árvore correta.
 */
void test_floor_and_ceiling(BinaryTree *bt) {
    printf("\n--- Testando bt_floor e bt_ceiling (com 'S' como raiz) ---\n");

    char test_keys[] = {'G', 'D', 'Q', 'A'};
    int n_tests = sizeof(test_keys) / sizeof(test_keys[0]);

    for (int i = 0; i < n_tests; i++) {
        char key = test_keys[i];
        printf("Chave de teste: '%c'\n", key);

        // Testando floor
        Node *floor_node = bt_floor(bt, &key);
        if (floor_node) {
            char *floor_key = (char *)bt_node_get_key(floor_node);
            printf("  Floor: '%c'\n", *floor_key);
        } else {
            printf("  Floor: (null)\n");
        }

        // Testando ceiling
        Node *ceiling_node = bt_ceiling(bt, &key);
        if (ceiling_node) {
            char *ceiling_key = (char *)bt_node_get_key(ceiling_node);
            printf("  Ceiling: '%c'\n", *ceiling_key);
        } else {
            printf("  Ceiling: (null)\n");
        }
    }
    /*
    Resultados esperados para as chaves {A, C, E, H, M, R, S, X}:
    - Chave 'G': Floor: 'E', Ceiling: 'H'
    - Chave 'T': Floor: 'S', Ceiling: 'X'
    - Chave '@': Floor: (null), Ceiling: 'A'
    - Chave 'A': Floor: 'A', Ceiling: 'A'
    */
}