#include "ProblemConfig.h"

#include "Vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* PROGRAM CONFIG PRIVATE*/


struct ProgramConfig
{
    int P;           
    int M;               
    Vector *keyIndexes[2];                
    FILE *files[3];    
};


static Vector* parse_indices(char *str) 
{
    Vector *indices = Vector_build(sizeof(int));
    Vector_allocate(indices, 10); // Alocar espaço inicial
    
    char *token = strtok(str, ",");
    while (token != NULL) 
    {
        if(Vector_allocated_amt(indices) == Vector_size(indices))
            Vector_allocate(indices, Vector_size(indices) + 10);
        
        int value = atoi(token);
        Vector_push_back(indices, &value);
        token = strtok(NULL, ",");
    }
    
    return indices;
}


/* PROGRAM CONFIG PUBLIC */


ProgramConfig *ProgramConfig_parse_arguments(int argc, char *argv[]) 
{
    
    ProgramConfig *pc = malloc(sizeof(ProgramConfig));
    
    if (argc != 8) 
    {
        printf("Uso: %s P M i1,i2,...,ik j1,j2,...,jk arquivo1 arquivo2 arquivo3\n", argv[0]);
        free(pc);
        exit(1);
    }
    
    pc->P = atoi(argv[1]);
    pc->M = atoi(argv[2]);
    
    char *tempI = strdup(argv[3]);  
    char *tempJ = strdup(argv[4]);
    
    pc->keyIndexes[0] = parse_indices(tempI);
    pc->keyIndexes[1] = parse_indices(tempJ);
    
    free(tempI);
    free(tempJ);
    
    pc->files[0] = fopen(argv[5], "r");
    pc->files[1] = fopen(argv[6], "r");
    pc->files[2] = fopen(argv[7], "w");
    
    if (! pc->files[0]|| ! pc->files[1]|| ! pc->files[2]) 
    {
        printf("Erro ao abrir arquivos!\n");
        exit(1);
    }
    
    return pc;
}


void ProgramConfig_free(ProgramConfig *pc)
{
    Vector_free(pc->keyIndexes[0]);
    Vector_free(pc->keyIndexes[1]);
    if (pc->files[0]) fclose(pc->files[0]);
    if (pc->files[1]) fclose(pc->files[1]);
    if (pc->files[2]) fclose(pc->files[2]);

    free(pc);
}


Vector **ProgramConfig_get_key_indexes(ProgramConfig *pc)
{
    return pc->keyIndexes;
}


FILE **ProgramConfig_get_files(ProgramConfig *pc)
{
    return pc->files;
}


int ProgramConfig_get_P(ProgramConfig *pc)
{
    return pc->P;
}


int ProgramConfig_get_M(ProgramConfig *pc)
{
    return pc->M;
}