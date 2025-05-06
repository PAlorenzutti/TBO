#include <stdio.h>
#include "erastotenes.h"

int main(){
    int N;

    scanf("%d", &N);

    Erastotenes* lista = numerosPrimos(N);

    printaPrimos(lista);

    freeErastotenes(lista);

    return 0;
}