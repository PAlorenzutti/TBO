#ifndef RBT_H
#define RBT_H

#include "vector.h"

typedef int (*cmp_func)(const void *, const void *);

typedef void (*key_print)(const void *);
typedef void (*val_print)(const void *);

typedef void (*key_destroy)(void *);
typedef void (*val_destroy)(void *);


typedef void* Value;
typedef void* Key;
typedef struct RBT RBT;

RBT *rbt_init(cmp_func cmp, key_destroy key_des, val_destroy val_des);

Value rbt_get(RBT *rbt, Key key);

void rbt_insert(RBT *rbt, Key key, Value val);

void rbt_destroy(void *rbt_ptr);

void rbt_print(RBT *rbt, key_print key_print_func, val_print val_print_func);

int rbt_size(RBT* rbt);

Vector* rbt_to_vector(RBT* rbt);

// Funções a serem implementadas
int rbt_rank(RBT *rbt, Key key);
Key rbt_floor(RBT *rbt, Key key);
Key rbt_ceiling(RBT *rbt, Key key);
void rbt_delete(RBT *rbt, Key key);
void rbt_delmin(RBT *rbt);
void rbt_delmax(RBT *rbt);
Key rbt_select(RBT *rbt, int k);

#endif