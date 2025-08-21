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
    int size; // Responsável por calcular o rank
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

    node->size = 1;

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
        // Destroi a chave, se a função para isso existir.
        if (key_destroy_fn != NULL)
        {
            key_destroy_fn(node->key);
        }

        // Destroi o valor, se a função para isso existir.
        if (val_destroy_fn != NULL)
        {
            val_destroy_fn(node->val);
        }
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
        // Destroi a chave, se a função para isso existir.
        if (key_destroy_fn != NULL)
        {
            key_destroy_fn(node->key);
        }

        // Destroi o valor, se a função para isso existir.
        if (val_destroy_fn != NULL)
        {
            val_destroy_fn(node->val);
        }
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
    print_fn(n->key);
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

static int size(Node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    else
    {
        return n->size;
    }
}

int bt_size(BinaryTree *bt)
{
    return size(bt->root);
}

Node *bt_rec_put(Node *node, Key key, Value val, cmp_fn func, key_destroy_fn key_destroy_fn, val_destroy_fn val_func)
{

    if (node == NULL)
    {
        return node_init(key, val);
    }

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

    node->size = size(node->left) + size(node->right) + 1;

    return node;
}

static int rec_rank(Node *n, Key key, cmp_fn cmp_fn)
{
    if (n == NULL)
    {
        return 0;
    }

    int cmp = cmp_fn(key, n->key);

    if (cmp < 0)
    {
        return rec_rank(n->left, key, cmp_fn);
    }
    else if (cmp > 0)
    {
        return 1 + size(n->left) + rec_rank(n->right, key, cmp_fn);
    }
    else
    {
        return size(n->left);
    }
}

int bt_rank(BinaryTree *bt, Key key)
{
    return rec_rank(bt->root, key, bt->cmp_fn);
}

void bt_put(BinaryTree *bt, Key key, Value val)
{
    bt->root = bt_rec_put(bt->root, key, val, bt->cmp_fn, bt->key_destroy_fn, bt->val_destroy_fn);
}

static Node *rec_floor(Node *n, Key key, cmp_fn cmp_fn)
{
    // Quando a árvore possui somente chaves maiores que a chave passada
    // Quando a árvore está vázia
    if (n == NULL)
    {
        return NULL;
    }

    int cmp = cmp_fn(key, n->key);

    // Se já tiver uma chave igual a chave passada, obviamente o floor vai ser igual a ele;
    if (cmp == 0)
    {
        return n;
    }

    // Se a chave for menor do que a chave atual, continua indo pra esquerda, estamos procurando chaves menores que a passada;
    if (cmp < 0)
    {
        return rec_floor(n->left, key, cmp_fn);
    }

    // Após finalmente encontrar uma chave menor, procura a maior das menores indo tudo para a direita;
    Node *temp = rec_floor(n->right, key, cmp_fn);

    // Se o filho da direita for diferente de nulo, continua indo para a direita;
    if (temp != NULL)
    {
        return temp;
    }
    // Se o filho da direita for nulo, retorna o nó atual;
    else
    {
        return n;
    }
}

Node *bt_floor(BinaryTree *bt, Key key)
{
    // Procura recursivamente a maior chave menor ou igual à chave passada;
    Node *n = rec_floor(bt->root, key, bt->cmp_fn);

    if (n == NULL)
    {
        return NULL;
    }
    else
    {
        return n;
    }
}

static Node *rec_ceiling(Node *n, Key key, cmp_fn cmp_fn)
{
    // Quando a árvore possui somente chaves menores que a chave passada
    // Quando a árvore está vázia
    if (n == NULL)
    {
        return NULL;
    }

    int cmp = cmp_fn(key, n->key);

    // Se já tiver uma chave igual a chave passada, obviamente o floor vai ser igual a ele;
    if (cmp == 0)
    {
        return n;
    }

    // Se a chave for maior do que a chave atual, continua indo pra direita, estamos procurando chaves maiores que a passada;
    if (cmp > 0)
    {
        return rec_ceiling(n->right, key, cmp_fn);
    }

    // Após finalmente encontrar uma chave maior, procura a menor das maiores indo tudo para a esquerda;
    Node *temp = rec_ceiling(n->left, key, cmp_fn);

    // Se o filho da esquerda for diferente de nulo, continua indo para a esquerda;
    if (temp != NULL)
    {
        return temp;
    }
    // Se o filho da esquerda for nulo, retorna o nó atual;
    else
    {
        return n;
    }
}

