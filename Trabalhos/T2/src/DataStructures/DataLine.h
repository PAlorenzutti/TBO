#pragma once

#include "Vector.h"
#include <stdio.h>

/* TYPES */

typedef struct DataLine DataLine;
typedef struct DataLineControl DataLineControl;

/* DATA LINE CONTROL HEADERS*/

DataLineControl *DataLineControl_build(Vector *keyIndexes, FILE *in);

void DataLineControl_free(DataLineControl *control);

int DataLineControl_get_field_amt(DataLineControl *control);

/* DATA LINE HEADERS*/

void DataLine_build(DataLineControl *control, DataLine *out);

int DataLine_desserialize(void *controlVoid, void *outVoid, FILE *input);

void DataLine_serialize(void *controlVoid, void *dataVoid, FILE *out);

void DataLine_free(void *controlVoid, void *dataVoid);

int DataLine_compare_unique(const void* v1, const void *v2);

int DataLine_compare(int keyAmt, DataLine *d1, DataLine *d2);

char **DataLine_get_fields(DataLine *d);

char **DataLine_get_keys(DataLine *d);

int DataLine_size();