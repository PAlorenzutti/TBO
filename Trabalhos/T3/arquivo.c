#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arquivo.h"
#include "vector.h"
#include "str.h"
#include "rbt.h"

struct Program{
    Vector *index;
    RBT *indexador;
    RBT *stopwords;
    char *folder_path;
};

Program *program_init(char *folder_path){
    Program *p = (Program*)malloc(sizeof(Program));

    // Guarda o caminho da pasta de entrada;
    p->folder_path = strdup(folder_path);

    // Inicia o vetor de índices (páginas = String) 
    p->index = vector_init(str_destroy, 10);

    // Inicia a árvore de termos (chaves = String) e suas páginas (String)
    p->indexador = rbt_init(str_compare, str_destroy, vector_destroy);

    // Inicia a árvore de termos (chaves = String) e suas stop words (values);
    p->stopwords = rbt_init(str_compare, str_destroy, str_destroy);

    return p;
}

void stopwords_read(Program *p){
    // Monta o caminho do arquivo stopwords.txt
    char *path = malloc(strlen(p->folder_path) + strlen("/stopwords.txt") + 1);
    strcpy(path, p->folder_path);
    strcat(path, "/stopwords.txt");

    FILE *f = fopen(path, "r");

    char *line = NULL;
    size_t len = 0;

    while(getline(&line, &len, f) != -1){
        // Remove o new line
        line[strcspn(line, "\n")] = '\0';

        // Cria String* para a stopword
        char *temp = strdup(line);
        String *stopword = str_init(temp, strlen(temp));
        free(temp);

        // Só insere se ainda não estiver na árvore
        if (rbt_get(p->stopwords, stopword) == NULL) {
            rbt_insert(p->stopwords, stopword, stopword);
        } else {
            str_destroy(stopword);
        }
    }

    free(line);
    fclose(f);
    free(path);
}

void index_read(Program *p){
    // Estabelece o caminho exato do arquivo index.txt de acordo com o caminho relativo;
    char *path = malloc(strlen(p->folder_path) + strlen("/index.txt") + 1);
    strcpy(path, p->folder_path);
    strcat(path, "/index.txt");
    
    // Leitura do arquivo
    FILE *f = fopen(path, "r");

    char *line = NULL;
    size_t len = 0;

    while(getline(&line, &len, f) != -1){
        // Remove o new line
        line[strcspn(line, "\n")] = '\0';

        // Cria uma cópia da linha (strdup) e inicializa String*
        char *temp = strdup(line);
        String *page = str_init(temp, strlen(temp));
        free(temp);

        // Insere no vetor de índices
        vector_insert(p->index, page);
    }

    free(line);

    // Fecha o arquivo e libera o caminho construído
    fclose(f);
    free(path);
}

void pages_read(Program *p){
    for (int i = 0; i < vector_size(p->index); i++) {
        String *page = (String*)vector_get(p->index, i);

        // Monta o caminho do arquivo: <path>/pages/<nome_arquivo>
        size_t path_len = strlen(p->folder_path) + strlen("/pages/") + strlen(str_get(page)) + 1;
        char *path = malloc(path_len);
        strcpy(path, p->folder_path);
        strcat(path, "/pages/");
        strcat(path, str_get(page));

        FILE *f = fopen(path, "r");

        char *line = NULL;
        size_t len = 0;

        while (getline(&line, &len, f) != -1) {
            // Pega o primeiro termo da linha;
            char *token = strtok(line, " \t\r\n");

            while (token) {
                // Inicia uma string, em lowercase, com o termo;
                String *termo = str_init(token, strlen(token));

                // Se não for stopword, indexa
                if (rbt_get(p->stopwords, termo) == NULL) {
                    // Verifica se existe esse termo na árvore de índices
                    Vector *pages = rbt_get(p->indexador, termo);
                    
                    if (!pages) {
                        // Se não existir, cria o vetor de páginas e insere na árvore de índices;
                        pages = vector_init(str_destroy, vector_size(p->index));
                        vector_insert(pages, str_init(str_get(page), str_len(page)));
                        rbt_insert(p->indexador, termo, pages);
                        // printf("%s está contido em %s\n", str_get(termo), str_get(page));
                    } else {
                        // Verifica se já existe o nome do arquivo no vetor (opcional)
                        int found = 0;
                        // printf("%s encontrado novamente em %s\n", str_get(termo), str_get(page));
                        for (int j = 0; j < vector_size(pages); j++) {
                            String *search = (String*)vector_get(pages, j);
                            if (str_compare(search, page) == 0) {
                                found = 1;
                                break;
                            }
                        }
                        if (!found) {
                            vector_insert(pages, str_init(str_get(page), str_len(page)));
                        }
                        str_destroy(termo);
                    }
                } else {
                    // Se for stopword, apenas destrói o termo
                    str_destroy(termo);
                }
                token = strtok(NULL, " \t\r\n");
            }
        }

        free(line);
        fclose(f);
        free(path);
    }

    rbt_print(p->indexador, str_print, vector_print_str);
}

void program_destroy(Program *p){
    // Libera a string de caminho
    free(p->folder_path);

    // Libera o vetor de páginas
    vector_destroy(p->index);

    // Libera a árvore de stopwords
    rbt_destroy(p->stopwords);

    // Libera a árvore de índices 
    rbt_destroy(p->indexador);

    // Libera o programa
    free(p);
}