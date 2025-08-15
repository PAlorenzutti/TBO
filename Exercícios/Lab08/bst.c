#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bst.h"
#include "queue.h"
#include "stack.h"



struct Node
{

    Key key;
    Value val;
    Node *left, *right;
};

struct BinaryTree
{

    Node *root;
    cmp_fn cmp_fn;
    key_destroy_fn key_destroy_fn;
    val_destroy_fn val_destroy_fn;
};

// NODE FUNCTIONS
Node *node_init(Key key, Value val)
{

    Node *node = (Node *)malloc(sizeof(Node));

    node->key = key;
    node->val = val;

    node->left = NULL;
    node->right = NULL;

    return node;
}

int node_height(Node *atual)
{
    if (atual == NULL)
    {
        return -1;
    }

    int altura_esquerda = node_height(atual->left);
    int altura_direita = node_height(atual->right);

    if (altura_esquerda > altura_direita)
    {
        return altura_esquerda + 1;
    }
    else
    {
        return altura_direita + 1;
    }
}

void node_destroy(Node *node, key_destroy_fn key_destroy_fn, val_destroy_fn val_destroy_fn)
{

    if (key_destroy_fn == val_destroy_fn)
    {
        key_destroy_fn(node->key);
    }
    else
    {
        key_destroy_fn(node->key);
        val_destroy_fn(node->val);
    }

    free(node);
}

void rec_node_destroy(Node *node, key_destroy_fn key_destroy_fn, val_destroy_fn val_destroy_fn)
{
    if (node == NULL)
    {
        return;
    }

    if (key_destroy_fn == val_destroy_fn)
    {
        key_destroy_fn(node->key);
    }
    else
    {
        key_destroy_fn(node->key);
        val_destroy_fn(node->val);
    }

    rec_node_destroy(node->left, key_destroy_fn, val_destroy_fn);
    rec_node_destroy(node->right, key_destroy_fn, val_destroy_fn);
    free(node);
}

void node_print(Node *n, print_fn print_fn)
{

    if (n == NULL)
    {
        printf("NULL");
        return;
    }
    printf("(");
    print_fn(n->val);
    printf(", ");
    node_print(n->left, print_fn);
    printf(", ");
    node_print(n->right, print_fn);
    printf(")");
}

// BINARY TREE FUNCTIONS
BinaryTree *bt_init(key_destroy_fn key_destroy_fn, val_destroy_fn val_destroy_fn, cmp_fn cmp_fn)
{
    BinaryTree *bt = (BinaryTree *)malloc(sizeof(BinaryTree));
    bt->root = NULL;
    bt->cmp_fn = cmp_fn;
    bt->key_destroy_fn = key_destroy_fn;
    bt->val_destroy_fn = val_destroy_fn;
    return bt;
}

Value bt_rec_get(Node *node, Key key, cmp_fn func)
{

    if (node == NULL)
        return NULL;
    int cmp_fn = func(key, node->key);
    if (cmp_fn < 0)
    {
        return bt_rec_get(node->left, key, func);
    }
    else if (cmp_fn > 0)
    {
        return bt_rec_get(node->right, key, func);
    }
    else
    {
        return node->val;
    }
}

Value bt_get(BinaryTree *bt, Key key)
{
    return bt_rec_get(bt->root, key, bt->cmp_fn);
}

Node *bt_rec_put(Node *node, Key key, Value val, cmp_fn func, key_destroy_fn key_destroy_fn, val_destroy_fn val_func)
{

    if (node == NULL)
        return node_init(key, val);
    int cmp_fn = func(key, node->key);
    if (cmp_fn < 0)
    {
        node->left = bt_rec_put(node->left, key, val, func, key_destroy_fn, val_func);
    }
    else if (cmp_fn > 0)
    {
        node->right = bt_rec_put(node->right, key, val, func, key_destroy_fn, val_func);
    }
    else
    { // chave já existe
        // Se for igual a uma chave que já tem na árvore, ignora e libera da memória
        if (key == val)
        {
            // Caso em que chave e valor são iguais
            key_destroy_fn(key);
        }
        else
        {
            key_destroy_fn(key);
            val_destroy_fn(val);
        }

        return node;
    }
    return node;
}

void bt_put(BinaryTree *bt, Key key, Value val)
{
    bt->root = bt_rec_put(bt->root, key, val, bt->cmp_fn, bt->key_destroy_fn, bt->val_destroy_fn);
}

