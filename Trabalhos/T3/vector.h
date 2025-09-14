#ifndef _ED_VECTOR_H_
#define _ED_VECTOR_H_

typedef void* data_type;

typedef void (*data_print)(const void *);

typedef void (*data_destroy)(void *);

typedef struct vector Vector;

// Aloca espaço para um elemento do tipo vector e inicializa os seus atributos.
Vector *vector_init(data_destroy destroy_fn, int init_size);

// Adiciona o item dado ao final do vector e aumenta o tamanho. Aumenta a capacidade alocada se necessário.
void vector_insert(Vector *v, data_type val);

// Retorna o número de elementos atualmente no vector.
int vector_size(Vector *v);

// Retorna o i-ésimo elemento do vector.
data_type vector_get(Vector *v, int i);

// Ordena o vetor in-place (sem criar um novo vetor)
void vector_sort(Vector *v, int (*cmp_fn) (const void*, const void*));

// Retorna o indice de val usando busca binaria. Retorna -1 se nao encontrado.
int vector_binary_search(Vector *v, data_type val);

// Printa todos elementos do vetor (depuração)
void vector_print(Vector *v, data_print print_fn);

// Função de depuração
void vector_print_str(const void *v);

// Libera o espaço reservado para o vector.
void vector_destroy(void* value);

#endif