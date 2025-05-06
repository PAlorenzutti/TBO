#include "erastotenes.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Erastotenes{
    int *primos;
    int size;
};

// int ehPrimo(int numero){
//     for(int i = 2; i < sqrt(numero) + 1; i++){
//         if(numero % i == 0){
//             return 0;
//         } 
//     }

//     return 1;
// }

void atualizarLista(Erastotenes *lista, int i){
    for(int j = 2; j < (int)lista->size/i + 1; j++){
        lista->primos[i * j - 2] = 1;
    }
}

Erastotenes* numerosPrimos(int N){
    Erastotenes *lista = (Erastotenes*)malloc(sizeof(Erastotenes));

    lista->primos = (int*)calloc(N, sizeof(int));
    lista->size = N;

    for(int i = 2; i < sqrt(N); i++){
        atualizarLista(lista, i);
    }

    return lista;
}

void printaPrimos(Erastotenes *lista){
    for(int i = 0; i < lista->size - 1; i++){
        if(lista->primos[i] == 0){
            printf("%d ", i + 2);
        }
    }
}

void freeErastotenes(Erastotenes *lista){
    free(lista->primos);
    free(lista);
}