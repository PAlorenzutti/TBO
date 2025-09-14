#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

// ===================================================================================================================
// ESTRUTURAS PRINCIPAIS
// ===================================================================================================================

struct Aresta {
    int u; // Índice do ponto u
    int v; // Índice do ponto v
    double peso; // peso da aresta entre u e v
};

struct Grafo {
    Aresta *arestas; // conjunto de arestas
    int num_arestas; // Numero de arestas
};

struct Grupo{
    char **ids;
    int tamanho; // Tamanho atual, ou seja, quantos ids estão naquele grupo
    int allocated; // Quantidade de char* alocados
};

// ===================================================================================================================
// FUNÇÕES AUXILIARES
// ===================================================================================================================

// Função que compara o peso entre duas arestas.
int comparar_arestas_peso(const void *a, const void *b) {
    Aresta *ar1 = (Aresta *)a;
    Aresta *ar2 = (Aresta *)b;
    if (ar1->peso > ar2->peso) return 1;
    if (ar1->peso < ar2->peso) return -1;
    return 0;
}

// O(m)
// Função que calcula a distância entre dois pontos.
double calcular_distancia(Ponto *p1, Ponto *p2, int m) {
    double soma = 0.0;
    // O(m)
    for (int i = 0; i < m; i++) {
        double diff = obter_uma_coord(p1, i) - obter_uma_coord(p2, i);
        soma += diff * diff;
    }
    // Imagino que não seja necessário fazer a raiz quadrada.
    return soma;
}

// Função que compara cada identificador lexicograficamente dentro de um mesmo grupo
int comparar_ids(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// Função que compara o primeiro identificador entre grupos diferentes
int comparar_grupos_por_menor_id(const void *a, const void *b) {
    Grupo *g1 = (Grupo*)a;
    Grupo *g2 = (Grupo*)b;
    return strcmp(g1->ids[0], g2->ids[0]);
}

// ===================================================================================================================
// FUNÇÕES PARA GRAFO
// ===================================================================================================================

// O((n²/2) * m) + O(n*logn)
Grafo *construir_grafo_completo(ConjuntoDePontos *conjunto) {

    int n = obter_num_pontos(conjunto); // Pego o número de pontos
    int m = obter_num_dimensoes(conjunto); // Pego o número de dimensões de cada ponto
    int capacidade = (n * (n - 1)) / 2; // Calculo a quantidade máxima de arestas que o grafo terá

    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo)); // Aloco espaço para a estrutura Grafo
    if(grafo == NULL) {
        return NULL;
    }

    grafo->arestas = (Aresta *)malloc(capacidade * sizeof(Aresta)); // Aloco espaço para o conjuto de arestas
    if(grafo->arestas == NULL) {
        free(grafo);
        return NULL;
    }

    grafo->num_arestas = 0; // Inicialmente é 0

    // O((n²/2) * m)
    // Cria as arestas do grafo, com os vértices que ligam ela e o peso entre eles.
    for (int i = 0; i < n; i++) {

        Ponto* p1 = obter_ponto(conjunto, i); // Pego o primeiro ponto

        for(int j = i + 1; j < n; j++) {

            Ponto* p2 = obter_ponto(conjunto, j); // Pego o segundo

            // O(m)
            double d = calcular_distancia(p1, p2, m); // Calculo a distância entre eles

            Aresta aresta; // Inicializo a estrutura Aresta vazia
            aresta.u = i; // Defino u com o índice do primeiro ponto
            aresta.v = j; // Defino v com o índice do segundo ponto
            aresta.peso = d; // Defino o peso da aresta como a distância entre os dois pontos

            grafo->arestas[grafo->num_arestas] = aresta; // Insiro a aresta no conjunto de arestas
            grafo->num_arestas++;
        }
    }

    // O(n*logn) -> n = numero de arestas
    // Já aproveito para ordenar as arestas em ordem crescente
    qsort(grafo->arestas, grafo->num_arestas, sizeof(Aresta), comparar_arestas_peso);

    //ordenar_arestas_por_peso(grafo); // Já aproveito para ordenar as arestas em ordem crescente

    return grafo;
}

