#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bst.h"

int int_cmp(const void *key1, const void *key2)
{
    const int *v = key1;
    const int *w = key2;
    return *v - *w;
}

void int_destroy(void *key)
{
    int *v = key;
    free(v);
}

void int_print(const void *key){
    const int *v = key;
    printf("%d, ", *v);
}

int main(){
    BinaryTree *bt = bt_init(int_destroy, int_destroy, int_cmp);

    srand(time(NULL));

    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        int val = rand();

        int *ptr = malloc(sizeof(int));
        *ptr = val;

        bt_put(bt, ptr, ptr);
    }


    // printf("%d", bt_height(bt));

    bt_print(bt, int_print);

    bt_destroy(bt);
    
    return 0;
}