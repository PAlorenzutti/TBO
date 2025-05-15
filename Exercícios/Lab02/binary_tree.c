#include "binary_tree.h"
#include "vector.h"
#include "stack.h"
#include "queue.h"

#include <stdlib.h>
#include <stdio.h>

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

Node *add_recursive(BinaryTree *bt, Node *atual, void *key, void *val){
    if(atual == NULL){
        return treenode_construct(key, val);
    }

    if(bt->cmp_fn(key, atual->key) > 0){
        atual->right = add_recursive(bt, atual->right, key, val);
    }
    else if(bt->cmp_fn(key, atual->key) < 0){
        atual->left = add_recursive(bt, atual->left, key, val);
    }
    else {  
        //Se for igual a uma chave que já tem na árvore, ignora e libera da memória
        if(key == val){
            //Caso em que chave e valor são iguais
            bt->key_destroy_fn(key);
        }else{
            bt->key_destroy_fn(key);
            bt->val_destroy_fn(val);
        }
        

        return atual;
    }

    return atual;
}

void binary_tree_add(BinaryTree *bt, void *key, void *val){
    bt->root = add_recursive(bt, bt->root, key, val);
}

void treenode_destroy(Node *node, KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn){
    if(node->key == node->val){
        key_destroy_fn(node->key);
    }else{
        key_destroy_fn(node->key);
        val_destroy_fn(node->val);
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

int height_recursive(Node *atual){
    if(atual == NULL){
        return -1;
    }

    int altura_esquerda = height_recursive(atual->left);
    int altura_direita = height_recursive(atual->right);

    if(altura_esquerda > altura_direita){
        return altura_esquerda + 1;
    }else{
        return altura_direita + 1;
    }
}

int binary_tree_height(BinaryTree *bt){
    if(bt->root == NULL){
        return -1;
    }
    
    return height_recursive(bt->root);
}

void print_recursive(Node *node, PrintFn printFn) {
    if (node == NULL) {
        printf("NULL");
        return;
    }

    printf("(");
    printFn(node->key);
    print_recursive(node->left, printFn);
    printf(", ");
    print_recursive(node->right, printFn);
    printf(")");
}

void binary_tree_print(BinaryTree *bt, PrintFn printFn) {
    if (bt == NULL) return;
    print_recursive(bt->root, printFn);
    printf("\n");
}

void binary_tree_destroy(BinaryTree *bt){
    if(bt == NULL){
        return;
    }

    destroy_recursive(bt, bt->root);

    free(bt);
}

void preorder_recursive(Node *node, Vector *v) {
    if (node == NULL) return;
    vector_push_back(v, node->key); 
    preorder_recursive(node->left, v); 
    preorder_recursive(node->right, v); 
}

Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt) {
    Vector *v = vector_construct();
    if (bt != NULL && bt->root != NULL) {
        preorder_recursive(bt->root, v);
    }
    return v;
}

void postorder_recursive(Node *node, Vector *v) {
    if (node == NULL) return;
    postorder_recursive(node->left, v); 
    postorder_recursive(node->right, v); 
    vector_push_back(v, node->key); 
}

Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt) {
    Vector *v = vector_construct();

    if (bt != NULL && bt->root != NULL) {
        postorder_recursive(bt->root, v);
    }

    return v;
}

void inorder_recursive(Node *node, Vector *v) {
    if (node == NULL) return;
    inorder_recursive(node->left, v); 
    vector_push_back(v, node->key); 
    inorder_recursive(node->right, v); 
}

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt) {
    Vector *v = vector_construct();
    if (bt != NULL && bt->root != NULL) {
        inorder_recursive(bt->root, v);
    }
    return v;
}

// Inorder traversal iterativo
Vector *binary_tree_inorder_traversal(BinaryTree *bt) {
    Vector *v = vector_construct();
    if (bt == NULL || bt->root == NULL) return v;

    Stack *s = stack_construct();
    Node *current = bt->root;

    while (current != NULL || !stack_empty(s)) {
        // Vai até o nó mais à esquerda possível
        while (current != NULL) {
            stack_push(s, current);
            current = current->left;
        }

        // Visita o nó no topo da pilha
        current = stack_pop(s);
        vector_push_back(v, current->key);

        // Vai para a subárvore direita
        current = current->right;
    }

    stack_destroy(s);
    return v;
}

// Preorder traversal iterativo
Vector *binary_tree_preorder_traversal(BinaryTree *bt) {
    Vector *v = vector_construct();
    if (bt == NULL || bt->root == NULL) return v;

    Stack *s = stack_construct();
    stack_push(s, bt->root);

    while (!stack_empty(s)) {
        Node *current = stack_pop(s);
        vector_push_back(v, current->key);

        // Empilha primeiro o filho direito para ser processado depois do esquerdo
        if (current->right != NULL) {
            stack_push(s, current->right);
        }
        if (current->left != NULL) {
            stack_push(s, current->left);
        }
    }

    stack_destroy(s);
    return v;
}

// Postorder traversal iterativo (usando duas pilhas)
Vector *binary_tree_postorder_traversal(BinaryTree *bt) {
    Vector *v = vector_construct();
    if (bt == NULL || bt->root == NULL) return v;

    Stack *s1 = stack_construct();
    Stack *s2 = stack_construct();
    stack_push(s1, bt->root);

    while (!stack_empty(s1)) {
        Node *current = stack_pop(s1);
        stack_push(s2, current);

        // Empilha os filhos esquerdo e direito na primeira pilha
        if (current->left != NULL) {
            stack_push(s1, current->left);
        }
        if (current->right != NULL) {
            stack_push(s1, current->right);
        }
    }

    // A segunda pilha agora contém os nós em ordem postorder reversa
    while (!stack_empty(s2)) {
        Node *node = stack_pop(s2);
        vector_push_back(v, node->key);
    }

    stack_destroy(s1);
    stack_destroy(s2);
    return v;
}

Vector *binary_tree_levelorder_traversal(BinaryTree *bt) {
    if (bt == NULL || bt->root == NULL) {
        return vector_construct(); 
    }

    Vector *output = vector_construct(); 
    Queue *queue = queue_construct();

    queue_enqueue(queue, bt->root);

    while (vector_size(queue) > 0) {
        Node *current = queue_dequeue(queue); 

        vector_push_back(output, current->key);

        if (current->left != NULL) {
            queue_enqueue(queue, current->left);
        }
        if (current->right != NULL) {
            queue_enqueue(queue, current->right);
        }
    }

    queue_destroy(queue);

    return output;
}