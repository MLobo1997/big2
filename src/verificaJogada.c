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

void insert (int v[] , int N , int x){

	int i;

	for (i = 0 ; x > v[i] && i < N ; i++);

	while (i < N){
		v[N] = v[N - 1];
		N--;
	}

	v[i] = x;
}

void insertSort (int v[] , int N){

	int i;

	for (i = 0 ; i < N ; i++)
		insert (v , i , v[i]);

}

/** \brief Recebe dois valores de cartas e verifica se são seguidas
*/
int verificaSeguidos (int n1 , int n2){

	int flag = 0;

	if (n1 + 1 == n2 || n1 == n2 + 1) flag = 1;

	if ((n1 == 12 && n2 == 0) || (n1 == 0 && n2 == 12)) flag = 1; /*Para o caso de uma ser um 3 e outra 2*/

	return flag;
}

int sequencia (char cartas[3][56]){

	int valores[5], tmp[5];
	int i, n, ntmp, flag = 0;

	for (i = 0 ; i < 5 ; i++)
		valores[i] = returnValue(cartas[i]);

	insertSort (valores , 5);

	for (i = 0 ; i + 1 < 5 && verificaSeguidos (valores[i] , valores[i + 1]) ; i++);

	if (i == 4) flag = 1;

	else{
		for (n = 0 , i++ ; i < 5 ; n++, i++)
			tmp[n] = valores[i];
		for (i = 0, ntmp = 4 , n = 4 - n ; n >= 0 ; n--, i++, ntmp--)
			valores[ntmp] = valores[n];
		for (n = 0 , i = 4 - i ; n <= i  ; n++)
			valores[n] = tmp[n];


		for (i = 0 ; i + 1 < 5 && verificaSeguidos (valores[i] , valores[i + 1]) ; i++);

		if (i == 4) flag = 1;
	}

	return flag;
}

int flush (char cartas[3][56]){

	int naipes[5], i;

	for (i = 0 ; i < 5 ; i++)
		naipes[i] = returnNaipe (cartas[i]);

	for (i = 0 ; i + 1 < 5 && (naipes[i] == naipes[i + 1]) ; i++);

	return (i == 4);
}

/** Verifica se uma determinada jogada é valida*/ 

int verificaJogada (MAO mao){

	int nCartas, flag = 0;

	char cartas[3][56];

	nCartas = maoRead(cartas , mao);

	if (nCartas == 1) flag = 1;

	if (nCartas == 2) if (returnValue (cartas[0]) == returnValue (cartas[1])) flag = 1;

	if (nCartas == 3) if (returnValue (cartas[0]) == returnValue (cartas[1]) && returnValue (cartas[1]) == returnValue (cartas[2])) flag = 1;

	if (nCartas == 5) if (sequencia (cartas), flush (cartas)) flag = 1;

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

/*
int main (){

	int array[1000], N, i;

	printf("Elementos?:\n");
	scanf ("%d" , &N);

	for (i = 0 ; i < N ; i++){
		printf("Elemento nº%d\n", i);
		scanf ("%d" , &array[i]);
	}

	insertSort (array , N);

	for (i = 0 ; i < N ; i++)
		printf("%d\n" , array[i]);

	return 0;
}
*/