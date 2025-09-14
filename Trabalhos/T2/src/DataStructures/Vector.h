#pragma once

#include "../Utils/GenericTypeNames.h"
#include "stdlib.h"

/* TYPES */

typedef struct Vector Vector;

/* FUNCTION POINTERS TYPES */

typedef int (*CmpDataFunc)(const DataType* d1, const DataType* d2);

/* VECTOR HEADERS */

Vector* Vector_build(unsigned long elemSize);

void Vector_push_back(Vector* v, DataType* data);

void Vector_sort(Vector* v, CmpDataFunc compare);

DataType* Vector_get(Vector* v, int idx);

size_t Vector_size(Vector* v);

size_t Vector_allocated_amt(Vector* v);

void Vector_set_size(Vector *v, size_t size);

void Vector_allocate(Vector* v, int amt);

void Vector_reset(Vector* v);

DataType *Vector_get_data(Vector *v);

/*For perfomance, provides the next writable space and increases the size of the vector*/
DataType* Vector_get_last_position(Vector* v);

void Vector_free(Vector* v);