Node *bt_ceiling(BinaryTree *bt, Key key)
{
    // Procura recursivamente a menor chave maior ou igual à chave passada;
    Node *n = rec_ceiling(bt->root, key, bt->cmp_fn);

    if (n == NULL)
    {
        return NULL;
    }
    else
    {
        return n;
    }
}

Node *rec_delmin(Node *node, bool del, key_destroy_fn key_destroy_fn, val_destroy_fn val_destroy_fn)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (node->left == NULL)
    {
        Node *r = node->right;
        if (del)
            node_destroy(node, key_destroy_fn, val_destroy_fn);
        return r;
    }
    node->left = rec_delmin(node->left, del, key_destroy_fn, val_destroy_fn);
    node->size = size(node->left) + size(node->right) + 1;
    return node;
}

void bt_delmin(BinaryTree *bt)
{
    bt->root = rec_delmin(bt->root, true, bt->key_destroy_fn, bt->val_destroy_fn);
}

Node *rec_delmax(Node *node, bool del, key_destroy_fn key_destroy_fn, val_destroy_fn val_destroy_fn)
{

    if (node == NULL)
        return NULL;

    // Se já for o nó mais a direita, deleta ele e retorna o filho à esquerda dele, se ele tiver;
    if (node->right == NULL)
    {
        Node *l = node->left;

        if (del)
        {
            node_destroy(node, key_destroy_fn, val_destroy_fn);
        }

        return l;
    }

    // Depois de ter deletado, o filho à esquerda do que foi deletado passa a ser o filho da direita do pai dele;
    node->right = rec_delmax(node->right, del, key_destroy_fn, val_destroy_fn);
    node->size = size(node->left) + size(node->right) + 1;

    return node;
}

void bt_delmax(BinaryTree *bt)
{
    bt->root = rec_delmax(bt->root, true, bt->key_destroy_fn, bt->val_destroy_fn);
}

Node *rec_min(Node *node)
{
    if (node->left == NULL)
    {
        return node;
    }
    return rec_min(node->left);
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
    if (node != NULL)
    {
        node->size = size(node->left) + size(node->right) + 1;
    }
    return node;
}

void bt_delete(BinaryTree *bt, Key key)
{
    bt->root = rec_delete(bt->root, key, bt->cmp_fn, bt->key_destroy_fn, bt->val_destroy_fn);
}

static void rec_sideways_print(Node *node, print_fn print_fn, int level) {
    if (node == NULL) {
        return;
    }

    // 1. Visita a sub-árvore da direita primeiro (aparecerá no topo)
    rec_sideways_print(node->right, print_fn, level + 1);

    // 2. Imprime o nó atual com a indentação correta
    for (int i = 0; i < level; i++) {
        printf("    "); // Adiciona 4 espaços por nível de profundidade
    }
    print_fn(node->key); // Usa a função de impressão fornecida
    printf("\n");

    // 3. Visita a sub-árvore da esquerda (aparecerá embaixo)
    rec_sideways_print(node->left, print_fn, level + 1);
}

/**
 * @brief Função pública que inicia o processo de impressão da árvore.
 *
 * @param bt A árvore a ser impressa.
 * @param print_fn A função para imprimir a chave de cada nó.
 */
void bt_print(BinaryTree *bt, print_fn print_fn) {
    printf("----------------------------------\n");
    if (bt->root == NULL) {
        printf("(Árvore Vazia)\n");
    } else {
        // Inicia a recursão a partir da raiz, no nível 0
        rec_sideways_print(bt->root, print_fn, 0);
    }
    printf("----------------------------------\n");
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
void node_data_destroy(void *data)
{
    // Não faz nada, pois os vetores/pilhas/filas não são donos dos nós
}

void preorder_recursive(Node *node, Vector *v)
{
    if (node == NULL)
        return;
    vector_push_back(v, node->key);
    preorder_recursive(node->left, v);
    preorder_recursive(node->right, v);
}

Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt)
{
    Vector *v = vector_init(node_data_destroy, 1);
    if (bt != NULL && bt->root != NULL)
    {
        preorder_recursive(bt->root, v);
    }
    return v;
}

