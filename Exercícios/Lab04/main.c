#include "item.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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
        bool swap = false;
        
        for(int j = i + 1; j < hi; j++){
            if(less(a[j], a[i])){
                exch(a[j], a[i]);

                swap = true;
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

//insertion sort
extern void insertion_sort(Item *a, int lo, int hi) {
    for (int i = lo + 1; i < hi; i++) {
        for (int k = i; k > lo && less(a[k], a[k - 1]); k--) {
            exch(a[k], a[k - 1]);  // Função de troca definida em item.h
        }
    }
}

//shaker sort
extern void shaker_sort(Item *a, int lo, int hi){
    for(int i = 0; i < hi/2; i++){
        bool swap = false;

        for(int j = i; j < hi - i - 1; j++){
            if(less(a[j + 1], a[j])){
                exch(a[j], a[j + 1]);
            }

            swap = true;
        }

        for(int j = hi - i - 2; j > i; j--){
            if(less(a[j], a[j - 1])){
                exch(a[j], a[j - 1]);
            }

            swap = true;
        }

        if(!swap){
            break;
        }
    }
}

void merge(Item *a, Item *aux, int lo, int mid, int hi){
    for(int k = lo; k <= hi; k++){
        //copia o array para o auxiliar (temporário);
        aux[k] = a[k];
    }

    //seta o índice da primeira (i) e segunda (j) parte do vetor;
    int i = lo, j = mid + 1;

    for(int k = lo; k <= hi; k++){
        if(i > mid){
            //caso já tenha puxado todos os índices da primeira parte do vetor, adiciona a "a" apenas a segunda parte e incrementa a segunda parte;
            a[k] = aux[j];
            j++;
        }else if(j > hi){
            //caso já tenha puxado todos os índices da segunda parte do vetor, adiciona a "a" apenas a primeira parte e incrementa a primeira parte;
            a[k] = aux[i];
            i++;
        }else if(less(aux[j], aux[i])){
            //caso o índice da segunda parte seja menor que o índice da primeira parte, adiciona a "a" apenas a segunda parte;
            a[k] = aux[j];
            j++;
        }else{
            //caso o índice da primera parte seja menor, adiciona ele a "a";
            a[k] = aux[i];
            i++;
        }
    }
}
// /// @brief merge_sort normal
// /// @param a 
// /// @param aux 
// /// @param lo 
// /// @param hi 
// void merge_sort(Item *a, Item *aux, int lo, int hi){
//     //significa que o array possui apenas um elemento;
//     if(hi <= lo){
//         return;
//     }

//     int mid = lo + (hi - lo) / 2;

//     //sort com a primeira metade do vetor;
//     merge_sort(a, aux, lo, mid);

//     //sort com a segunda metade do vetor;
//     merge_sort(a, aux, mid + 1, hi);

//     //merge com as duas partes que já estão ordenadas;
//     merge(a, aux, lo, mid, hi);
// }

// /// @brief merge_sort com cutoff
// /// @param a 
// /// @param aux 
// /// @param lo 
// /// @param hi 
// void merge_sort(Item *a, Item *aux, int lo, int hi){
//     //significa que o array possui apenas o número de elementos limite para fazer o insertion sort;
//     if(hi <= lo + CUTOFF - 1){
//         insertion_sort(a, lo, hi);
//         return;
//     }

//     int mid = lo + (hi - lo) / 2;

//     //sort com a primeira metade do vetor;
//     merge_sort(a, aux, lo, mid);

//     //sort com a segunda metade do vetor;
//     merge_sort(a, aux, mid + 1, hi);

//     //merge com as duas partes que já estão ordenadas;
//     merge(a, aux, lo, mid, hi);
// }

// /// @brief merge_sort com merge skip (sem cutoff)
// /// @param a 
// /// @param aux 
// /// @param lo 
// /// @param hi 
// void merge_sort(Item *a, Item *aux, int lo, int hi){
//     //significa que o array possui apenas um elemento;
//     if(hi <= lo){
//         return;
//     }

//     int mid = lo + (hi - lo) / 2;

//     //sort com a primeira metade do vetor;
//     mergee_sort(a, aux, lo, mid);

//     //sort com a segunda metade do vetor;
//     merge_sort(a, aux, mid + 1, hi);

//     //se ao comparar o primeiro da segunda parte com o último da primeira parte, com os dois vetores já ordenadas, o último for menor do que o primeiro, então já está ordenado
//     if(!less(a[mid+1], a[mid])){
//         return;
//     }

//     //merge com as duas partes que já estão ordenadas;
//     merge(a, aux, lo, mid, hi);
// }

// /// @brief merge_sort com merge skip (com cutoff)
// /// @param a 
// /// @param aux 
// /// @param lo 
// /// @param hi 
// void merge_sort(Item *a, Item *aux, int lo, int hi){
//     //significa que o array possui apenas o número de elementos do cutoff;
//     if(hi <= lo + CUTOFF - 1){
//         insertion_sort(a, lo, hi);
//         return;
//     }

//     int mid = lo + (hi - lo) / 2;

//     //sort com a primeira metade do vetor;
//     merge_sort(a, aux, lo, mid);

//     //sort com a segunda metade do vetor;
//     merge_sort(a, aux, mid + 1, hi);

//     //se ao comparar o primeiro da segunda parte com o último da primeira parte, com os dois vetores já ordenadas, o último for menor do que o primeiro, então já está ordenado
//     if(!less(a[mid+1], a[mid])){
//         return;
//     }

//     //merge com as duas partes que já estão ordenadas;
//     merge(a, aux, lo, mid, hi);
// }

/// @brief merge_sort bottom up com merge skip
/// @param a 
/// @param aux 
/// @param lo 
/// @param hi 
void merge_sort(Item *a, int lo, int hi){
    //pega o tamanho do vetor;
    int N = (hi - lo) + 1;

    //pega o final do vetor;
    int y = N - 1;

    Item *aux = malloc(N * sizeof(Item));

    for(int sz  = 1; sz < N; sz = SZ2){
        for(int lo = 0; lo < N - sz; lo += SZ2){
            int x = lo + SZ2 - 1;
            
            if(!less(a[lo + sz], a[lo + sz - 1])){
                break;
            }    
            
            merge(a, aux, lo, lo + sz - 1, MIN(x, y));
        }
    }

    free(aux);
}


// /// @brief sort genérico para merge sort
// /// @param a vetor de itens (inteiros)
// /// @param lo chão do vetor
// /// @param hi teto do vetor
// void sort(Item *a, int lo, int hi){
//     int n = (hi - lo) + 1; //pega o tamanho do vetor (ex: vetor de 8 itens, 0-7, 7-0+1=8);

//     Item *aux = malloc(n * sizeof(Item));

//     merge_sort(a, aux, lo, hi);

//     free(aux);
// }

int main(int argc, char *argv[]){
    Item *itens = (Item*)calloc(atoi(argv[1]), sizeof(Item));

    leitura_arquivo(itens, atoi(argv[1]), argv[2]);

    clock_t begin = clock();

    merge_sort(itens, 0, atoi(argv[1]));

    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    print_itens(itens, atoi(argv[1]));

    printf("Tempo de execução: %lf\n", time_spent);

    free(itens);
}