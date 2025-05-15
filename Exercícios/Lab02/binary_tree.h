#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#include "vector.h"

typedef struct BinaryTree BinaryTree;

typedef struct Node Node;

typedef int (*CmpFn) (void*, void*);

typedef void (*ValDestroyFn) (void *);

typedef void (*KeyDestroyFn) (void *);

typedef void (*PrintFn) (void *);

BinaryTree *binary_tree_construct(KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn, CmpFn cmp_fn);

void binary_tree_add(BinaryTree *bt, void *key, void *val);

int binary_tree_height(BinaryTree *bt);

void binary_tree_destroy(BinaryTree *bt);

void binary_tree_print(BinaryTree *bt, PrintFn print_fn);

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt);

Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt);

Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt);

Vector *binary_tree_inorder_traversal(BinaryTree *bt);

Vector *binary_tree_preorder_traversal(BinaryTree *bt);

Vector *binary_tree_postorder_traversal(BinaryTree *bt);


#endif