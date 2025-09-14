#include "ESElement.h"

#include <stdlib.h>


/* ES ELEMENT PRIVATE*/


struct ESElement
{
    void *control;

    DesserializeFunc desserialize;
    SerializeFunc serialize;
    FreeFunc free;
};


/* ES ELEMENT PUBLIC */


ESElement *ESElement_build(DesserializeFunc desserialize, SerializeFunc serialize, 
                            FreeFunc free, void *control)
{
    ESElement *ss = malloc(sizeof(ESElement));
    ss->desserialize = desserialize;
    ss->serialize = serialize;
    ss->free = free;
    ss->control = control;
    return ss;
}


void ESElement_serialize(ESElement *ss, void *data, FILE *output)
{
    ss->serialize(ss->control, data, output);
}


int ESElement_desserialize(ESElement *ss, void *out, FILE *input)
{
    return ss->desserialize(ss->control, out, input);
}


void ESElement_free(ESElement *ss)
{
    free(ss);
}


void ESElement_data_free(ESElement *ss, void *data)
{
    ss->free(ss->control, data);
}