#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

typedef struct BinaryTree BinaryTree;

typedef struct Node Node;

typedef int (*CmpFn) (void*, void*);

typedef void (*ValDestroyFn) (void *);

typedef void (*KeyDestroyFn) (void *);

BinaryTree *binary_tree_construct(KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn, CmpFn cmp_fn);

void binary_tree_add(BinaryTree *bt, void *key, void *val);

void binary_tree_destroy(BinaryTree *bt);


#endif