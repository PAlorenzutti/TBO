#pragma once

#include "../../DataStructures/Vector.h"

/* TYPES */

typedef struct Intercalator Intercalator;

/* FUNCTION POINTERS TYPES */

typedef void (*ReceiveDataFunc)(void* data, void* control);
typedef bool (*AquireDataFunc)(void *writePos, int pageIdx, void* control);

/* INTERCALATOR HEADERS */

Intercalator* ITR_initialize(int pageAmt, int elemSize, void *intercalatorControl,
                                AquireDataFunc aquire_data, ReceiveDataFunc receive_batch, CmpDataFunc compare);
                            
void ITR_intercalate(Intercalator *itr);

void ITR_reset(Intercalator *itr);

void ITR_free(Intercalator *itr);


