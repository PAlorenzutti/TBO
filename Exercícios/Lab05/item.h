#ifndef _ITEM_H_
#define _ITEM_H_

#define CUTOFF 5

typedef int Item;

#define key(A) (A)
#define less(A, B) (key(A) < key(B))
#define exch(A, B) { Item t = A; A = B; B = t; }
#define compexch(A, B) if (less(B, A)) exch(A, B)

#define SZ2 (sz+sz)
#define MIN(X, Y) ((X < Y) ? (X) : (Y))

#endif 