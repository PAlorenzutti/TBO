#include "DataStructures/ProblemConfig.h"
#include "DataStructures/DataLine.h"
#include "DataStructures/Vector.h"
#include "Algorithms/ExternalSorting/ExternalSorting.h"
#include "Algorithms/ExternalSorting/ESElement.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void sort_file(ProgramConfig *pc, int fileId, char *outName, DataLineControl *control);

void merge_sorted_files(ProgramConfig *pc, char **filenames, DataLineControl **controls);

int main(int argc, char *argv[]) {
    clock_t start = clock();
    ProgramConfig *pc = ProgramConfig_parse_arguments(argc, argv);

    char *fileNames[] = {"ordenado1.txt", "ordenado2.txt"};
    DataLineControl *controls[2];

    for(int i = 0; i<2; i++)
    {
        controls[i] = DataLineControl_build
                                  (
                                    ProgramConfig_get_key_indexes(pc)[i], 
                                    ProgramConfig_get_files(pc)[i]
                                  );
        sort_file(pc, i, fileNames[i], controls[i]);
    }

    merge_sorted_files(pc, fileNames, controls);

    ProgramConfig_free(pc);
    DataLineControl_free(controls[0]);
    DataLineControl_free(controls[1]);
    return 0;
}


void sort_file(ProgramConfig *pc, int fileId, char *outName, DataLineControl *control)
{                           
    ESElement *ss = ESElement_build
                                    (
                                        DataLine_desserialize, DataLine_serialize, 
                                        DataLine_free, control
                                    );

    FILE *inputFile = ProgramConfig_get_files(pc)[fileId];
    int P = ProgramConfig_get_P(pc), M = ProgramConfig_get_M(pc);
    ExternalSortingProfile *profile = EXP_initialize_sorter(
        inputFile, P, M, ss, DataLine_compare_unique, DataLine_size()
    );
    EXP_sort(profile, outName);
    EXP_free(profile);
    ESElement_free(ss);
}

// funcao para escrever linha de juncao
void write_joined_line(FILE *outputFile, DataLine *d1, DataLine *d2, int keyAmt, DataLineControl **controls) 
{
    // 1. escreve as chaves (do arquivo 1)
    char **d1Keys = DataLine_get_keys(d1);
    for (int i = 0; i < keyAmt; i++) {
        fputs(d1Keys[i], outputFile);
        if (i != keyAmt-1) fputc(',', outputFile);
    }
    
    // 2. escreve os campos nao-chave do arquivo 1
    char **d1Fields = DataLine_get_fields(d1);
    int numFields1 = DataLineControl_get_field_amt(controls[0]);
    for (int i = 0; i < numFields1; i++) {
        fputc(',', outputFile);
        fputs(d1Fields[i], outputFile);
    }
    
    // 3. escreve os campos nao-chave do arquivo 2
    char **d2Fields = DataLine_get_fields(d2);
    int numFields2 = DataLineControl_get_field_amt(controls[1]);
    for (int i = 0; i < numFields2; i++) {
        fputc(',', outputFile);
        fputs(d2Fields[i], outputFile);
    }
    
    fputc('\n', outputFile);
}

// funcao principal da juncao usando strings
void merge_sorted_files(ProgramConfig *pc, char **filenames, DataLineControl **controls) {
    FILE *sortedFile1 = fopen(filenames[0], "rb");
    FILE *sortedFile2 = fopen(filenames[1], "rb");
    FILE *outputFile = ProgramConfig_get_files(pc)[2];

    if (!sortedFile1 || !sortedFile2 || !outputFile) {
        if(sortedFile1) fclose(sortedFile1); 
        if(sortedFile2) fclose(sortedFile2); 
        return;
    }

    DataLine *d1 = malloc(DataLine_size()), *d2 = malloc(DataLine_size());
    int keyAmt = Vector_size(ProgramConfig_get_key_indexes(pc)[0]);
    int stat1 = DataLine_desserialize(controls[0], d1, sortedFile1);
    int stat2 = DataLine_desserialize(controls[1], d2, sortedFile2);
    bool stop = (stat1 == -1 || stat2 == -1);

    while(!stop) {

        int comparison = DataLine_compare(keyAmt, d1, d2);

        if (comparison < 0) 
        {
            DataLine_free(controls[0], d1);
            stat1 = DataLine_desserialize(controls[0], d1, sortedFile1);
        }
        else if (comparison > 0) 
        {
            DataLine_free(controls[1], d2);
            stat2 = DataLine_desserialize(controls[1], d2, sortedFile2);
        }
        else 
        {
            // match! escreve a juncao
            write_joined_line(outputFile, d1, d2, keyAmt, controls);

            DataLine_free(controls[0], d1);
            DataLine_free(controls[1], d2);

            // avanca ambos os arquivos
            stat1 = DataLine_desserialize(controls[0], d1, sortedFile1);
            stat2 = DataLine_desserialize(controls[1], d2, sortedFile2);
        }

        stop = (stat1 == -1 || stat2 == -1);
    }

    if(stat1 != -1) DataLine_free(controls[0], d1);
    if(stat2 != -1) DataLine_free(controls[1], d2);
    free(d1);
    free(d2);
    fclose(sortedFile1);
    fclose(sortedFile2);
}