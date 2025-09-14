#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Ponto{
    char *id;        // Identificador do ponto
    double *coords;  // Coordenadas no espaço R^m
};

struct ConjuntoDePontos{
    Ponto *pontos;   // Vetor de pontos
    int n;        // Número de pontos
    int m;       // Número de dimensões de cada ponto
    int allocated; // O quanto foi alocado para o vetor de pontos
};

ConjuntoDePontos* ler_pontos(const char* nome_arquivo) {

    // Abre o arquivo para leitura
    FILE* f = fopen(nome_arquivo, "r");
    if(f == NULL) {
        return NULL;
    }

    // Aloca espaço para a estrutura que irá armazenar os pontos
    ConjuntoDePontos* conjunto = (ConjuntoDePontos *)malloc(sizeof(ConjuntoDePontos));
    if(conjunto == NULL) {
        fclose(f);
        return NULL;
    }

    // Inicializa as variáveis da estrutura
    conjunto->n = 0; // Ainda não sei a quantidade de pontos
    conjunto->m = 0; // Ainda não sei o número de dimensões
    conjunto->allocated = 50; // Espaço alocado inicialmente para a quantidade de pontos (referência: caso de teste 1)

    conjunto->pontos = (Ponto *)malloc(sizeof(Ponto) * conjunto->allocated);
    if(conjunto->pontos == NULL) {
        fclose(f);
        free(conjunto);
        return NULL;
    }

    char* linha = NULL;
    size_t len = 0;

    // O(n*(m+1) + O(m))
    while (getline(&linha, &len, f) != -1) {

        // Remove newline
        linha[strcspn(linha, "\n")] = '\0';


        // Esse if descobre qual o número de dimensões do ponto
        if(conjunto->n == 0) { // Caso seja a leitura da primeira linha

            char *linha_copia = strdup(linha);
    
            int num_tokens = 0;
            char *resultado_lido = strtok(linha_copia, ",");

            // Vai contando cada palavra que ele separa por vírgula
            // O(m)
            while (resultado_lido != NULL) {
                num_tokens++;
                resultado_lido = strtok(NULL, ",");
            }
            
            // Define o número de dimensões
            conjunto->m = num_tokens - 1; // Como ele contabiliza o identificador do ponto, é preciso tirar 1 da contagem
            free(linha_copia);
        }

        // Definindo os valores de um ponto
        Ponto p;
        char *token = strtok(linha, ","); // Pega o identifcador do ponto

        p.id = strdup(token); // Copia o conteúdo, aloca espaço e armazena o identificador do ponto
        p.coords = malloc(conjunto->m * sizeof(double)); // Aloca espaço para os valores das coordenadas do ponto

        for (int i = 0; i < conjunto->m; i++) {
            token = strtok(NULL, ","); // Lê o valor da coordenada
            p.coords[i] = strtod(token, NULL); // Faz a conversão de string para double e armazena
        }

        // Verifica se o vetor de pontos precisa ser realocado
        if(conjunto->allocated == conjunto->n) {
            
            conjunto->allocated *= 2;
            conjunto->pontos = (Ponto *)realloc(conjunto->pontos, sizeof(Ponto) * conjunto->allocated);
        }

        // Insere no conjunto de pontos o ponto
        conjunto->pontos[conjunto->n] = p;
        conjunto->n++;
    }

    free(linha);
    fclose(f);
    return conjunto;
}

int obter_num_pontos(ConjuntoDePontos *conjunto) {
    return conjunto->n;
}

int obter_num_dimensoes(ConjuntoDePontos *conjunto) {
    return conjunto->m;
}

Ponto *obter_ponto(ConjuntoDePontos *conjunto, int idx) {

    if(idx < 0 || idx >= conjunto->n) return NULL;
    return &conjunto->pontos[idx];
}

char *obter_id(Ponto *ponto) {
    return ponto->id;
}

double obter_uma_coord(Ponto *ponto, int idx) {
    return ponto->coords[idx];
}

void liberar_pontos(ConjuntoDePontos *conjunto) {
    if (conjunto == NULL) return;

    for (int i = 0; i < conjunto->n; i++) {
        free(conjunto->pontos[i].id);
        free(conjunto->pontos[i].coords);
    }
    free(conjunto->pontos);
    free(conjunto);
}