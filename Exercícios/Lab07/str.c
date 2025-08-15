#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "str.h"

struct String
{
    char *c;
    int len;
};

// Inicializa uma String com uma cópia dos caracteres fornecidos
String *str_init(char *c, int len)
{
    String *s = (String *)malloc(sizeof(String));

    // Aloca o tamanho da string + "\0"
    s->c = (char *)malloc((len + 1) * sizeof(char));

    // Copia todos os caracteres exatamente como estão
    for (int i = 0; i < len; i++)
    {
        s->c[i] = c[i];
    }

    // Seta o último caractere como '\0'
    s->c[len] = '\0';

    // Passa o tamanho da string
    s->len = len;

    return s;
}

// Compara duas strings a partir de um determinado caractere
int str_compare_from(String *s1, String *s2, int i)
{
    // Pega qual é a menor string entre s e t;
    // Se s for menor que t, pega o tamanho de s, caso contrário, pega o tamanho de t;
    int min = s1->len < s2->len ? s1->len : s2->len;

    for (int j = i; j < min; j++)
    {
        // Se for menor, retorna -1
        // Se for maior, retorna 1
        if (s1->c[j] < s2->c[j])
        {
            return -1;
        }
        if (s1->c[j] > s2->c[j])
        {
            return 1;
        }
    }

    // Se for igual, retorna 0
    return s1->len - s2->len;
}

int str_compare(const void *s1, const void *s2)
{
    return str_compare_from((String *)s1, (String *)s2, 0);
}

char *str_get(String *s)
{
    return s->c;
}

char str_char_at(String *s, int i)
{
    return s->c[i];
}

int str_len(String *s)
{
    return s->len;
}

void str_print(const void *string)
{
    String *s = (String *)string;

    printf("%s\n", s->c);
}

void str_print_at(String *s, int i){
    for(int j = i; s->c[j] != '\0'; j++){
        printf("%c", s->c[j]);
    }
}

void str_print_begin_end(String *s, int begin, int end){
    for(int i = begin; i <= end; i++){
        printf("%c", s->c[i]);
    }
}

// Libera a memória de uma String
void str_destroy(void *string)
{
    String *s = (String *)string;

    free(s->c);
    free(s);
}

// Concatena s2 ao final de s1, adicionando espaço se s1 não estiver vazia
// Realoca s1 internamente e retorna o próprio ponteiro s1
String *str_concat(String *s1, String *s2) {
    int add_space = (s1->len > 0 && s2->len > 0) ? 1 : 0;
    int new_len = s1->len + s2->len + add_space;
    s1->c = (char*)realloc(s1->c, (new_len + 1) * sizeof(char));
    if (!s1->c) return NULL;
    if (add_space) {
        s1->c[s1->len] = ' ';
        for (int i = 0; i < s2->len; i++)
            s1->c[s1->len + 1 + i] = s2->c[i];
    } else {
        for (int i = 0; i < s2->len; i++)
            s1->c[s1->len + i] = s2->c[i];
    }
    s1->c[new_len] = '\0';
    s1->len = new_len;
    return s1;
}