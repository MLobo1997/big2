#include "jogador_inteligente.h"
#include "read.h"
#include "jogar.h"
#include "output.h"

/** \brief Ao receber o valor e o naipe de uma carta converte-a para uma string de identificação com a estrutura "(valor)(naipe)"
@param STRING Array onde será colocada a identificação da carta.
@param INT Valor da carta.
@param INT Naipe da carta.
*/
void valueToStr (char str[], int valor , int naipe){

	char suit[5] = NAIPES;
	char rank[14] = VALORES;

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
/** Informa o jogo que passou a jogada.
@return INT 1, caso o procedimento tenha terminado corretamente.
*/
int passarOut (){

	printf("PASSO\n");

	return 1;
}

/** Imprime as jogadas.
@param MAO Mão a ser impressa.
@param INT Número de cartas na mão.
@return INT 1, caso o procedimento tenha terminado corretamente.
*/
int jogaOut (MAO mao, int N){

	char cards[5][3];

	maoWrite(cards, mao);

	if (N == 1) printf("%s\n", cards[0]);

	if (N == 2) printf("%s %s\n", cards[0], cards[1]);

	if (N == 3) printf("%s %s %s\n", cards[0], cards[1], cards[2]);

	if (N == 5) printf("%s %s %s %s %s\n", cards[0], cards[1], cards[2], cards[3], cards[4]);

	return 1;
}