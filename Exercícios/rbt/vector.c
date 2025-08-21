#include "vector.h"

#include <stdio.h>
#include <stdlib.h>

#define VECTOR_INCREASE_FACTOR 2

struct vector{
  data_type *data; //void** data
  int size;
  int allocated;
  data_destroy data_destroy;
};

Vector *vector_init(data_destroy data_des, int init_size){
  Vector *v = (Vector*)calloc(1, sizeof(Vector));

  v->allocated = init_size;

  v->data = (data_type*)calloc(v->allocated, sizeof(data_type));

  v->size = 0;

  v->data_destroy = data_des;

  return v;
}

void vector_insert(Vector *v, data_type val){
  //verifica se tem espaço disponível, se não, dobra o tamanho do vetor;  
  if(v->size == v->allocated){
    v->allocated += VECTOR_INCREASE_FACTOR;

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

void vector_sort(Vector *v, int (*cmp_fn) (const void*, const void*)){
  qsort(v->data, v->size, sizeof(data_type), cmp_fn);
}

int vector_binary_search(Vector *v, data_type val, cmp_func cmp){
  
  int left = 0;
  int right = v->size - 1;

  while(left <= right) {
    int mid = left + (right - left) / 2;
    int search = cmp(val, vector_get(v, mid));

    if(search == 0) {
      return mid;
    }
    else if(search > 0) {
      left = mid + 1;
    }
    else {
      right = mid - 1;
    }
  }

  return -1;
}

void vector_print(Vector *v, data_print print_fn){
    for(int i = 0; i < v->size; i++){
        print_fn(v->data[i]);
    }
    printf("\n");
}

void vector_destroy(void* value){    
  Vector *v = (Vector*) value;
  
  // Libera cada elemento do vetor
  for(int i = 0; i < v->size; i++){
    v->data_destroy(v->data[i]);
  }
  
  free(v->data);
  free(v);
}

void vector_set(Vector* v, int idx, data_type val){

  if(idx > -1 && idx < v->size){
    
    // if(val == NULL){
    //   vector_remove();
    //   return;
    // }

    v->data[idx] = val;
  }
}

void vector_reset_size(Vector* v){
    v->size = 0;
}

void vector_push_back(Vector *v, data_type val) {
    vector_insert(v, val);
}

data_type vector_pop_back(Vector *v) {
    if (v->size == 0) {
        return NULL;
    }
    v->size--;
    return v->data[v->size];
}

data_type vector_pop_front(Vector *v) {
    if (v->size == 0) {
        return NULL;
    }
    data_type front_val = v->data[0];
    for (int i = 0; i < v->size - 1; i++) {
        v->data[i] = v->data[i + 1];
    }
    v->size--;
    return front_val;
}

// Funçãopara usar como data_destroy/print quando nada deve ser feito
void do_nothing(void * a){
    //não faz nada
}