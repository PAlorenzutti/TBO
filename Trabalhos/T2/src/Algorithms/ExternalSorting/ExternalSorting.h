#pragma once

#include "../../DataStructures/Vector.h"
#include "ESElement.h"
#include <stdio.h>


#define EXP_START_GROUP 1

/* TYPES */

typedef struct ExternalSortingProfile ExternalSortingProfile;

/* INTERCALATOR HEADERS */

ExternalSortingProfile *EXP_initialize_sorter(FILE* inputFile, int P, int M, ESElement *serializer,
                                              CmpDataFunc compareFunc, size_t elemSize);

void EXP_sort(ExternalSortingProfile *profile, char *outName);

void EXP_free(ExternalSortingProfile *profile);

int EXP_get_P(ExternalSortingProfile *profile);

int EXP_get_M(ExternalSortingProfile *profile);

FILE *EXP_get_file(ExternalSortingProfile *profile, int group, int idx);

size_t EXP_get_elem_size(ExternalSortingProfile *profile);

int EXP_get_total_elements(ExternalSortingProfile *profile);

ESElement *EXP_get_serializer(ExternalSortingProfile *profile);