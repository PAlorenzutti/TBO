#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "rbt.h"

#define RED true
#define BLACK false

typedef struct Node
{
    Key key;
    Value val;
    bool color;
    int size; // número de nós na subárvore
    struct Node *left, *right;
} Node;

struct RBT
{
    int size;
    cmp_func cmp_fn;
    key_destroy key_destroy_fn;
    val_destroy val_destroy_fn;
    Node *root;
};

// NODE FUNCTIONS
Node *rbt_node_init(Key key, Value val)
{

    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->val = val;
    node->color = RED;
    node->size = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void rbt_rec_node_destroy(Node *node, key_destroy key_destroy_fn, val_destroy val_destroy_fn)
{

    if (node == NULL)
    {
        return;
    }
    if (key_destroy_fn == val_destroy_fn)
    {
        key_destroy_fn(node->key);
        // Não chamar val_destroy_fn para evitar double free
    }
    else
    {
        key_destroy_fn(node->key);
        val_destroy_fn(node->val);
    }
    rbt_rec_node_destroy(node->left, key_destroy_fn, val_destroy_fn);
    rbt_rec_node_destroy(node->right, key_destroy_fn, val_destroy_fn);
    free(node);
}

static int node_size(Node *node)
{
    if (node == NULL)
        return 0;
    return node->size;
}

bool is_red(Node *node)
{
    if (node == NULL)
        return BLACK;
    return node->color; //  RED == true
}

Node *rotate_left(Node *h)
{
    Node *x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = x->left->color;
    x->left->color = RED;
    x->size = h->size;
    h->size = 1 + node_size(h->left) + node_size(h->right);
    return x;
}

Node *rotate_right(Node *h)
{
    Node *x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = x->right->color;
    x->right->color = RED;
    x->size = h->size;
    h->size = 1 + node_size(h->left) + node_size(h->right);
    return x;
}

void flip_colors(Node *h)
{
    h->color = RED;
    h->left->color = BLACK;
    h->right->color = BLACK;
}
// #############################################################################################################################

// RBT FUNCTIONS
RBT *rbt_init(cmp_func cmp_fn, key_destroy key_destroy_fn, val_destroy val_destroy_fn)
{

    RBT *rbt = (RBT *)malloc(sizeof(RBT));

    rbt->root = NULL;
    rbt->cmp_fn = cmp_fn;
    rbt->key_destroy_fn = key_destroy_fn;
    rbt->val_destroy_fn = val_destroy_fn;
    rbt->size = 0;
    return rbt;
}

Value rec_rbt_get(Node *node, Key key, cmp_func cmp_fn)
{

    if (node == NULL)
        return NULL;
    int cmp = cmp_fn(key, node->key);
    if (cmp < 0)
    {
        return rec_rbt_get(node->left, key, cmp_fn);
    }
    else if (cmp > 0)
    {
        return rec_rbt_get(node->right, key, cmp_fn);
    }
    else
    {
        return node->val;
    }
}

Value rbt_get(RBT *rbt, Key key)
{
    return rec_rbt_get(rbt->root, key, rbt->cmp_fn);
}

Node *rec_rbt_insert(Node *h, Key key, Value val, cmp_func cmp_fn, RBT *rbt)
{

    if (h == NULL)
    {
        rbt->size++;
        return rbt_node_init(key, val);
    }
    int cmp = cmp_fn(key, h->key);
    if (cmp < 0)
    {
        h->left = rec_rbt_insert(h->left, key, val, cmp_fn, rbt);
    }
    else if (cmp > 0)
    {
        h->right = rec_rbt_insert(h->right, key, val, cmp_fn, rbt);
    }
    else
    {
        h->val = val;
    }

    if (is_red(h->right) && !is_red(h->left))
    {
        h = rotate_left(h);
    }
    if (is_red(h->left) && is_red(h->left->left))
    {
        h = rotate_right(h);
    }
    if (is_red(h->left) && is_red(h->right))
    {
        flip_colors(h);
    }

    h->size = 1 + node_size(h->left) + node_size(h->right);
    return h;
}

void rbt_insert(RBT *rbt, Key key, Value val)
{
    rbt->root = rec_rbt_insert(rbt->root, key, val, rbt->cmp_fn, rbt);
}

void rbt_destroy(void *rbt_ptr)
{
    RBT *rbt = (RBT *)rbt_ptr;

    rbt_rec_node_destroy(rbt->root, rbt->key_destroy_fn, rbt->val_destroy_fn);
    free(rbt);
}

// RANK
static int rec_rbt_rank(Node *node, Key key, cmp_func cmp_fn)
{
    if (node == NULL)
        return 0;
    int cmp = cmp_fn(key, node->key);
    if (cmp < 0)
    {
        return rec_rbt_rank(node->left, key, cmp_fn);
    }
    else if (cmp > 0)
    {
        return 1 + node_size(node->left) + rec_rbt_rank(node->right, key, cmp_fn);
    }
    else
    {
        return node_size(node->left);
    }
}

int rbt_rank(RBT *rbt, Key key)
{
    return rec_rbt_rank(rbt->root, key, rbt->cmp_fn);
}

// FLOOR
static Node *rec_rbt_floor(Node *node, Key key, cmp_func cmp_fn)
{
    if (node == NULL)
        return NULL;
    int cmp = cmp_fn(key, node->key);
    if (cmp == 0)
        return node;
    if (cmp < 0)
        return rec_rbt_floor(node->left, key, cmp_fn);
    Node *t = rec_rbt_floor(node->right, key, cmp_fn);
    if (t != NULL)
        return t;
    else
        return node;
}

Key rbt_floor(RBT *rbt, Key key)
{
    Node *node = rec_rbt_floor(rbt->root, key, rbt->cmp_fn);
    if (node == NULL)
        return NULL;
    return node->key;
}

// CEILING
static Node *rec_rbt_ceiling(Node *node, Key key, cmp_func cmp_fn)
{
    if (node == NULL)
        return NULL;
    int cmp = cmp_fn(key, node->key);
    if (cmp == 0)
        return node;
    if (cmp > 0)
        return rec_rbt_ceiling(node->right, key, cmp_fn);
    Node *t = rec_rbt_ceiling(node->left, key, cmp_fn);
    if (t != NULL)
        return t;
    else
        return node;
}

Key rbt_ceiling(RBT *rbt, Key key)
{
    Node *node = rec_rbt_ceiling(rbt->root, key, rbt->cmp_fn);
    if (node == NULL)
        return NULL;
    return node->key;
}

// DELETION HELPERS
static Node *balance(Node *h)
{
    if (is_red(h->right))
        h = rotate_left(h);
    if (is_red(h->left) && is_red(h->left->left))
        h = rotate_right(h);
    if (is_red(h->left) && is_red(h->right))
        flip_colors(h);

    h->size = node_size(h->left) + node_size(h->right) + 1;
    return h;
}

static Node *move_red_left(Node *h)
{
    flip_colors(h);
    if (is_red(h->right->left))
    {
        h->right = rotate_right(h->right);
        h = rotate_left(h);
        flip_colors(h);
    }
    return h;
}

static Node *move_red_right(Node *h)
{
    flip_colors(h);
    if (is_red(h->left->left))
    {
        h = rotate_right(h);
        flip_colors(h);
    }
    return h;
}

static Node *rec_min(Node *node)
{
    if (node->left == NULL)
        return node;
    return rec_min(node->left);
}

// DELMIN
static Node *rec_rbt_delmin(Node *h, RBT *rbt)
{
    if (h->left == NULL)
    {
        rbt->key_destroy_fn(h->key);
        rbt->val_destroy_fn(h->val);
        free(h);
        rbt->size--;
        return NULL;
    }
    if (!is_red(h->left) && !is_red(h->left->left))
        h = move_red_left(h);
    h->left = rec_rbt_delmin(h->left, rbt);
    return balance(h);
}

void rbt_delmin(RBT *rbt)
{
    if (rbt->root == NULL)
        return;
    rbt->root = rec_rbt_delmin(rbt->root, rbt);
    if (rbt->root != NULL)
        rbt->root->color = BLACK;
}

// DELMAX
static Node *rec_rbt_delmax(Node *h, RBT *rbt)
{
    if (is_red(h->left))
        h = rotate_right(h);
    if (h->right == NULL)
    {
        rbt->key_destroy_fn(h->key);
        rbt->val_destroy_fn(h->val);
        free(h);
        rbt->size--;
        return NULL;
    }
    if (!is_red(h->right) && !is_red(h->right->left))
        h = move_red_right(h);
    h->right = rec_rbt_delmax(h->right, rbt);
    return balance(h);
}

void rbt_delmax(RBT *rbt)
{
    if (rbt->root == NULL)
        return;
    rbt->root = rec_rbt_delmax(rbt->root, rbt);
    if (rbt->root != NULL)
        rbt->root->color = BLACK;
}

// DELETE
static Node *rec_rbt_delete(Node *h, Key key, cmp_func cmp_fn, RBT *rbt)
{
    if (h == NULL)
        return NULL;

    if (cmp_fn(key, h->key) < 0)
    {
        if (!is_red(h->left) && !is_red(h->left->left))
            h = move_red_left(h);
        h->left = rec_rbt_delete(h->left, key, cmp_fn, rbt);
    }
    else
    {
        if (is_red(h->left))
            h = rotate_right(h);
        if (cmp_fn(key, h->key) == 0 && (h->right == NULL))
        {
            rbt->key_destroy_fn(h->key);
            rbt->val_destroy_fn(h->val);
            free(h);
            rbt->size--;
            return NULL;
        }
        if (!is_red(h->right) && !is_red(h->right->left))
            h = move_red_right(h);
        if (cmp_fn(key, h->key) == 0)
        {
            Node *x = rec_min(h->right);
            rbt->key_destroy_fn(h->key);
            rbt->val_destroy_fn(h->val);
            h->key = x->key;
            h->val = x->val;
            h->right = rec_rbt_delmin(h->right, rbt);
        }
        else
        {
            h->right = rec_rbt_delete(h->right, key, cmp_fn, rbt);
        }
    }
    return balance(h);
}

void rbt_delete(RBT *rbt, Key key)
{
    if (rbt_get(rbt, key) == NULL)
        return;
    rbt->root = rec_rbt_delete(rbt->root, key, rbt->cmp_fn, rbt);
    if (rbt->root != NULL)
        rbt->root->color = BLACK;
}

// SELECT
static Node *rec_rbt_select(Node *node, int k)
{
    // Se chegou a um nó nulo, significa que não existe k chave menor na árvore;
    if (node == NULL)
        return NULL;

    // Calcula o número de elementos na subárvore da esquerda (chaves menores) contando com a raiz;
    int t = node_size(node->left);

    // Se o número de chaves na subárvore da esquerda é maior que k, continua indo para esquerda;
    if (t > k)
    {
        return rec_rbt_select(node->left, k);
    }
    

    // Se o número de chaves na subárvore da esquerda é menor que k, então a chave está na subárvore da direita;
    // É excluído a raiz da subárvore atual e os elementos da esquerda na busca;
    else if (t < k)
    {
        return rec_rbt_select(node->right, k - t - 1);
    }

    // Se t for igual a k, então significa que restam apenas k elementos menores, e significa que aquele elemento é o k-ésimo menor
    else
    {
        return node;
    }
}

Key rbt_select(RBT *rbt, int k)
{
    // k é baseado em 1, então o k-ésimo elemento está no índice k-1.
    // A verificação agora é se k está entre 1 e o tamanho da árvore.
    if (k < 1 || k > rbt_size(rbt))
        return NULL;
    Node *node = rec_rbt_select(rbt->root, k - 1);

    printf("\n--- Justificativa Teórica ---\n");
    printf("Se a árvore T não fosse uma LLRBT, mas apenas uma árvore binária de busca comum, seria possível ter o mesmo algoritmo com garantia de tempo O(log(n))?\n\n");
    printf("Não. O algoritmo `rbt_select` depende fundamentalmente da altura da árvore para sua complexidade. Em uma LLRBT, a altura é garantidamente logarítmica (O(log n)) em relação ao número de nós 'n', devido às propriedades de balanceamento. Em cada passo, o algoritmo desce um nível na árvore, resultando em um tempo de busca proporcional à altura.\n\n");
    printf("Em uma árvore binária de busca comum (BST), não há garantia de balanceamento. No pior caso (por exemplo, se os elementos forem inseridos em ordem crescente ou decrescente), a árvore pode degenerar em uma lista ligada, com altura O(n). Nesse cenário, a busca pelo k-ésimo elemento também levaria tempo O(n), pois o algoritmo percorreria um caminho de comprimento 'n' na árvore.\n\n");
    printf("Portanto, a garantia de complexidade O(log n) para a operação `select` é uma consequência direta do balanceamento mantido pela LLRBT.\n");

    return node->key;
}

// DEPURAÇÃO
void rbt_print_inorder(Node *node, key_print key_print_fn, val_print val_print_fn)
{
    if (node == NULL)
        return;
    rbt_print_inorder(node->left, key_print_fn, val_print_fn);
    key_print_fn(node->key);
    val_print_fn(node->val);
    rbt_print_inorder(node->right, key_print_fn, val_print_fn);
}

void rbt_print(RBT *rbt, key_print key_print_fn, val_print val_print_fn)
{
    rbt_print_inorder(rbt->root, key_print_fn, val_print_fn);
}

int rbt_size(RBT *rbt)
{
    return rbt->size;
}

void rbt_vector_inorder(Node *node, Vector *vector)
{
    if (node == NULL)
        return;
    rbt_vector_inorder(node->left, vector);
    vector_insert(vector, node->val);
    rbt_vector_inorder(node->right, vector);
}

Vector *rbt_to_vector(RBT *rbt)
{
    Vector *vector = vector_init(do_nothing, rbt->size);
    rbt_vector_inorder(rbt->root, vector);
    return vector;
}