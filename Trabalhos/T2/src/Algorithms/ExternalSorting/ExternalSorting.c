#include "ExternalSorting.h"

#include "../Intercalation/Intercalator.h"
#include "../../DataStructures/Vector.h"
#include "ESIntercalatorControl.h"
#include "ESElement.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* EXTERNAL SORTING PROFILE PRIVATE */


#define EXP_DATA_END 1
#define FILENAME_BUFFER_SIZE 20


struct ExternalSortingProfile
{
    int M, P;
    size_t totalElements;
    FILE *inputFile;
    Vector *disksVec;
    
    ESElement *serializer;
    void *compareData;
    CmpDataFunc compare;
    size_t elemSize; 
};


static int EXP_parse_data(ExternalSortingProfile *profile, Vector *output)
{
    if(feof(profile->inputFile))
        return EXP_DATA_END;
    
    // Write directly into preallocated vector to avoid copy;
    // update vector size only if deserialization succeeds.
    for(int i = 0; i<profile->M; i++)
    {
        void *out = Vector_get(output, i);
        int status = ESElement_desserialize(profile->serializer, out, profile->inputFile);
        if(status == -1) break;
        Vector_set_size(output, Vector_size(output) +1);
    }
    if(Vector_size(output) == 0) return EXP_DATA_END;
    return 0;
}


static void EXP_initialize_disk_vec(Vector *diskVec, int P)
{
    char fileName[FILENAME_BUFFER_SIZE];
    for(int i = 0; i<2*P; i++)
    {
        snprintf(fileName, FILENAME_BUFFER_SIZE, "%d.txt", i);
        FILE *f = fopen(fileName, "wb+");
        Vector_push_back(diskVec, &f);
    }
}


static bool EXP_initialize_blocks(ExternalSortingProfile *profile)
{
    int currentDiskIdx = 0;
    Vector *blocksVec = Vector_build(profile->elemSize);
    Vector_allocate(blocksVec, profile->M);

    int totalElements = 0;
    int itt = 0;

    /* Aquire data, sort, write to disk, repeat */
    while(EXP_parse_data(profile, blocksVec) != EXP_DATA_END)
    {
        itt++;
        FILE *currentDisk = EXP_get_file(profile, EXP_START_GROUP, currentDiskIdx);
        Vector_sort(blocksVec, profile->compare);
        
        for(int i = 0; i<Vector_size(blocksVec); i++)
        {
            void *data = Vector_get(blocksVec, i);
            ESElement_serialize(profile->serializer, data, currentDisk);
            ESElement_data_free(profile->serializer, data);
        }

        totalElements += Vector_size(blocksVec);
        currentDiskIdx = (currentDiskIdx+1)%profile->P;

        Vector_reset(blocksVec);
    }

    profile->totalElements = totalElements;
    Vector_free(blocksVec);

    return itt == 1; //If the initialization was made in 1 iteration, then it is already sorted
}


static int EXP_intercalate(ExternalSortingProfile *profile)
{
    ESIntercalatorControl *control;
    control = ESintercalatorControl_build(profile);
    
    Intercalator *itr = ITR_initialize
                                    (
                                        profile->P, profile->elemSize, control,
                                        ESIntercalatorControl_aquire_page_data,
                                        ESIntercalatorControl_receive_data, profile->compare
                                    );
                                            
    while(1)
    {     
        int itt = 0;
        while(1)
        {
            itt++;
            ITR_intercalate(itr);
            
            if(ESIntercalatorControl_group_end(control)) break;
            ESIntercalatorControl_block_swap(control);
            ITR_reset(itr);
        }
        
        if(itt == 1) break;
        ESIntercalatorControl_disk_group_swap(control);
        ITR_reset(itr);
    }

    int group = ESIntercalatorControl_get_out_group(control);
    int idx = ESIntercalatorControl_get_out_idx(control);
    
    ITR_free(itr);
    ESIntercalatorControl_free(control);
    return group * profile->P + idx;
}


/* EXTERNAL SORTING PROFILE PUBLIC */


ExternalSortingProfile *EXP_initialize_sorter(FILE* inputFile, int P, int M, ESElement *serializer,
                                              CmpDataFunc compareFunc, size_t elemSize)
{
    ExternalSortingProfile *esp = malloc(sizeof(ExternalSortingProfile));

    esp->P = P; esp->M = M; esp->elemSize = elemSize;
    esp->inputFile = inputFile; esp->totalElements = 0;
    esp->serializer = serializer;
    esp->disksVec = Vector_build(sizeof(FILE *));
    Vector_allocate(esp->disksVec, 2*P);

    esp->compare = compareFunc;

    return esp;
}


void EXP_sort(ExternalSortingProfile *profile, char *outName)
{
    int fileIdx, startFile = EXP_START_GROUP * profile->P;
    EXP_initialize_disk_vec(profile->disksVec, profile->P);

    if(EXP_initialize_blocks(profile))
        //sorted in main memory
        fileIdx = startFile;
    else
        fileIdx = EXP_intercalate(profile);

    for(int i = 0; i<Vector_size(profile->disksVec); i++)
        fclose(*(FILE**)Vector_get(profile->disksVec, i));

    char currentName[FILENAME_BUFFER_SIZE];
    snprintf(currentName, FILENAME_BUFFER_SIZE, "%d.txt", fileIdx);
    rename(currentName, outName);
}


void EXP_free(ExternalSortingProfile *profile)
{
    Vector_free(profile->disksVec);
    free(profile);
}


int EXP_get_P(ExternalSortingProfile *profile)
{
    return profile->P;
}


int EXP_get_M(ExternalSortingProfile *profile)
{
    return profile->M;
}


FILE *EXP_get_file(ExternalSortingProfile *profile, int group, int idx)
{
    return *(FILE **)Vector_get(profile->disksVec, profile->P * group + idx);
}


size_t EXP_get_elem_size(ExternalSortingProfile *profile)
{
    return profile->elemSize;
}


int EXP_get_total_elements(ExternalSortingProfile *profile)
{
    return profile->totalElements;
}

ESElement *EXP_get_serializer(ExternalSortingProfile *profile)
{
    return profile->serializer;
}