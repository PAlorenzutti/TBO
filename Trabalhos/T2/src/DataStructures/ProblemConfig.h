#pragma once

#include "Vector.h"
#include <stdio.h>

/* TYPES */

typedef struct ProgramConfig ProgramConfig;

/* PROGRAM CONFIG HEADERS */

ProgramConfig *ProgramConfig_parse_arguments(int argc, char *argv[]);

void ProgramConfig_free(ProgramConfig *pc);

Vector **ProgramConfig_get_key_indexes(ProgramConfig *pc);

FILE **ProgramConfig_get_files(ProgramConfig *pc);

int ProgramConfig_get_P(ProgramConfig *pc);

int ProgramConfig_get_M(ProgramConfig *pc);
