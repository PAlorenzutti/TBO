#ifndef _LISTA_H_
#define _LISTA_H_

typedef struct Lista Lista;

Lista* crivoDeEratostenes(int N);

void printaLista(Lista *lista);

void liberaLista(Lista *lista);

#endif