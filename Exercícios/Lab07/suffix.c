#include "suffix.h"
#include "str.h"

#include <stdlib.h>

struct Suffix
{
    String *s;
    int index;
};

Suffix *suffix_init(String *s, int index)
{
    Suffix *sf = (Suffix *)malloc(sizeof(Suffix));
    sf->s = s;
    sf->index = index;
    return sf;
}

String *suffix_get(const Suffix *sf)
{
    return sf->s;
}

int suffix_index(const Suffix *sf)
{
    return sf->index;
}

void suffix_print(const void *sf)
{
    Suffix *suffix = (Suffix *)sf;

    str_print_at(suffix->s, suffix->index);
}

void suffix_destroy(void *sf)
{
    Suffix *suffix = (Suffix *)sf;

    free(suffix);
}

// Função parecida com str_compare_from
int suffix_compare(const void *a, const void *b)
{
    const Suffix *sa = *(const Suffix **)a;
    const Suffix *sb = *(const Suffix **)b;

    // Pega o menor sufixo entre os dois passados;
    int n_a = str_len(sa->s) - sa->index;
    int n_b = str_len(sb->s) - sb->index;
    int min_len = n_a < n_b ? n_a : n_b;

    // Mesma coisa
    for (int k = 0; k < min_len; k++)
    {
        char ca = str_char_at(sa->s, sa->index + k);
        char cb = str_char_at(sb->s, sb->index + k);
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
    }

    // Se saiu do loop e tem o mesmo comprimento, são iguais;
    if (n_a == n_b)
        return 0;

    // Se a primeira for menor que a segunda, retorna -1, caso contrário, retorna 1;
    return n_a < n_b ? -1 : 1;
}

int suffix_prefix_compare(const void *query, const void *suffix)
{
    String *search = (String *)query;
    const Suffix *sf = *(const Suffix **)suffix;
    int n = str_len(sf->s);
    int i = sf->index;
    int k = 0;
    int search_len = str_len(search);

    // Enquanto a palavra (prefixo) não tiver terminado e o sufixo não tiver chegado ao final;
    while (k < search_len && (i + k) < n)
    {
        char cq = str_char_at(search, k);
        char cs = str_char_at(suffix_get(sf), i + k);
        if (cq < cs)
            return -1;
        if (cq > cs)
            return 1;
        k++;
    }

    // Se ao final do loop, a palavra (prefixo) tiver chegado ao final, então o sufixo é igual ao prefixo;
    if (k == search_len)
    {
        return 0;
    }
    return -1;
}