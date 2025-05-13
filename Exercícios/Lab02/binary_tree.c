#include "binary_tree.h"

#include <stdlib.h>

struct Node{
    void *key;
    void *val;
    struct Node *right;
    struct Node *left;
};

struct BinaryTree{
    Node *root;
    KeyDestroyFn key_destroy_fn;
    ValDestroyFn val_destroy_fn;
    CmpFn cmp_fn;
};

BinaryTree *binary_tree_construct(KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn, CmpFn cmp_fn){
    BinaryTree *bt = (BinaryTree*)malloc(sizeof(BinaryTree));

    bt->cmp_fn = cmp_fn;
    bt->val_destroy_fn = val_destroy_fn;
    bt->key_destroy_fn = key_destroy_fn;

    bt->root = NULL;

    return bt;
}

Node *treenode_construct(void *key, void *val){
    Node *node = (Node*)malloc(sizeof(Node));

    node->key = key;
    node->val = val;

    node->left = NULL;
    node->right = NULL;

    return node;
}

Node *add_recursive(Node *atual, void *key, void *val, CmpFn cmp_fn){
    if(atual == NULL){
        return treenode_construct(key, val);
    }

    if(cmp_fn(key, atual->key) > 0){
        atual->right = add_recursive(atual->right, key, val, cmp_fn);
    }

    if(cmp_fn(key, atual->key) < 0){
        atual->left = add_recursive(atual->left, key, val, cmp_fn);
    }

    return atual;
}

void binary_tree_add(BinaryTree *bt, void *key, void *val){
    bt->root = add_recursive(bt->root, key, val, bt->cmp_fn);
}

void treenode_destroy(Node *node, KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn){
    if(key_destroy_fn == val_destroy_fn){
        key_destroy_fn(node->key);
    }else{
        key_destroy_fn(node->key);
        key_destroy_fn(node->val);
    }

    free(node);
}

void destroy_recursive(BinaryTree *bt, Node *atual){
    if(atual == NULL){
        return;
    }

    destroy_recursive(bt, atual->left);

    destroy_recursive(bt, atual->right);

    treenode_destroy(atual, bt->key_destroy_fn, bt->val_destroy_fn);
}

void binary_tree_destroy(BinaryTree *bt){
    if(bt == NULL){
        return;
    }

    destroy_recursive(bt, bt->root);

    free(bt);
}