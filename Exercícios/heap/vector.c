#include "vector.h"

#include <stdio.h>
#include <stdlib.h>

#define VECTOR_INCREASE_FACTOR 2

struct vector
{
  data_type *data; // void** data
  int size;
  int allocated;
  data_destroy data_destroy;
};

Vector *vector_init(data_destroy data_des, int init_size)
{
  Vector *v = (Vector *)calloc(1, sizeof(Vector));

  v->allocated = init_size;

  v->data = (data_type *)calloc(v->allocated, sizeof(data_type));

  v->size = 0;

  v->data_destroy = data_des;

  return v;
}

void vector_push_back(Vector *v, data_type val)
{
  // verifica se tem espaço disponível, se não, dobra o tamanho do vetor;
  if (v->size == v->allocated)
  {
    v->allocated += VECTOR_INCREASE_FACTOR;

    v->data = (data_type *)realloc(v->data, v->allocated * sizeof(data_type));
  }

  v->data[v->size] = val;
  v->size++;
}

void vector_insert(Vector *v, data_type val, int i)
{
  // verifica se tem espaço disponível, se não, dobra o tamanho do vetor;
  if (v->size == v->allocated)
  {
    v->allocated += VECTOR_INCREASE_FACTOR;

    v->data = (data_type *)realloc(v->data, v->allocated * sizeof(data_type));
  }

  v->data[i] = val;
  v->size;
}

int vector_compare(Vector *v, int i, int j, cmp_fn cmp_fn)
{
  return cmp_fn(v->data[i], v->data[j]);
}

void vector_swap(Vector *v, int i, int j)
{
  data_type temp  = v->data[j];
  v->data[j] = v->data[i];
  v->data[i] = temp;
}

data_type vector_remove(Vector *v, int i)
{
  data_type data = v->data[i];

  // começa o loop pelo elemento a ser retirado;
  for (int j = i; j < v->size - 1; j++)
  {
    v->data[j] = v->data[j + 1];
  }

  // diminui o tamanho do vetor;
  v->size--;

  return data;
}

data_type vector_pop_front(Vector *v)
{
  return vector_remove(v, 0);
}

data_type vector_pop_back(Vector *v)
{
  return vector_remove(v, v->size - 1);
}

int vector_size(Vector *v)
{
  return v->size;
}

data_type vector_get(Vector *v, int i)
{
  if (!(i < v->size) || !(i > -1))
  {
    printf("Erro!\n");
    exit(1);
  }

  return v->data[i];
}

void vector_sort(Vector *v, cmp_fn cmp_fn)
{
  qsort(v->data, v->size, sizeof(data_type), cmp_fn);
}

int vector_binary_search(Vector *v, data_type val, cmp_fn cmp_fn)
{
  int idx_max = v->size - 1;
  int idx_min = 0;

  while (idx_max >= idx_min)
  {
    int idx_mid = (idx_max + idx_min) / 2;
    data_type mid = vector_get(v, idx_mid);
    int cmp = cmp_fn(val, &mid);
    if (cmp > 0)
    {
      idx_min = idx_mid + 1;
    }
    else if (cmp < 0)
    {
      idx_max = idx_mid - 1;
    }
    else
    {
      return idx_mid;
    }
  }
  return -1;
}

void vector_print(Vector *v, data_print print_fn)
{
  for (int i = 0; i < v->size; i++)
  {
    print_fn(v->data[i]);

    if (i != v->size - 1)
    {
      printf(", ");
    }
  }
}

data_type *vector_data(Vector *v)
{
  return v->data;
}

void vector_destroy(void *value)
{
  Vector *v = (Vector *)value;

  // Libera cada elemento do vetor
  for (int i = 0; i < v->size; i++)
  {
    v->data_destroy(v->data[i]);
  }

  free(v->data);
  free(v);
}
