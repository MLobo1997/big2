#include "jogador_inteligente.h"
#include "read.h"
#include "jogar.h"
#include "output.h"

/** \brief Devolver o valor de 0 a 12 de uma carta
@param STRING Carta identificada.
@return INT Valor da carta.
*/

int returnValue (char carta[3]){

	int i;
	char rank[14] = VALORES;

	for (i = 0; carta[0] != rank[i] ; i++);

	return i;
}

/** \brief Devolver o naipe de 0 a 3 de uma carta
@param STRING Carta identificada.
@return INT Valor da carta.
*/

int returnNaipe (char carta[3]){

	int i;
	
	char suit[5] = NAIPES;

	for (i = 0; carta[1] != suit[i] ; i++);

	return i;
}
/** \brief Converte cartas identificadas num long long int.
@param ARRAY Identificação das cartas num array.
@param INT número de cartas no array.
@return MAO Mão que se encontra no array.
*/
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

/** \brief Coloca no estado os valores da ultima jogada.
@param JOGO Estado.
@param STRING Identificação da ação.
@param STRING Identificação das cartas.
@param INT Número de cartas na 2ª string.
@return INT 1 caso o processo tenha terminado.
*/
int readMove (JOGO e, char *action, char cards[13][3], int N){

	if (strcmp(action, "JOGOU") == 0){
		e->played = maoRead(cards, N - 1);
		e->nCartas = N - 1;
		e->passar = 0;
	}
	return 1;
}

/** \brief Lê os comandos que não estão seguidos de uma mão, atualizando o estado.
@param JOGO Estado.
@param STRING Comando.
@return INT 1 caso o processo tenha terminado.
*/
int readAction (JOGO e, char *action){

	if (strcmp(action, "PASSOU") == 0){

		if (e->passar == 2){ /*Se for a 3ª passagem consecutiva, os parâmetros de jogada são reinicializados*/
			e->passar = 0; 
			e->nCartas = 0;
			e->played = (MAO) 0;
		}

		else e->passar++; /*Senão o e->passar aumenta*/
	}

	if (strcmp(action, "JOGADA") == 0) jogar(e);

	return 1;
}

/** Lê a mão que nos foi atribuída.
@param JOGO Estado.
@param STRING Identificação da ação.
@param STRING Identificação das cartas.
@param INT Número de cartas na 2ª string.
@return INT 1 caso o processo tenha terminado.
*/

int readHand (JOGO e, char *action, char cards[13][3], int N){

	MAO tmp = (MAO) 0;

	if (strcmp(action, "MAO") == 0)
	e->mao = maoRead(cards, N - 1);

	tmp = abreJogo(e);
	e->nCartas = 0;

	if (numCartas(tmp) == 5) e->bestCombo = tmp;

	return 1;
}

int readInput (char *action, char cards[13][3]){

	int n = 0, i;

	char input[150], *delim = " \n", *tokens;

	if (fgets(input, 150, stdin) != NULL){

		tokens = strtok(input, delim);

		n++;

		strcpy (action, tokens);

		tokens = strtok(NULL, delim);

		for (i = 0 ; tokens != NULL ; i++){

			strcpy(cards[i], tokens);
			n++;
			tokens = strtok(NULL, delim);
		}

	}

	return n;
}