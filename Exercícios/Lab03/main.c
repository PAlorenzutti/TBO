#include "item.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_itens(Item *itens, int N){
    for(int i = 0; i < N; i++){
        printf("%d\n", (int)itens[i]);
    }
}

void leitura_arquivo(Item *itens, int N, char *path){
    FILE *file = fopen(path, "r");

    char *linha = NULL;
    size_t tamanho = 0;

    for(int i = 0; i < N; i++){
        getline(&linha, &tamanho, file);

        itens[i] = atoi(linha);
    }

    free(linha);

    fclose(file);
}

//bubble sort
extern void bubble_sort(Item *a, int lo, int hi){
    for(int i = 0; i < hi; i++){
        int swap = 0;
        
        for(int j = i + 1; j < hi; j++){
            if(less(a[j], a[i])){
                exch(a[j], a[i]);

                swap = 1;
            }
        }

        if(!swap){
            break;
        }
    }
}

//selection sort
extern void selection_sort(Item *a, int lo, int hi){
    for(int i = 0; i < hi; i++){
        int menor = i;
        
        for(int j = i + 1; j < hi; j++){
            if(less(a[j], a[menor])){
                menor = j;
            }
        }

        if(a[menor] != a[i]){
            exch(a[menor], a[i]);
        }
    }
}

int main(int argc, char *argv[]){
    Item *itens = (Item*)calloc(atoi(argv[1]), sizeof(Item));

    leitura_arquivo(itens, atoi(argv[1]), argv[2]);

    clock_t begin = clock();

    selection_sort(itens, 0, atoi(argv[1]));

    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    print_itens(itens, atoi(argv[1]));

    printf("%lf", time_spent);

    free(itens);
}