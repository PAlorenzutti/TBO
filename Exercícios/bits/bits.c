#include "bits.h"

#include <stdio.h>

void imprimeBits(unsigned int n)
{
    for (int i = 31; i >= 0; i--)
    {
        if ((n & (1 << i)) != 0)
            printf("1");
        else
            printf("0");

        if (i % 8 == 0 && i > 0)
        {
            printf(" ");
        }
    }
    printf("\n");
}

// a) verificar se x é um número impar;
int ehImpar(unsigned int x)
{
    
    // imprimeBits(x & 1);

    // Faz operação & entre o número e o número 1
    // Retorna 1, resultado da interseção entre os dois números se for ímpar
    // Retorna 0, resultado da interseção entre os dois números se for par
    return (x & 1);
}

// b) obter o resto da divisão de x por 2 elevado a k
unsigned int restoDivisaoPorPotenciaDe2(unsigned int x, unsigned int k)
{
    imprimeBits(x);
    imprimeBits(k - 1);
    
    return x & (k - 1);
}

// c) verificar se x é divisível por 2 elevado a k
int ehDivisivelPorPotenciaDe2(unsigned int x, unsigned int k)
{
    return (x & (k - 1)) == 0;
}