#include "DataLine.h"

#include "ProblemConfig.h"
#include "Vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* DATA LINE CONTROL PRIVATE*/


struct DataLineControl
{
    int *keyPos;
    int keyAmt, fieldsAmt;
};


/* DATA LINE CONTROL PUBLIC */


DataLineControl *DataLineControl_build(Vector *keyIndexes, FILE *in)
{
    DataLineControl *control = malloc(sizeof(DataLineControl));
    char *line = NULL;
    size_t size = 0;
    int totalFields = 1;

    if (getline(&line, &size, in) != -1)
    {
        for (int i = 0; line[i] != '\0'; i++)
            if(line[i] == ',') totalFields++;
    }

    rewind(in); // volta para o início do arquivo
    if (line) free(line);
    
    int *keyPos = malloc(totalFields * sizeof(int));

    for(int i = 0; i<totalFields; i++) 
        keyPos[i] = -1;
    for (int i = 0; i < Vector_size(keyIndexes); i++) 
    {
        int idx = *(int*)Vector_get(keyIndexes, i);
        keyPos[idx] = i;
    }
    
    control->keyAmt = Vector_size(keyIndexes);
    control->fieldsAmt = totalFields - control->keyAmt;
    control->keyPos = keyPos;
    return control;
}


int DataLineControl_get_field_amt(DataLineControl *control)
{
    return control->fieldsAmt;
}


void DataLineControl_free(DataLineControl *control)
{
    free(control->keyPos);
    free(control);
}


/* DATA LINE PRIVATE */


struct DataLine
{
    char **keys;
    char **fields;
};


/* DATA LINE PUBLIC */


void DataLine_build(DataLineControl *control, DataLine *out)
{
    out->keys = malloc(sizeof(char *) * control->keyAmt);
    out->fields = malloc(sizeof(char *) * control->fieldsAmt);
}


int DataLine_desserialize(void *controlVoid, void *outVoid, FILE *input) 
{
    DataLineControl *control = controlVoid;
    DataLine *out = outVoid;

    char *line = NULL;
    size_t size = 0;
    getline(&line, &size, input);
    if(feof(input))
    {
        free(line);
        return -1;
    }

    DataLine_build(control, out);

    int fieldPos = 0, fieldIdx = 0;
    int totalFields = control->fieldsAmt + control->keyAmt;
    int *keyPosArr = control->keyPos;

    char *token;
    token = strtok(line, ",\n");
    while (token && fieldIdx < totalFields) 
    {
        if (keyPosArr[fieldIdx] != -1) 
        {
            int keyPos = keyPosArr[fieldIdx];
            out->keys[keyPos] = malloc(strlen(token) + 1);
            strcpy(out->keys[keyPos], token);
            keyPos++;
        } 
        else 
        {
            out->fields[fieldPos] = malloc(strlen(token) + 1);
            strcpy(out->fields[fieldPos], token);
            fieldPos++;
        }

        fieldIdx++;
        token = strtok(NULL, ",\n");
    }

    free(line);
    return 0;
}


void DataLine_serialize(void *controlVoid, void *dataVoid, FILE *out)
{
    DataLineControl *control = controlVoid;
    DataLine *data = dataVoid;
    
    int totalFields = control->fieldsAmt + control->keyAmt;

    int fieldPos = 0;
    for (int i = 0; i < totalFields; i++)
    {
        if(control->keyPos[i] != -1)
            fputs(data->keys[control->keyPos[i]], out);
        else
            fputs(data->fields[fieldPos++], out);

        if(i != totalFields-1) fputc(',', out);
    }
    fputc('\n', out);
}


// a função de comparação do qsort não aceita contexto.
int DataLine_compare_unique(const void* v1, const void *v2)
{
    const DataLine *d1 = v1, *d2 = v2;
    int idx = 0;
    int stat = strcmp(d1->keys[idx], d2->keys[idx]);
    while(stat == 0) //keys are unique, so eventually it will end
    {
        idx++;
        stat = strcmp(d1->keys[idx], d2->keys[idx]);
    }
    return stat;
}


int DataLine_compare(int keyAmt, DataLine *d1, DataLine *d2)
{
    int stat = strcmp(d1->keys[0], d2->keys[0]);
    int idx = 1;
    while(idx < keyAmt && stat == 0)
    {
        stat = strcmp(d1->keys[idx], d2->keys[idx]);
        idx++;
    }
    return stat;
}


void DataLine_free(void *controlVoid, void *dataVoid)
{
    DataLineControl *control = controlVoid;
    DataLine *data = dataVoid;

    for (int i = 0; i < control->keyAmt; i++)
        free(data->keys[i]);
    free(data->keys);
    for (int i = 0; i < control->fieldsAmt; i++)
        free(data->fields[i]);
    free(data->fields);
}


char **DataLine_get_fields(DataLine *d)
{
    return d->fields;
}

char **DataLine_get_keys(DataLine *d)
{
    return d->keys;
}

int DataLine_size()
{
    return sizeof(DataLine);
}