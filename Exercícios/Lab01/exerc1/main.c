#include <stdio.h>
#include "lista.h"

int main(){
    int N;

    scanf("%d", &N);

    Lista* lista = crivoDeEratostenes(N);

    printaLista(lista);

    liberaLista(lista);

    return 0;
}