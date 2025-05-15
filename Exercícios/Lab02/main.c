#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "binary_tree.h"

int cmp_fn(void *key1, void *key2)
{
    int *v = key1;
    int *w = key2;

    return *v - *w;
}

void key_destroy_fn(void *key)
{
    int *v = key;
    free(v);
}

void print_fn(void *key){
    int *v = key;
    printf("%d, ", *v);
}

int main(){
    BinaryTree *bt = binary_tree_construct(key_destroy_fn, key_destroy_fn, cmp_fn);

    srand(time(NULL));

    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        int val = rand();

        int *ptr = malloc(sizeof(int));
        *ptr = val;

        binary_tree_add(bt, ptr, ptr);
    }


    printf("%d", binary_tree_height(bt));

    binary_tree_destroy(bt);
    
    return 0;
}