int obter_num_arestas(Grafo *grafo) {
    return grafo->num_arestas;
}

Aresta *obter_aresta(Grafo *grafo, int idx) {
    if ( idx < 0 || idx >= grafo->num_arestas) return NULL;
    return &grafo->arestas[idx];
}

int obter_u(Aresta *aresta) {
    return aresta->u;
}

int obter_v(Aresta *aresta) {
    return aresta->v;
}

double obter_peso(Aresta *aresta) {
    return aresta->peso;
}

void liberar_grafo(Grafo *grafo) {
    free(grafo->arestas);
    free(grafo);
}


// ===================================================================================================================
// FUNÇÃO PARA KRUSKAL MODIFICADA
// ===================================================================================================================

// O(n) + O((n - 1 - (k -1)) * a(n))
UnionFind *forma_grupos(Grafo *grafo, int num_vertices, int k) {

    // Numero atual de arestas
    int num_arestas = 0;

    // O(n)
    // Estrutura para formar e guardar os grupos
    UnionFind *uf = criar_union_find(num_vertices); 
    if(uf == NULL) {
        return NULL;
    }

    // O((n - 1 - (k -1)) * a(n)) -> a = alpha (função de ackermann inversa)
    // Algoritmo de kruskal modificado para já formar cada grupo separado.
    // Obs: Nesse caso não há necessidade de formar primeiro a MST e depois remover as k - 1 arestas mais pesadas.
    // Uma solução mais rápida é não inserir essas k - 1 arestas. Assim, já será obtido cada grupo separadamente.
    for (int i = 0; num_arestas < ((num_vertices - 1) - (k - 1)); i++) {
        Aresta *aresta = obter_aresta(grafo, i);
        int u = obter_u(aresta);
        int v = obter_v(aresta);

        if (find(uf, u) != find(uf, v)) {
            union_sets(uf, u, v);
            num_arestas++;
        }
    }

    // Faço a liberação do grafo, pois não será mais utilizado no programa.
    // Essa estrutura consome muita memória por conta do conjunto de arestas,
    // então fazer essa liberação aqui já alivia o consumo de memória.
    liberar_grafo(grafo);

    return uf;
}

// ===================================================================================================================
// FUNÇÕES PARA O AGRUPAMENTO
// ===================================================================================================================

