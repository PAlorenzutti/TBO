#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"
#include "grafo.h"

int main(int argc, char* argv[]) {

    // Verifica se a quantidade de argumentos é igual a 4
    if (argc != 4) {
        return 1;
    }

    const char* nome_entrada = argv[1];
    int k = atoi(argv[2]);
    const char* nome_saida = argv[3];

    // Verifica se o número de grupos é maior ou igual a 1
    if (k < 1) {
        return 1;
    }

    // 1. Leitura dos pontos
    ConjuntoDePontos* pontos = ler_pontos(nome_entrada);
    if (pontos == NULL) {
        return 1;
    }

    int n = obter_num_pontos(pontos);

    // 2. Construção do grafo
    Grafo* grafo = construir_grafo_completo(pontos);
    if (grafo == NULL) {
        liberar_pontos(pontos);
        return 1;
    }

    // 3. Geração da MST
    UnionFind *uf = forma_grupos(grafo, n, k);
    if (uf == NULL) {
        liberar_grafo(grafo);
        liberar_pontos(pontos);
        return 1;
    }

    // 4. Agrupamento e escrita do resultado
    Grupo *grupos = organiza_grupos(uf,n, k, pontos);
    if(grupos == NULL) {
        liberar_union_find(uf);
        liberar_pontos(pontos);
        return 1;
    }

    gera_saida(grupos, nome_saida, k);

    // 5. Liberação
    libera_grupos(grupos, k);
    liberar_pontos(pontos);

    return 0;
}
