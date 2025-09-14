#include "ESIntercalatorControl.h"

#include "ESElement.h"
#include <stdio.h>


/* ES INTERCALATOR CONTROL PRIVATE */


struct ESIntercalatorControl
{
    int currentDiskGroup, currentBlockSize, currentOutputIdx, currentBlock;
    int totalWrittenElements;
    long *elementsReadPerFile;
    ExternalSortingProfile *profile;
};


static void ESintercalatorControl_reset_files(ESIntercalatorControl *ctr)
{
    int P = EXP_get_P(ctr->profile);
    for(int i = 0; i<2*P; i++)
    {
        int currentGroup = i/P;
        int currentIdx = i%P;

        FILE *file = EXP_get_file(ctr->profile, currentGroup, currentIdx);
        fflush(file);

        int currentOutputGroup = (ctr->currentDiskGroup+1)%2;
        if(currentGroup == currentOutputGroup) //Reset files for writing
            freopen(NULL, "wb+", file);

        rewind(file);
    }
}


/* ES INTERCALATOR CONTROL PUBLIC */


ESIntercalatorControl *ESintercalatorControl_build(ExternalSortingProfile *profile)
{
    ESIntercalatorControl *ctr = malloc(sizeof(ESIntercalatorControl));
    ctr->currentBlock = 0;
    ctr->currentBlockSize = EXP_get_M(profile);
    ctr->currentDiskGroup = EXP_START_GROUP;
    ctr->currentOutputIdx = 0;
    ctr->profile = profile;
    ctr->totalWrittenElements = 0;
    ctr->elementsReadPerFile = calloc(EXP_get_P(ctr->profile), sizeof(long));
    ESintercalatorControl_reset_files(ctr);

    return ctr;
}


void ESIntercalatorControl_disk_group_swap(ESIntercalatorControl *ctr)
{
    ctr->totalWrittenElements = 0;
    ctr->currentBlockSize *= EXP_get_P(ctr->profile);
    ctr->currentDiskGroup = (ctr->currentDiskGroup+1) % 2;
    ctr->currentBlock = 0;
    ctr->currentOutputIdx = 0;
    for(int i = 0; i<EXP_get_P(ctr->profile); i++)
        ctr->elementsReadPerFile[i] = 0;
    ESintercalatorControl_reset_files(ctr);
}


void ESIntercalatorControl_block_swap(ESIntercalatorControl *ctr)
{
    ctr->currentBlock++;
    ctr->currentOutputIdx = (ctr->currentOutputIdx + 1) % EXP_get_P(ctr->profile);
}


bool ESIntercalatorControl_aquire_page_data(void *dataOut, int pageIdx, void *controlVoid)
{
    ESIntercalatorControl *control = controlVoid;
    FILE *file = EXP_get_file(control->profile, control->currentDiskGroup, pageIdx);
    if(feof(file)) return true;

    long fileElemPos = control->elementsReadPerFile[pageIdx];
    if(fileElemPos / control->currentBlockSize != control->currentBlock)
        return true; 

    ESElement *ss = EXP_get_serializer(control->profile);
    int stat = ESElement_desserialize(ss, dataOut, file);
    if(stat == -1) return true;

    control->elementsReadPerFile[pageIdx]++;
    
    return false;
}


void ESIntercalatorControl_receive_data(void *data, void *controlVoid)
{
    ESIntercalatorControl *control = controlVoid;
    control->totalWrittenElements++;

    int currentOutputGroup = (control->currentDiskGroup +1)%2;
    FILE *file = EXP_get_file(control->profile, currentOutputGroup, control->currentOutputIdx);
    ESElement *ss = EXP_get_serializer(control->profile);
    ESElement_serialize(ss, data, file);
    ESElement_data_free(ss, data);
}


bool ESIntercalatorControl_group_end(ESIntercalatorControl *ctr)
{
    return ctr->totalWrittenElements == EXP_get_total_elements(ctr->profile);
}


void ESIntercalatorControl_free(ESIntercalatorControl *ctr)
{
    free(ctr->elementsReadPerFile);
    free(ctr);
}


int ESIntercalatorControl_get_out_group(ESIntercalatorControl *ctr)
{
    return (ctr->currentDiskGroup+1)%2;
}


int ESIntercalatorControl_get_out_idx(ESIntercalatorControl *ctr)
{
    return ctr->currentOutputIdx;
}