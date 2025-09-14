#include "Vector.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* VECTOR PRIVATE */

/*Contiguous Vector*/
struct Vector
{
    size_t size;  
    size_t allocatedSize;
    size_t elemSize;
    DataType* data;
};


/* VECTOR PUBLIC */


Vector* Vector_build(size_t elemSize)
{
    Vector* v = calloc(1, sizeof(Vector));
    v->elemSize = elemSize;
    return v;
}


void Vector_push_back(Vector* v, DataType* data)
{
    char *dataPtr = ((char *)v->data) + v->size * v->elemSize;
    memcpy(dataPtr, data, v->elemSize);
    v->size++;
}


void Vector_sort(Vector* v, CmpDataFunc compare)
{
    qsort(v->data, v->size, v->elemSize, compare);
}


DataType* Vector_get(Vector* v, int idx)
{
    return ((char*)v->data) + idx * v->elemSize;
}


/*Useful to write directly in the vector last position*/
DataType* Vector_get_last_position(Vector* v)
{
    return Vector_get(v, v->size++);
}


size_t Vector_size(Vector* v)
{
    return v->size;
}

size_t Vector_allocated_amt(Vector* v)
{
    return v->allocatedSize;
}

void Vector_set_size(Vector *v, size_t size)
{
    v->size = size;
}

void Vector_allocate(Vector* v, int amt)
{
    v->data = realloc(v->data, v->elemSize *  amt);
    v->allocatedSize = amt;
}


void Vector_reset(Vector* v)
{
    v->size = 0;
}


DataType *Vector_get_data(Vector *v)
{
    return v->data;
}

void Vector_free(Vector* v)
{
    free(v->data);
    free(v);
}