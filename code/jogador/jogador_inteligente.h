#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long int MAO;

typedef struct jogo{

	MAO played[3], /*identifa as maos jogadas pelos outros jogadores na ultima ronda*/
		mao; /*Minha mão*/
	int jogador, /*identifica o jogador atual*/
		passar, /*identifica a sequencia de passagem*/
		eu; /*Identifica o jogador que eu represento*/

} *JOGO;

int initiate (JOGO e);

int indice(int naipe, int valor);

long long int add_carta(long long int est, int naipe, int valor);

long long int rem_carta(long long int est, int naipe, int valor);

MAO rem_cartas(MAO est, MAO rem);

MAO add_cartas(MAO est, MAO add);

int carta_existe(long long int est, int naipe, int valor);