// O(n * (O(n * a(n)) + O(k))) + O(k*(n*logn)) + O(k*logk)
Grupo *organiza_grupos(UnionFind *uf, int num_vertices, int k, ConjuntoDePontos *pontos) {

    // Agrupar os pontos por representante
    Grupo *grupos = (Grupo *)malloc(sizeof(Grupo) * k); // Aloco um vetor de grupos
    if(grupos == NULL) {
        return NULL;
    }

    int *representantes = (int *)malloc(sizeof(int) * k); // Aloco um vetor para armazenar os representantes de cada grupo
    if(representantes == NULL) {
        free(grupos);
        return NULL;
    }

    int num_grupos = 0; // Quantos grupos eu tenho atualmente no vetor de grupos

    // Obs: um representante é o índice que representa a raiz dentro da estrutura union_find.
    // Com isso eu consigo separar quais pontos estão em um mesmo conjunto, pois se eles possuem
    // um mesmo representante, logo estão no mesmo conjunto.

    /*
        Vamos imaginar o seguinte:

        Grupo *grupos = [G1, G2, G3]; Um vetor de grupos
        int *representantes = [3, 0, 5]; Um vetor que armazena os representantes de cada grupo.

        Isso signica que quando eu dou um find(uf, i) e descubro o representante do vértice i, eu consigo verificar
        no array int *representantes de qual grupo aquele vértice pertence.
        Se o representante retornado pelo find for 0, eu irei descobrir no array de representantes que o grupo 
        desse vértice está na posição 1 do vetor de grupos, ou seja, ele pertence ao G2.
        
        Entretanto, caso o find(uf, i) retorne algum número que não está no array de representantes, isso indica que
        aquele vértice pertence a um grupo separado dos demais, ou seja, um novo grupo. Esse novo grupo é criado e 
        adicionado ao vetor de grupos. Então no final teríamos os seguintes vetores:

        Grupo *grupos = [G1, G2, G3, G4];
        int *representantes = [3, 0, 5, 7]; Um vetor que armazena os representantes de cada grupo.
    */

    // O(n * (O(n * a(n)) + O(k)))
    for(int i = 0; i < num_vertices; i++) {
        int rep = find(uf, i); // Quem representa o ponto i?
        int grupo_idx = -1; // Índice do grupo que eu irei inserir o vértice

        // O(k)
        // Esse loop é responsável por iterar no vetor de representantes
        // para descobrir se aquele ponto pertence a um grupo já existente
        for(int j = 0; j < num_grupos; j++) {

            // Caso aquele ponto pertença a um grupo já criado
            if (rep == representantes[j]) {
                grupo_idx = j; // Consigo definir qual é o índice do grupo que irei inserir o ponto
                break;
            }
        }


        // Caso eu não descobrir qual grupo o ponto pertence,
        // isso indica que aquele ponto representa um novo grupo
        if (grupo_idx == -1) {

            representantes[num_grupos] = rep; // Indico que esse ponto é o representante de um novo grupo
            grupo_idx = num_grupos; // O índice do novo grupo será a próxima posição livre no vetor de grupos

            // Crio um novo grupo
            Grupo novo_grupo;
            novo_grupo.tamanho = 0; // Atualmente nenhum identificiador está no grupo
            novo_grupo.allocated = 4; // Tamanho inicial do vetor de id's que estão no grupo
            novo_grupo.ids = (char **)malloc(sizeof(char *) * novo_grupo.allocated); // Aloco espaço para esses id's

            grupos[grupo_idx] = novo_grupo; // Adiciono o novo grupo no vetor de grupos
            num_grupos++; // Incremento a contagem de grupos
        }
        
        // Verifico se há necessidade de alocar mais espaço para o vetor de id's e aumento o allocated
        if (grupos[grupo_idx].tamanho == grupos[grupo_idx].allocated) {
            grupos[grupo_idx].allocated *= 2;
            grupos[grupo_idx].ids = (char **)realloc(grupos[grupo_idx].ids, sizeof(char*) * grupos[grupo_idx].allocated);
        }

        // Insiro o identificador no seu grupo respectivo e aumento o tamanho desse grupo
        grupos[grupo_idx].ids[grupos[grupo_idx].tamanho] = strdup(obter_id(obter_ponto(pontos, i)));
        grupos[grupo_idx].tamanho++;
    }

    liberar_union_find(uf);
    free(representantes);

    // O(k*(n*logn)) -> n = quantos identificadores tem no grupo
    // Ordeno os identificadores de cada grupo
    for (int i = 0; i < num_grupos; i++) {
        qsort(grupos[i].ids, grupos[i].tamanho, sizeof(char*), comparar_ids);
    }

    // O(k*logk) -> k = numero de grupos
    // Ordeno os grupos lexicograficamente, pelo identificador
    qsort(grupos, num_grupos, sizeof(Grupo), comparar_grupos_por_menor_id);

    return grupos;
}


// O(k * n) -> onde n = quantos identificadores tem no grupo
void gera_saida(Grupo *grupos, const char *nome_arquivo_saida, int k) {

    FILE *f = fopen(nome_arquivo_saida, "w");
    if (f == NULL) {
        return;
    }

    for(int i = 0; i < k; i++) {
        for (int j = 0; j < grupos[i].tamanho; j++) {
            fprintf(f, "%s", grupos[i].ids[j]);
            if (j < grupos[i].tamanho - 1) fprintf(f, ",");
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

void libera_grupos(Grupo *grupos, int k) {

    for(int i = 0; i < k; i++) {
        for(int j = 0; j < grupos[i].tamanho; j++) {
            free(grupos[i].ids[j]);
        }
        free(grupos[i].ids);
    }
    free(grupos);
}