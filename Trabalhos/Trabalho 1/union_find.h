#ifndef UNION_FIND_H
#define UNION_FIND_H

typedef struct UnionFind UnionFind;

/**
 * @brief Cria a estrutura de UnionFind para n elementos.
 * @param n Número de elementos.
 * @return Um ponteiro para a estrutura UnionFind.
 */
UnionFind *criar_union_find(int n);

/**
 * @brief Encontra o representante do conjunto que contém o elemento i.
 * @param uf Um ponteiro para a estrutura UnionFind.
 * @param i O elemento i.
 * @return O representante do conjunto que contém i.
 */
int find(UnionFind* uf, int i);

/**
 * @brief Une os conjuntos que contêm os elementos i e j.
 * @param uf Um ponteiro para a estrutura UnionFind.
 * @param i O elemento i;
 * @param j O elemento j;
 */
void union_sets(UnionFind* uf, int i, int j);

/**
 * @brief Libera toda a memória associada a UnionFind.
 * @param uf Um ponteiro para a estrutura UnionFind.
 */
void liberar_union_find(UnionFind* uf);

#endif