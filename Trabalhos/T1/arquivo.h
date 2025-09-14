#ifndef ARQUIVO_H
#define ARQUIVO_H

typedef struct Ponto Ponto;
typedef struct ConjuntoDePontos ConjuntoDePontos;

/**
 * Lê os pontos a partir de um arquivo e retorna uma estrutura preenchida.
 * @param nome_arquivo Caminho para o arquivo de entrada.
 * @return Ponteiro para ConjuntoDePontos ou NULL em caso de erro.
 */
ConjuntoDePontos *ler_pontos(const char *nome_arquivo);

/**
 * @brief Retorna o número de pontos.
 * @param conjunto um ponteiro para o conjunto de pontos.
 * @return O número de pontos.
 */
int obter_num_pontos(ConjuntoDePontos *conjunto);

/**
 * @brief Retorna o número de dimensões (m) dos pontos.
 * @param conjunto um ponteiro para o conjunto de pontos.
 * @return número de dimensões dos pontos.
 */
int obter_num_dimensoes(ConjuntoDePontos *conjunto);

/**
 * @brief Retorna o ponteiro para um ponto específico.
 * @param conjunto um ponteiro para o conjunto de pontos.
 * @param idx índice de qual ponto você quer dentro do conjunto.
 * @return ponteiro para um ponto específico ou NULL caso seja um índice inválido.
 */
 Ponto *obter_ponto(ConjuntoDePontos *conjunto, int idx);

/**
 * @brief Retorna o ID de um ponto.
 * @param ponto um ponteiro para o ponto.
 * @return o identificador do ponto.
 */
 char *obter_id(Ponto *ponto);

/**
 * @brief Retorna o valor de uma coordenada específica de um ponto.
 * @param ponto Um ponteiro para o ponto.
 * @param idx Índice de qual ponto você quer dentro do conjunto.
 * @return O valor de uma coordenada específica de um ponto.
 */
double obter_uma_coord(Ponto *ponto, int idx);

/**
 * @brief Libera toda a memória associada ao ConjuntoDePontos.
 * @param conjunto um ponteiro para o conjunto de pontos.
 */
void liberar_pontos(ConjuntoDePontos *conjunto);

#endif