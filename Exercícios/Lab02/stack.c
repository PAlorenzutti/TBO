#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

struct Stack{
    Vector *v;
};

// cria uma stack
Stack *stack_construct(){
    Stack *s = (Stack*)malloc(sizeof(Stack));

    s->v = vector_construct();

    return s;
}

// insere um item na stack
void stack_push(Stack *s, data_type val){
    vector_push_back(s->v, val);
}

// remove o ultimo item inserido e o retorna
data_type stack_pop(Stack *s){
    return vector_pop_back(s->v);
}

// retorna 1 se a stack está vazia e 0 caso contrário
int stack_empty(Stack *s){
    if(!vector_size(s->v)){
        return 1;
    }else{
        return 0;
    }
}

// libera o espaço alocado para a stack
void stack_destroy(Stack *s){
    vector_destroy(s->v);
    free(s);
}
