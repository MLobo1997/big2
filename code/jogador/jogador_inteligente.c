#include "jogador_inteligente.h"
#include "read.h"
#include "jogar.h"
#include "output.h"

/** \brief Devolve o índice da carta

@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O índice correspondente à carta
*/
int indice(int naipe, int valor){
	return naipe * 13 + valor;
}

/** \brief Adiciona uma carta ao estado

@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/
MAO add_carta(MAO est, int naipe, int valor){
	int idx = indice(naipe, valor);
	return est | ((MAO) 1 << idx);
}

/** \brief Remove uma carta do estado

@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/
MAO rem_carta(MAO est, int naipe, int valor){
	int idx = indice(naipe, valor);
	return est & ~((MAO) 1 << idx);
}

/** \brief Elimina todas as cartas de uma determinada seleção numa mao
@param mao 		Mao a que se vão retirar as cartas
@param selecao 	Seleçao de cartas que são retiradas
*/

MAO rem_cartas(MAO est, MAO rem){

	rem = ~rem;

	est = est & rem;

	return est;
}

/** \brief Adicionada todas as cartas de uma determinada seleção numa mao
@param mao 		Mao a que se vão adicionar as cartas
@param selecao 	Seleçao de cartas que são adicionadas
*/

MAO add_cartas(MAO est, MAO add){

	est |= add;

	return est; 

}


/** \brief Verifica se uma carta pertence ao estado

@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		1 se a carta existe e 0 caso contrário
*/
int carta_existe(MAO est, int naipe, int valor){
	int idx = indice(naipe, valor);
	return (est >> idx) & 1;
}
/** \brief Inicializa as informações do jogo.
@param JOGO Estado do jogo, onde o parametros vao ser todos colocados a zero.
@return INT 1, caso a função tenha corrido devidamente.
*/
int initiate (JOGO e){ 

	e->played = (MAO) 0;
	e->mao = (MAO) 0;
	e->bestCombo = (MAO) 0;
	e->passar = 0;
	e->nCartas = 0;

	return 1;
}