#include "queue.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>

struct Queue{
    Vector *v;
};

// cria uma queue
Queue *queue_construct(){
    Queue *queue = (Queue*)malloc(sizeof(Queue));

    queue->v = vector_construct();

    return queue;
}

// insere um item na queue 
void queue_enqueue(Queue *queue, data_type value){
    vector_push_back(queue->v, value);
}

// remove o elemento mais antigo da pilha e o retorna
data_type queue_dequeue(Queue *queue){
    return vector_pop_front(queue->v); // Remove o item da fila
}

// retorna 1 se a queue está vazia e 0 caso contrário
int queue_empty(Queue *queue){
    if(!vector_size(queue->v)){
        return 1;
    }else{
        return 0;
    }
}

// libera o espaco alocado para a queue
void queue_destroy(Queue *queue){
    vector_destroy(queue->v); // Libera o vetor
    free(queue); // Libera a fila
}