Node *rec_delmin(Node *node, bool del, key_destroy_fn key_destroy_fn, val_destroy_fn val_destroy_fn)
{

    if (node->left == NULL)
    {
        Node *r = node->right;
        if (del)
            node_destroy(node, key_destroy_fn, val_destroy_fn);
        return r;
    }
    node->left = rec_delmin(node->left, del, key_destroy_fn, val_destroy_fn);
    return node;
}

void delmin(BinaryTree *bt)
{
    bt->root = rec_delmin(bt->root, true, bt->key_destroy_fn, bt->val_destroy_fn);
}

Node *rec_min(Node *node)
{
    if (node->left == NULL)
    {
        return node;
    }
    rec_min(node->left);
}

Node *rec_delete(Node *node, Key key, cmp_fn func, key_destroy_fn key_destroy_fn, val_destroy_fn val_destroy_fn)
{

    if (node == NULL)
        return NULL;
    int cmp_fn = func(key, node->key);
    if (cmp_fn < 0)
    {
        node->left = rec_delete(node->left, key, func, key_destroy_fn, val_destroy_fn);
    }
    else if (cmp_fn > 0)
    {
        node->right = rec_delete(node->right, key, func, key_destroy_fn, val_destroy_fn);
    }
    else
    {
        if (node->right == NULL)
        {
            Node *l = node->left;
            node_destroy(node, key_destroy_fn, val_destroy_fn);
            return l;
        }
        if (node->left == NULL)
        {
            Node *r = node->right;
            node_destroy(node, key_destroy_fn, val_destroy_fn);
            return r;
        }
        Node *t = node;
        node = rec_min(t->right);
        node->right = rec_delmin(t->right, false, key_destroy_fn, val_destroy_fn);
        node->left = t->left;
        node_destroy(t, key_destroy_fn, val_destroy_fn);
    }
    return node;
}

void bt_delete(BinaryTree *bt, Key key)
{
    bt->root = rec_delete(bt->root, key, bt->cmp_fn, bt->key_destroy_fn, bt->val_destroy_fn);
}

void bt_print(BinaryTree *bt, print_fn print_fn)
{
    Node *n = bt->root;
    node_print(n, print_fn);
    printf("\n");
}

int bt_height(BinaryTree *bt)
{
    if (bt->root == NULL)
    {
        return -1;
    }

    return node_height(bt->root);
}

void bt_destroy(BinaryTree *bt)
{

    rec_node_destroy(bt->root, bt->key_destroy_fn, bt->val_destroy_fn);
    free(bt);
}

// Função destrutora para Node* para uso em vector, stack, queue
void node_data_destroy(void *data) {
    // Não faz nada, pois os vetores/pilhas/filas não são donos dos nós
}

void preorder_recursive(Node *node, Vector *v) {
    if (node == NULL) return;
    vector_push_back(v, node->key); 
    preorder_recursive(node->left, v); 
    preorder_recursive(node->right, v); 
}

Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt) {
    Vector *v = vector_init(node_data_destroy, 1);
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
    Vector *v = vector_init(node_data_destroy, 1);

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
    Vector *v = vector_init(node_data_destroy, 1);
    if (bt != NULL && bt->root != NULL) {
        inorder_recursive(bt->root, v);
    }
    return v;
}

// Inorder traversal iterativo
Vector *binary_tree_inorder_traversal(BinaryTree *bt) {
    Vector *v = vector_init(node_data_destroy, 1);
    if (bt == NULL || bt->root == NULL) return v;

    Stack *s = stack_construct(node_data_destroy, 1);
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
    Vector *v = vector_init(node_data_destroy, 1);
    if (bt == NULL || bt->root == NULL) return v;

    Stack *s = stack_construct(node_data_destroy, 1);
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
    Vector *v = vector_init(node_data_destroy, 1);
    if (bt == NULL || bt->root == NULL) return v;

    Stack *s1 = stack_construct(node_data_destroy, 1);
    Stack *s2 = stack_construct(node_data_destroy, 1);
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
        return vector_init(node_data_destroy, 1); 
    }

    Vector *output = vector_init(node_data_destroy, 1); 
    Queue *queue = queue_construct(node_data_destroy, 1);

    queue_enqueue(queue, bt->root);

    while (!queue_empty(queue)) {
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