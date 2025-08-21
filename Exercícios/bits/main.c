#include <stdio.h>
#include <math.h>

#include "bits.h"

int main()
{
    unsigned int x = 32;
    unsigned int k = 3;

    printf("x = %u\n", x);
    imprimeBits(x);

    // a) verificar se x é um número impar;
    if (ehImpar(x))
    {
        printf("x é impar\n");
    }
    else
    {
        printf("x é par\n");
    }

    // b) obter o resto da divisão de x por 2 elevado a k
    unsigned int resto = restoDivisaoPorPotenciaDe2(x, pow(2, k));
    printf("O resto da divisão de x por 2^%d é %u\n", k, resto);

    // c) verificar se x é divisível por 2 elevado a k
    if (ehDivisivelPorPotenciaDe2(x, k))
    {
        printf("x é divisível por 2^%d\n", k);
    }
    else
    {
        printf("x não é divisível por 2^%d\n", k);
    }

    return 0;
}