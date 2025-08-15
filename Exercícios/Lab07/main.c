#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "str.h"
#include "suffix.h"
#include "vector.h"
#include "radix.h"

void search_loop(String *arquivo, Vector *suffixes, int N, int context)
{
    char *buffer = NULL;
    size_t len = 0;
    int read;

    while (true)
    {
        printf("Search: ");

        // Realiza a leitura
        read = getline(&buffer, &len, stdin);

        // Se deu erro, fecha
        if (read == -1)
        {
            break;
        }

        // Remove newline
        if (read > 0 && buffer[read - 1] == '\n')
        {
            buffer[read - 1] = '\0';
            read--;
        }

        // Se apertou Enter, fecha
        if (buffer[0] == '\0')
        {
            break;
        }

        // Cria String* s a partir do buffer
        String *s = str_init(buffer, read);

        // Tenta buscar o primeiro sufixo que bate com a pesquisa;
        int found = -1;
        int idx = vector_binary_search(suffixes, s, suffix_prefix_compare);

        // Se não encontrar, vai pra próxima iteração do loop, se tiver;
        if (idx == -1)
        {
            printf("not found\n");
            str_destroy(s);
            continue;
        }

        // Retrocede para o primeiro sufixo que casa
        found = idx;
        while (found > 0)
        {
            Suffix *prev = (Suffix *)vector_get(suffixes, found - 1);
            if (suffix_prefix_compare(s, &prev) != 0)
                break;
            found--;
        }
        // Imprime todos os sufixos que começam com query
        for (int i = found; i < N; i++)
        {
            // Pega o sufixo em i;
            Suffix *sf = (Suffix *)vector_get(suffixes, i);

            // Verifica se prefixo e sufixo ainda batem;
            if (suffix_prefix_compare(s, &sf) != 0)
            {
                break;
            }

            // Pega o início e fim em arquivo;
            int idx = suffix_index(sf);
            int begin = idx - context;
            int end = idx + str_len(s) + context - 1;
            
            // Se begin for menor que início de arquivo
            if (begin < 0)
            {
                begin = 0;
            }

            // Se end ultrapassar o tamanho de arquivo, fim vira o fim do arquivo;
            if (end >= N)
            {
                end = N - 1;
            }
        
            // Imprime o trecho em que está localizado o termo;
            str_print_begin_end(arquivo, begin, end);
            printf("\n");
        }

        // Destrói a string search;
        str_destroy(s);
    }

    // Libera o buffer;
    free(buffer);
}

int main(int argc, char *argv[])
{
    // Abre o arquivo para realizar a leitura
    FILE *f = fopen(argv[1], "r");

    // Pega o contexto
    int context = atoi(argv[2]);

    // Cria uma string vazia correspondente ao arquivo
    String *arquivo = str_init("", 0);

    // Algoritmo para leitura do arquivo;
    char *line = NULL;
    size_t len = 0;

    // Descarta o cabeçalho, por enquanto;
    getline(&line, &len, f);

    while (getline(&line, &len, f) != -1)
    {
        // Pega a primeira string até \t, \n ou \r
        char *token = strtok(line, " \t\n\r");

        // Enquanto tiver strings na linha
        while (token)
        {
            // Cria uma string do tamanho dela
            String *word = str_init(token, strlen(token));

            // Concatena com a string do arquivo
            arquivo = str_concat(arquivo, word);

            // Destrói a string que foi pega (foi feita uma cópia dela)
            str_destroy(word);

            // Passa para a pŕoxima string da linha
            token = strtok(NULL, " \t\n\r");
        }
    }

    // Libera memória de buffer de linha e fecha o arquivo;
    free(line);
    fclose(f);

    // Cria o vector de sufixos usando a biblioteca suffix
    int N = str_len(arquivo);
    Vector *suffixes = vector_init(suffix_destroy, N);
    for (int i = 0; i < N; i++)
    {
        Suffix *suf = suffix_init(arquivo, i);
        vector_insert(suffixes, suf);
    }

    // Ordena os sufixos usando radix sort otimizado;
    radix_sort_suffixes((Suffix **)vector_data(suffixes), N);

    vector_print(suffixes, suffix_print);

    search_loop(arquivo, suffixes, N, context);

    // Libera memória dos sufixos
    vector_destroy(suffixes);

    // Libera a memória da string correspondente ao arquivo;
    str_destroy(arquivo);

    return 0;
}