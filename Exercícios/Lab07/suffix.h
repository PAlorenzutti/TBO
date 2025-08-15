#ifndef SUFFIX_H
#define SUFFIX_H

#include "str.h"

typedef struct Suffix Suffix;


Suffix *suffix_init(String *s, int index);

String* suffix_get(const Suffix *sf);

int suffix_index(const Suffix *sf);

void suffix_print(const void *sf);

void suffix_destroy(void *sf);

int suffix_compare(const void *a, const void *b);

int suffix_prefix_compare(const void *query, const void *suffix);

#endif