#ifndef ARQUIVO_H
#define ARQUIVO_H

typedef struct Program Program;

Program *program_init(char *folder_path);

void index_read(Program *p);

void pages_read(Program *p);

void stopwords_read(Program *p);

void program_destroy(Program *p);

#endif