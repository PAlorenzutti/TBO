#pragma once

#include "ExternalSorting.h"

/* TYPES */

typedef struct ESIntercalatorControl ESIntercalatorControl;

/* ES INTERCALATOR CONTROL HEADERS*/

ESIntercalatorControl *ESintercalatorControl_build(ExternalSortingProfile *profile);

void ESIntercalatorControl_disk_group_swap(ESIntercalatorControl *ctr);

void ESIntercalatorControl_block_swap(ESIntercalatorControl *ctr);

bool ESIntercalatorControl_group_end(ESIntercalatorControl *ctr);

bool ESIntercalatorControl_aquire_page_data(void *dataOut, int pageIdx, void *controlVoid);

void ESIntercalatorControl_receive_data(void *data, void *controlVoid);
    
void ESIntercalatorControl_free(ESIntercalatorControl *ctr);

int ESIntercalatorControl_get_page_size(ESIntercalatorControl *ctr);

int ESIntercalatorControl_get_out_group(ESIntercalatorControl *ctr);

int ESIntercalatorControl_get_out_idx(ESIntercalatorControl *ctr);