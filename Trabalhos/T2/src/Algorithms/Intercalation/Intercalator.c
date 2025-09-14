#include "Intercalator.h"

#include "../../DataStructures/Vector.h"
#include <stdlib.h>


/* INTERCALATOR PRIVATE */


#define PAGE_DEAD -1


struct Intercalator 
{
    Vector *pages;
    int *status;
    int bodyCount;

    void *intercalatorControl;
    AquireDataFunc aquire_data;
    ReceiveDataFunc receive_data;
    CmpDataFunc compare;
};


static int ITR_get_smaller(Intercalator *itr, int pageA, int pageB)
{
    void *d1 = Vector_get(itr->pages, pageA);
    void *d2 = Vector_get(itr->pages, pageB);

    if(itr->compare(d1, d2) > 0) return pageB;
    return pageA;
}


static void ITR_aquire_data(Intercalator *itr, int pageIdx)
{
    void *writePos = Vector_get(itr->pages, pageIdx);
    bool dead = itr->aquire_data(writePos, pageIdx, itr->intercalatorControl);

    if(dead)
    {
        itr->bodyCount++;
        itr->status[pageIdx] = PAGE_DEAD;
    }
    
}


static void ITR_intercalate_iteration(Intercalator *itr)
{
    int smallerPageIdx = -1;
    for(int i = 0; i<Vector_size(itr->pages); i++)
    {
        if(itr->status[i] == PAGE_DEAD) continue;

        if(smallerPageIdx == -1) smallerPageIdx = i;
        else smallerPageIdx = ITR_get_smaller(itr, smallerPageIdx, i);
    }
    
    void *elem = Vector_get(itr->pages, smallerPageIdx);
    itr->receive_data(elem, itr->intercalatorControl);
    ITR_aquire_data(itr, smallerPageIdx);
}


/* INTERCALATOR PUBLIC */


Intercalator* ITR_initialize(int pageAmt, int elemSize, void *intercalatorControl,
                                    AquireDataFunc aquire_data, ReceiveDataFunc receive_batch, CmpDataFunc compare)
{
    Intercalator *itr = malloc(sizeof(Intercalator));
    itr->aquire_data = aquire_data; itr->receive_data = receive_batch;
    itr->compare = compare; 
    itr->bodyCount = 0; 
    itr->intercalatorControl = intercalatorControl;
    itr->status = calloc(pageAmt, sizeof(int));

    itr->pages = Vector_build(elemSize);
    Vector_allocate(itr->pages, pageAmt);

    for(int i = 0; i<pageAmt; i++)
        ITR_aquire_data(itr, i);
    Vector_set_size(itr->pages, pageAmt);

    return itr;
}


void ITR_intercalate(Intercalator *itr)
{
    while(itr->bodyCount != Vector_size(itr->pages))
        ITR_intercalate_iteration(itr);
}


void ITR_reset(Intercalator *itr)
{
    itr->bodyCount = 0;
    for(int i = 0; i<Vector_size(itr->pages); i++)
    {
        itr->status[i] = 0;
        ITR_aquire_data(itr, i);
    }
}


void ITR_free(Intercalator *itr)
{
    Vector_free(itr->pages);
    free(itr->status);
    free(itr);
}