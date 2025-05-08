#include "vector.h"

#include <stdio.h>
#include <stdlib.h>

#define VECTOR_INIT_SIZE 10
#define VECTOR_INCREASE_FACTOR 2

struct vector{
  data_type *data; //void** data
  int size;
  int allocated;
};

Vector *vector_construct(){
  Vector *v = (Vector*)calloc(1, sizeof(Vector));

  v->allocated = VECTOR_INIT_SIZE;

  v->data = (data_type*)calloc(v->allocated, sizeof(data_type));

  v->size = 0;

  return v;
}

void vector_push_back(Vector *v, data_type val){
  //verifica se tem espaço disponível, se não, dobra o tamanho do vetor;  
  if(v->size == v->allocated){
    v->allocated *= VECTOR_INCREASE_FACTOR;

    v->data = (data_type*)realloc(v->data, v->allocated * sizeof(data_type));
  }


  v->data[v->size] = val;
  v->size++;
}

int vector_size(Vector *v){
  return v->size;
}

data_type vector_get(Vector *v, int i){
  if(!(i < v->size) || !(i > -1)){
    printf("Erro!\n");
    exit(1);
  }

  return v->data[i];
}

void vector_set(Vector *v, int i, data_type val){
  if(!(i < v->size) || !(i > -1)){
    printf("Erro ao atribuir!\n");
    exit(1);
  }

  v->data[i] = val;
}

int vector_find(Vector *v, data_type val){
  for(int i = 0; i < v->size; i++){
    if(v->data[i] == val){
      return i;
    }
  }

  return -1;
}

data_type vector_max(Vector *v){
  //assume o primeiro como maior;
  data_type max = v->data[0];

  for(int i = 0; i < v->size; i++){
    if(v->data[i] > max){
      max = v->data[i];
    }
  }

  return max;
}

data_type vector_min(Vector *v){
  //assume o primeiro como menor;
  data_type min = v->data[0];

  for(int i = 0; i < v->size; i++){
    if(v->data[i] < min){
      min = v->data[i];
    }
  }

  return min;
}

int vector_argmax(Vector *v){
  data_type max = vector_max(v);

  return vector_find(v, max);
}

int vector_argmin(Vector *v){
  data_type min = vector_min(v);

  return vector_find(v, min);
}

data_type vector_remove(Vector *v, int i){
  data_type data = v->data[i];

  //começa o loop pelo elemento a ser retirado;
  for(int j = i; j < v->size - 1; j++){
    v->data[j] = v->data[j + 1];
  }

  //diminui o tamanho do vetor;
  v->size--;

  return data;
}

data_type vector_pop_front(Vector *v){
  return vector_remove(v, 0);
}

data_type vector_pop_back(Vector *v){
  return vector_remove(v, v->size - 1);
}

void vector_insert(Vector *v, int i, data_type val){
  //se o tamanho for igual ao alocado, dobra o tamanho do vetor;
  if(v->size == v->allocated){
    v->allocated *= 2;

    v->data = (data_type*)realloc(v->data, v->allocated * sizeof(data_type));
  }

  //começa da posição mais a direita e vai deslocando os itens anteriores;
  for(int j = v->size; j > i; j--){
    v->data[j] = v->data[j - 1];
  }

  //insere o valor na posição, agora livre;
  v->data[i] = val;
  v->size++;
}

void vector_swap(Vector *v, int i, int j){
  data_type temp;

  temp = v->data[i];
  v->data[i] = v->data[j];
  v->data[j] = temp;
}

void vector_sort(Vector *v, int (*cmp_fn) (const void*, const void*)){
  for(int i = 0; i < v->size - 1; i++){
    int swap = 0;

    for(int j = 0; j < v->size - i - 1; j++){
      //se o anterior for maior que o próximo, troca;
      if (cmp_fn(v->data[j], v->data[j + 1]) > 0){
        vector_swap(v, j, j + 1);
        swap = 1;
      }
    }

    //verifica se houve troca;
    if(swap == 0){
      break;
    }
  }
}

int vector_binary_search(Vector *v, data_type val){
  int idx_max = v->size - 1;
  int idx_min = 0;

  while(idx_max >= idx_min){
    //calcula o índice do elemento central do intervalo;
    int idx_mid = (idx_max + idx_min) / 2;

    //pega o índice do elemento central do intervalo;
    data_type mid = vector_get(v, idx_mid);

    if(val > mid){
      //atualiza o idx_min;
      idx_min = idx_mid + 1;

    }

    if(val < mid){
      //atualiza o idx_max;
      idx_max = idx_mid - 1;

    }

    //se o valor não é menor, nem maior, significa que ele é igual.
    if(val == mid){
      return idx_mid;
    }
  }

  return -1;
}

void vector_reverse(Vector *v){
  for(int i = 0; i < v->size - 1; i++){
    for(int j = 0; j < v->size - i - 1; j++){
      vector_swap(v, j, j + 1);
    }
  }
}

void vector_destroy(Vector *v){  
  for(int i = 0; i < v->size; i++){
    free(v->data[i]);
  }
  
  free(v->data);
  free(v);
}