void postorder_recursive(Node *node, Vector *v)
{
    if (node == NULL)
        return;
    postorder_recursive(node->left, v);
    postorder_recursive(node->right, v);
    vector_push_back(v, node->key);
}

Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt)
{
    Vector *v = vector_init(node_data_destroy, 1);

    if (bt != NULL && bt->root != NULL)
    {
        postorder_recursive(bt->root, v);
    }

    return v;
}

void inorder_recursive(Node *node, Vector *v)
{
    if (node == NULL)
        return;
    inorder_recursive(node->left, v);
    vector_push_back(v, node->key);
    inorder_recursive(node->right, v);
}

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt)
{
    Vector *v = vector_init(node_data_destroy, 1);
    if (bt != NULL && bt->root != NULL)
    {
        inorder_recursive(bt->root, v);
    }
    return v;
}

// Inorder traversal iterativo
Vector *binary_tree_inorder_traversal(BinaryTree *bt)
{
    Vector *v = vector_init(node_data_destroy, 1);
    if (bt == NULL || bt->root == NULL)
        return v;

    Stack *s = stack_construct(node_data_destroy, 1);
    Node *current = bt->root;

    while (current != NULL || !stack_empty(s))
    {
        // Vai até o nó mais à esquerda possível
        while (current != NULL)
        {
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
Vector *binary_tree_preorder_traversal(BinaryTree *bt)
{
    Vector *v = vector_init(node_data_destroy, 1);
    if (bt == NULL || bt->root == NULL)
        return v;

    Stack *s = stack_construct(node_data_destroy, 1);
    stack_push(s, bt->root);

    while (!stack_empty(s))
    {
        Node *current = stack_pop(s);
        vector_push_back(v, current->key);

        // Empilha primeiro o filho direito para ser processado depois do esquerdo
        if (current->right != NULL)
        {
            stack_push(s, current->right);
        }
        if (current->left != NULL)
        {
            stack_push(s, current->left);
        }
    }

    stack_destroy(s);
    return v;
}

// Postorder traversal iterativo (usando duas pilhas)
Vector *binary_tree_postorder_traversal(BinaryTree *bt)
{
    Vector *v = vector_init(node_data_destroy, 1);
    if (bt == NULL || bt->root == NULL)
        return v;

    Stack *s1 = stack_construct(node_data_destroy, 1);
    Stack *s2 = stack_construct(node_data_destroy, 1);
    stack_push(s1, bt->root);

    while (!stack_empty(s1))
    {
        Node *current = stack_pop(s1);
        stack_push(s2, current);

        // Empilha os filhos esquerdo e direito na primeira pilha
        if (current->left != NULL)
        {
            stack_push(s1, current->left);
        }
        if (current->right != NULL)
        {
            stack_push(s1, current->right);
        }
    }

    // A segunda pilha agora contém os nós em ordem postorder reversa
    while (!stack_empty(s2))
    {
        Node *node = stack_pop(s2);
        vector_push_back(v, node->key);
    }

    stack_destroy(s1);
    stack_destroy(s2);
    return v;
}

Vector *binary_tree_levelorder_traversal(BinaryTree *bt)
{
    if (bt == NULL || bt->root == NULL)
    {
        return vector_init(node_data_destroy, 1);
    }

    Vector *output = vector_init(node_data_destroy, 1);
    Queue *queue = queue_construct(node_data_destroy, 1);

    queue_enqueue(queue, bt->root);

    while (!queue_empty(queue))
    {
        Node *current = queue_dequeue(queue);

        vector_push_back(output, current->key);

        if (current->left != NULL)
        {
            queue_enqueue(queue, current->left);
        }
        if (current->right != NULL)
        {
            queue_enqueue(queue, current->right);
        }
    }

    queue_destroy(queue);

    return output;
}

Key bt_node_get_key(Node *n)
{
    if (n == NULL)
    {
        return NULL;
    }
    return n->key;
}

Value bt_node_get_val(Node *n)
{
    if (n == NULL)
    {
        return NULL;
    }
    return n->val;
}
