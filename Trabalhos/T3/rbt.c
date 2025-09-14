#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "rbt.h"

#define RED true
#define BLACK false


typedef struct Node {
    Key key;
    Value val;
    bool color;
    struct Node *left, *right;
} Node;

struct RBT {

    cmp_func cmp_fn;
    key_destroy key_destroy_fn;
    val_destroy val_destroy_fn;
    Node *root;
};

// NODE FUNCTIONS
Node *rbt_node_init(Key key, Value val) {

    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->val = val;
    node->color = RED;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void rbt_rec_node_destroy(Node *node, key_destroy key_destroy_fn, val_destroy val_destroy_fn) {

    if(node == NULL) {
        return;
    }
    if (key_destroy_fn == val_destroy_fn) {
        key_destroy_fn(node->key);
        // Não chamar val_destroy_fn para evitar double free
    } else {
        key_destroy_fn(node->key);
        val_destroy_fn(node->val);
    }
    rbt_rec_node_destroy(node->left, key_destroy_fn, val_destroy_fn);
    rbt_rec_node_destroy(node->right, key_destroy_fn, val_destroy_fn);
    free(node);
}

bool is_red(Node *node) {
    if(node == NULL) return BLACK;
    return node->color; //  RED == true
}

Node *rotate_left(Node *h) {
    Node *x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = x->left->color;
    x->left->color = RED;
    return x;
}

Node *rotate_right(Node *h) {
    Node *x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = x->right->color;
    x->right->color = RED;
    return x;
}

void flip_colors(Node *h) {
    h->color = RED;
    h->left->color = BLACK;
    h->right->color = BLACK;
}
// #############################################################################################################################


// RBT FUNCTIONS
RBT *rbt_init(cmp_func cmp_fn, key_destroy key_destroy_fn, val_destroy val_destroy_fn) {

    RBT *rbt = (RBT *)malloc(sizeof(RBT));

    rbt->root = NULL;
    rbt->cmp_fn = cmp_fn;
    rbt->key_destroy_fn = key_destroy_fn;
    rbt->val_destroy_fn = val_destroy_fn;

    return rbt;
}

Value rec_rbt_get(Node *node, Key key, cmp_func cmp_fn) {

    if(node == NULL) return NULL;
    int cmp = cmp_fn(key, node->key);
    if(cmp < 0) {
        return rec_rbt_get(node->left, key, cmp_fn);
    }
    else if(cmp > 0) {
        return rec_rbt_get(node->right, key, cmp_fn);
    }
    else {
        return node->val;
    }
}

Value rbt_get(RBT *rbt, Key key) {
    return rec_rbt_get(rbt->root, key, rbt->cmp_fn);
}

Node *rec_rbt_insert(Node *h, Key key, Value val, cmp_func cmp_fn) {

    if(h == NULL) {
        return rbt_node_init(key, val);
    }
    int cmp = cmp_fn(key, h->key);
    if(cmp < 0) {
        h->left = rec_rbt_insert(h->left, key, val, cmp_fn);
    }
    else if(cmp > 0) {
        h->right = rec_rbt_insert(h->right, key, val ,cmp_fn);
    }
    else {
        h->val = val;
    }

    if(is_red(h->right) && !is_red(h->left)) { h = rotate_left(h); }
    if(is_red(h->left) && is_red(h->left->left)) { h = rotate_right(h); }
    if(is_red(h->left) && is_red(h->right)) { flip_colors(h); }

    return h;
}

void rbt_insert(RBT *rbt, Key key, Value val) {
    rbt->root = rec_rbt_insert(rbt->root, key, val, rbt->cmp_fn);
}

void rbt_destroy(RBT *rbt) {
    rbt_rec_node_destroy(rbt->root, rbt->key_destroy_fn, rbt->val_destroy_fn);
    free(rbt);
}


// DEPURAÇÃO
void rbt_print_inorder(Node *node, key_print key_print_fn, val_print val_print_fn) {
    if(node == NULL) return;
    rbt_print_inorder(node->left, key_print_fn, val_print_fn);
    key_print_fn(node->key);
    val_print_fn(node->val);
    rbt_print_inorder(node->right, key_print_fn, val_print_fn);
}

void rbt_print(RBT *rbt, key_print key_print_fn, val_print val_print_fn) {
    rbt_print_inorder(rbt->root, key_print_fn, val_print_fn);
}