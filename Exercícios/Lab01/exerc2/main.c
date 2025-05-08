#include "vector.h"

#include <stdio.h>
#include <stdlib.h>

int main(){
    Vector *pessoas = vector_construct();

    int N, M;
    scanf("%d %d\n", &N, &M);

    //constrói o vetor de pessoas;
    for(int i = 0; i < N; i++){
        int *pessoa = (int*)malloc(sizeof(int));
        *pessoa = i;
        vector_push_back(pessoas, pessoa);
    }

    int index_atual = 0;

    while(vector_size(pessoas) > 1){
        index_atual = (index_atual + M - 1) % vector_size(pessoas);

        int *pessoa_removida = vector_remove(pessoas, index_atual);

        printf("Pessoa %d foi removida da roda\n", *pessoa_removida + 1);

        free(pessoa_removida);
    }

    int *lider = vector_pop_back(pessoas);
    printf("Pessoa %d é a líder\n", *lider + 1);
    free(lider);

    vector_destroy(pessoas);

    return 0;
}