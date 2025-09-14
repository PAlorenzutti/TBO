#pragma once

#include "stdio.h"

/* TYPES */

typedef struct ESElement ESElement;

/* FUNCTION POINTERS TYPES */

typedef void (*SerializeFunc)(void *control, void *data, FILE *output);

typedef int (*DesserializeFunc)(void *control, void *out, FILE *input);

typedef void (*FreeFunc)(void *control, void *data);

/* ES ELEMENT HEADERS */

ESElement *ESElement_build(DesserializeFunc desserialize, SerializeFunc serialize, 
                            FreeFunc free, void *control);

void ESElement_serialize(ESElement *ss, void *data, FILE *output);

int ESElement_desserialize(ESElement *ss, void *out, FILE *input);

void ESElement_data_free(ESElement *ss, void *data);

void ESElement_free(ESElement *ss);