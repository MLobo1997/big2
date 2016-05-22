#include "jogador_inteligente.h"
#include "read.h"

/** \brief Ao receber o valor e o naipe de uma carta converte-a para uma string de identificação com a estrutura "(valor)(naipe)"
@param STRING Array onde será colocada a identificação da carta.
@param INT Valor da carta.
@param INT Naipe da carta.
*/
void valueToStr (char str[], int valor , int naipe){

	char *suit = NAIPES;
	char *rank = VALORES;

	sprintf (str , "%c%c" , rank[valor] , suit[naipe]);
}

/** \brief Coloca num array strings com o formato "(valor)(naipe)" e devolve o número de strings colocadas
@param MATRIZ poderá ter até 56 cartas (strings de 3 elementos).
@param MAO O long long int que será lido.
@return NUM número de cartas.
*/

int maoWrite (char cards[52][3] , MAO mao){

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

/** \brief Devolver o valor de 0 a 12 de uma carta
@param STRING Carta identificada.
@return INT Valor da carta.
*/

int returnValue (char carta[3]){

	int i;
	char *rank = VALORES;

	for (i = 0; carta[0] != rank[i] ; i++);

	return i;
}

/** \brief Devolver o naipe de 0 a 3 de uma carta
@param STRING Carta identificada.
@return INT Valor da carta.
*/

int returnNaipe (char carta[3]){

	int i;
	
	char *suit = NAIPES;

	for (i = 0; carta[1] != suit[i] ; i++);

	return i;
}

MAO maoRead (char cards[13][3], int N){

	MAO mao = (MAO) 0;

	int i, valor, naipe;

	for (i = 0 ; i < N ; i++){
		valor = returnValue(cards[i]);
		naipe = returnNaipe(cards[i]);
		mao = add_carta(mao, naipe, valor);
	}

	return mao;
}

int readAction (JOGO e, char *action){

	if (!strcmp(action, "PASSOU")) e->passar++;

	if (!strcmp(action, "JOGADA"));

	return 1;
}