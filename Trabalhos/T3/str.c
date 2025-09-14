
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "str.h"

struct String{
    char *c;
    int len;
};

// Inicializa uma String com uma cópia dos caracteres fornecidos
String *str_init(char *c, int len){
    String *s = (String*)malloc(sizeof(String));

    // Aloca o tamanho da string + "\0"
    s->c = (char*)malloc((len + 1) * sizeof(char));

    // Copia todos os caracteres em lowercase
    for (int i = 0; i < len; i++) {
        s->c[i] = tolower((unsigned char)c[i]);
    }

    // Seta o último caractere como '\0'
    s->c[len] = '\0';

    // Passa o tamanho da string
    s->len = len;

    return s;
}

// Compara duas strings a partir de um determinado caractere
int str_compare_from(String *s1, String *s2, int i){
    // Pega qual é a menor string entre s e t;
    // Se s for menor que t, pega o tamanho de s, caso contrário, pega o tamanho de t;
    int min = s1->len < s2->len ? s1->len : s2->len;

    for(int j = i; j < min; j++){
        // Se for menor, retorna -1
        //Se for maior, retorna 1
        if(s1->c[j] < s2->c[j]){ return -1; }
        if(s1->c[j] > s2->c[j]){ return 1; }
    }

    // Se for igual, retorna 0
    return s1->len - s2->len;
}

int str_compare(const void* s1, const void* s2){
    return str_compare_from((String*)s1, (String*)s2, 0);
}

char* str_get(String *s){
    return s->c;
}

int str_len(String *s){
    return s->len;
}

void str_print(const void *string){
    String *s = (String*) string;
    
    printf("%s", s->c);
}

// Libera a memória de uma String
void str_destroy(void *string) {
    String *s = (String*) string;
    
    free(s->c);
    free(s);
}