#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long int MAO;

typedef struct jogo{

	MAO played, /*identifa a ultima mao jogada*/
		mao; /*Minha mão*/
	int	passar, /*identifica a sequencia de passagem*/
		nCartas, /*Número de cartas a ser jogadas*/
		eu; /*Identifica o jogador que eu represento*/

} *JOGO;

int initiate (JOGO e);

int indice(int naipe, int valor);

long long int add_carta(long long int est, int naipe, int valor);

long long int rem_carta(long long int est, int naipe, int valor);

MAO rem_cartas(MAO est, MAO rem);

MAO add_cartas(MAO est, MAO add);

int carta_existe(long long int est, int naipe, int valor);