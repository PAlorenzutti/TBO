#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Lista {
    int *numeros;    //0 indica primo, 1 indica não-primo
    int tamanho; 
};

Lista* criaLista(int N) {
    Lista *lista = (Lista*)malloc(sizeof(Lista));
    
    lista->numeros = (int*)calloc(N - 1, sizeof(int));    
    lista->tamanho = N - 1;

    return lista;
}

Lista* crivoDeEratostenes(int N) {
    Lista *lista = criaLista(N);
    
    //crivo de eratostenes;
    for (int i = 2; i < sqrt(N); i++) {
        for (int j = i * i; j <= N; j += i) {
            lista->numeros[j-2] = 1;
        }
    }
    
    return lista;
}

void printaLista(Lista *lista) {
    printf("Números primos encontrados:\n");
    for (int i = 0; i < lista->tamanho; i++) {
        if (lista->numeros[i] == 0) {
            printf("%d ", i + 2);
        }
    }
    printf("\n");
}

void liberaLista(Lista *lista) {
    if(lista != NULL){
        free(lista->numeros);
        free(lista);
    }
}