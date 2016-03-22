#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
Ordem dos naipes
*/
#define NAIPES		"DCHS"
/**
Ordem das cartas
*/
#define VALORES		"3456789TJQKA2"

typedef long long int MAO;

void valueToStr (char str[], int valor , int naipe){

	char *suit = NAIPES;
	char *rank = VALORES;

	sprintf (str , "%c_%c" , rank[valor] , suit[naipe]);
}

/**
Devolver o valor de 0 a 12 de uma carta
*/

int returnValue (char carta[3]){

	int i;
	char *rank = VALORES;

	for (i = 0; carta[0] != rank[i] ; i++);

	return i;
}

/**
Devolver o naipe de 0 a 3 de uma carta
*/

int returnNaipe (char carta[3]){

	int i;
	char *suit = NAIPES;

	for (i = 0; carta[2] != suit[i] ; i++);

	return i;

}

/** \brief Coloca numa matriz strings com o formato "(valor)_(naipe)" e devolve o número de strings colocadas
@param MATRIZ poderá ter até 56 cartas (strings de 3 elementos)
@param MAO O long long int que será lido.
*/

int maoRead (char cards[3][56] , MAO mao){

	int valor, naipe, n;

	char tmp[3];

	for (naipe = n = 0 ; naipe <= 3 ; naipe++)
		for (valor = 0 ; valor <= 12 ; valor++){
			if (mao & ((MAO) 1)){
				valueToStr (tmp , valor , naipe);
				strcpy (cards[n] , tmp);
				n++;
			}
			mao = mao >> 1;
		}

	return n;
}

int numCartas (MAO sel){


	int nCartas, count;

	for (nCartas = count = 0; count < 56 ; sel = sel >> 1 , count++)
			if (sel & ((MAO) 1)) nCartas++;

	return nCartas;
}

/** Verifica se uma determinada jogada é valida*/ 

int verificaJogada (MAO mao){

	int nCartas, flag = 0;

	char cartas[3][56];

	nCartas = maoRead(cartas , mao);

	if (nCartas == 1) flag = 1;

	if (nCartas == 2) if (returnValue (cartas[0]) == returnValue (cartas[1])) flag = 1;

	if (nCartas == 3) if (returnValue (cartas[0]) == returnValue (cartas[1]) && returnValue (cartas[1]) == returnValue (cartas[2])) flag = 1;

	if (nCartas == 4);

	if (nCartas == 5);

	return flag; 

}

/*
int main (){

	MAO mao = 133;

	char cartas[3][56];

	int n;

	n = maoRead (cartas , mao);

	while (n--)
		printf("%s\n", cartas[n]);

	return 0;

}
*/
/*
int main (){

	int n;

	n = returnNaipe("2_S");

	printf("%d\n", n);

	return 0;
}
*/