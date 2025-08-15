#ifndef _ED_VECTOR_H_
#define _ED_VECTOR_H_

typedef void *data_type;

typedef void (*data_print)(const void *);

typedef void (*data_destroy)(void *);

typedef int (*cmp_fn)(const void *, const void *);

typedef struct vector Vector;

// Aloca espaço para um elemento do tipo vector e inicializa os seus atributos.
Vector *vector_init(data_destroy destroy_fn, int init_size);

// Adiciona o item dado ao final do vector e aumenta o tamanho. Aumenta a capacidade alocada se necessário.
void vector_push_back(Vector *v, data_type val);

// Remove o i-ésimo elemento do vetor.
data_type vector_remove(Vector *v, int i);

// Remove o primeiro elemento
data_type vector_pop_front(Vector *v);

// Remove o ultimo elemento
data_type vector_pop_back(Vector *v);

// Retorna o número de elementos atualmente no vector.
int vector_size(Vector *v);

// Retorna o i-ésimo elemento do vector.
data_type vector_get(Vector *v, int i);

// Ordena o vetor in-place (sem criar um novo vetor)
void vector_sort(Vector *v, cmp_fn cmp_fn);

// Retorna o indice de val usando busca binaria. Retorna -1 se nao encontrado.
int vector_binary_search(Vector *v, data_type val, cmp_fn cmp_fn);

// Printa todos elementos do vetor (depuração)
void vector_print(Vector *v, data_print print_fn);

// Retorna o ponteiro para o array interno de dados do vector
data_type *vector_data(Vector *v);

// Libera o espaço reservado para o vector.
void vector_destroy(void *value);

#endif