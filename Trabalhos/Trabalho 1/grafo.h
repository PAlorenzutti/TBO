#ifndef GRAFO_H
#define GRAFO_H

#include "arquivo.h"  // Para acessar ConjuntoDePontos
#include "union_find.h"

typedef struct Aresta Aresta;
typedef struct Grafo Grafo;
typedef struct Grupo Grupo;

/**
 * @brief Constrói o grafo completo a partir do conjunto de pontos.
 * @param conjunto O conjunto de pontos para gerar o grafo.
 * @return Um grafo com arestas que representam a distância euclidiana entre dois pontos
 */
Grafo *construir_grafo_completo(ConjuntoDePontos *conjunto);

/**
 * @brief Retorna o número de arestas no grafo.
 * @param grafo Um grafo completo.
 * @return O número de arestas.
 */
int obter_num_arestas(Grafo *grafo);

/**
 * @brief Retorna o ponteiro para a i-ésima aresta.
 * @param grafo Um grafo completo.
 * @param idx O índice da aresta.
 * @return O ponteiro para a i-ésima aresta.
 */
Aresta *obter_aresta(Grafo *grafo, int idx);

/**
 * @brief Retorna índice do ponto u da aresta.
 * @param aresta O ponteiro para a aresta.
 * @return O índice do ponto u da aresta.
 */
int obter_u(Aresta *aresta);

/**
 * @brief Retorna índice do ponto v da aresta.
 * @param aresta O ponteiro para a aresta.
 * @return O índice do ponto v da aresta.
 */
int obter_v(Aresta *aresta);

/**
 * @brief Retorna o peso (distância) da aresta.
 * @param aresta O ponteiro para a aresta.
 * @return O peso da aresta.
 */
double obter_peso(Aresta *aresta);

/**
 * @brief Libera toda a memória alocada para o grafo.
 * @param grafo Um grafo completo.
 */
void liberar_grafo(Grafo *grafo);

/**
 * @brief Executa o algoritmo de Kruskal com uma leve modificação para formar cada grupo de pontos separados
 * @param grafo Um grafo completo.
 * @param num_vertices O número de vértices do grafo.
 * @param k O número de grupos desejado.
 * @return Um ponteiro da estrutura UnionFind que possui os grupos separados.
 */
UnionFind *forma_grupos(Grafo *grafo, int num_vertices, int k);

/**
 * @brief Organiza os grupos com os critérios de ordenação para ser gerado a saída posteriormente.
 * @param uf Um ponteiro para a estrutura UnionFind que contém cada grupo separado, mas não organizado.
 * @param num_vertices Número total de pontos.
 * @param k Número de grupos desejado.
 * @param pontos Conjunto de pontos (para obter os IDs).
 * @return Um ponteiro com os grupos organizados.
 */
Grupo *organiza_grupos(UnionFind *uf, int num_vertices, int k, ConjuntoDePontos *pontos);

/**
 * @brief Gera o arquivo de saída escrevendo os id's de cada grupo.
 * @param grupos Ponteiro para os grupos organizados.
 * @param nome_arquivo_saida Nome desejado para o arquivo de saída.
 * @param k Número de grupos desejado.
 */
void gera_saida(Grupo *grupos, const char *nome_arquivo_saida, int k);

/**
 * @brief Libera toda a memória associada ao Grupo.
 * @param grupos Ponteiro para os grupos organizados.
 * @param k Número de grupos desejado.
 */
void libera_grupos(Grupo *grupos, int k);

#endif
