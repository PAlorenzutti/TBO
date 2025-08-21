#ifndef BITS_H
#define BITS_H

void imprimeBits(unsigned int n);

// a) verificar se x é um número impar;
int ehImpar(unsigned int x);

// b) obter o resto da divisão de x por 2 elevado a k
unsigned int restoDivisaoPorPotenciaDe2(unsigned int x, unsigned int k);

// c) verificar se x é divisível por 2 elevado a k
int ehDivisivelPorPotenciaDe2(unsigned int x, unsigned int k);

#endif