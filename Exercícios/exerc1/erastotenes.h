#ifndef _ERASTOTENES_H_
#define _ERASTOTENES_H_

typedef struct Erastotenes Erastotenes;

int ehPrimo(int numero);

Erastotenes* numerosPrimos(int N);

void printaPrimos(Erastotenes *lista);

void freeErastotenes(Erastotenes *lista);

#endif