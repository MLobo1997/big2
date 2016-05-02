#ifndef VERIFICAJOGADA_H_ 
#define VERIFICAJOGADA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cartas.h"
#include "autoplay.h"
#include "verificaJogada.h"

void valueToStr (char str[], int valor , int naipe);

int returnValue (char carta[4]);

int returnNaipe (char carta[4]);

int maoRead (char cards[4][56] , MAO mao);

int numCartas (MAO sel);

void swap(int v[], int i, int j);

int partition (int v[], int N);

void QSort (int v[], int N);

int verificaSeguidos (int n1 , int n2);

int sequencia (char cartas[4][56]);

int flush (char cartas[4][56]);

int fullAndfour (char cartas[4][56]);

int previousPlayer (ESTADO *e);

int cartaMaior (char carta1[4] , char carta2[4]);

int verificaDuas (char cartas[4][56] , char cartasAnteriores[4][56]);

int verificaTres (char cartas[4][56] , char cartasAnteriores[4][56]);

int verificaJogada (ESTADO e);

#endif