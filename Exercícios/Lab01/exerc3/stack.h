#ifndef _STACK_H_
#define _STACK_H_

#include "vector.h"

#define TAM_MAX_DATA 32

typedef struct Stack Stack;

// cria uma stack
Stack *stack_construct();

// insere um item na stack
void stack_push(Stack *s, data_type val);

// remove o ultimo item inserido e o retorna
data_type stack_pop(Stack *s);

// retorna 1 se a stack está vazia e 0 caso contrário
int stack_empty(Stack *s);

int stack_size(Stack *s);

// libera o espaço alocado para a stack
void stack_destroy(Stack *s);

#endif