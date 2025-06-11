#include "union_find.h"
#include <stdlib.h>

struct UnionFind {
    int *id;
    int *sz;
};

// O(n)
UnionFind* criar_union_find(int n) {

    UnionFind* uf = (UnionFind *)malloc(sizeof(UnionFind));
    if(uf == NULL) {
        return NULL;
    }

    uf->id = (int *)malloc(n * sizeof(int));
    if(uf->id == NULL) {
        free(uf);
        return NULL;
    }

    uf->sz = (int *)malloc(n * sizeof(int));
    if(uf->sz == NULL) {
        free(uf->id);
        free(uf);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        uf->id[i] = i;
        uf->sz[i] = 1;
    }

    return uf;
}

// O(a(n))
int find(UnionFind* uf, int i) {

    while(i != uf->id[i]) {
        uf->id[i] = uf->id[uf->id[i]];
        i = uf->id[i];
    }
    return i;
}

// O(2*a(n))
void union_sets(UnionFind* uf, int i, int j) {
    int root_i = find(uf, i);
    int root_j = find(uf, j);

    if (root_i == root_j) return;

    if(uf->sz[root_i] < uf->sz[root_j]) {
        uf->id[root_i] = root_j;
        uf->sz[root_j] += uf->sz[root_i];
    }
    else {
        uf->id[root_j] = root_i;
        uf->sz[root_i] += uf->sz[root_j];
    }
}

void liberar_union_find(UnionFind* uf) {
    free(uf->id);
    free(uf->sz);
    free(uf);
}