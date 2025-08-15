#ifndef BST_H
#define BST_H

#include "vector.h"

typedef struct Node Node;
typedef struct BinaryTree BinaryTree;

typedef int (*cmp_fn)(const void *, const void *);
typedef void (*key_destroy_fn)(void *);
typedef void (*val_destroy_fn)(void *);
typedef void (*print_fn)(const void *);

typedef void *Value;
typedef void *Key;

BinaryTree *bt_init(key_destroy_fn key_destroy_fn, val_destroy_fn val_destroy_fn, cmp_fn cmp_fn);

void bt_put(BinaryTree *bt, void *key, void *val);

void bt_delete(BinaryTree *bt, Key key);

void bt_delmin(BinaryTree *bt);

void bt_delmax(BinaryTree *bt);

int bt_height(BinaryTree *bt);

void bt_destroy(BinaryTree *bt);

void bt_print(BinaryTree *bt, print_fn print_fn);

Vector *bt_inorder_rec(BinaryTree *bt);

Vector *bt_preorder_rec(BinaryTree *bt);

Vector *bt_postorder_rec(BinaryTree *bt);

Vector *bt_inorder(BinaryTree *bt);

Vector *bt_preorder(BinaryTree *bt);

Vector *bt_postoder(BinaryTree *bt);

#endif