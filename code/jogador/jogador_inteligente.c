#include "jogador_inteligente.h"
#include "read.h"

/** \brief Devolve o índice da carta

@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O índice correspondente à carta
*/
int indice(int naipe, int valor) {
	return naipe * 13 + valor;
}

/** \brief Adiciona uma carta ao estado

@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/
long long int add_carta(long long int est, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return est | ((long long int) 1 << idx);
}

/** \brief Remove uma carta do estado

@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/
long long int rem_carta(long long int est, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return est & ~((long long int) 1 << idx);
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
int carta_existe(long long int est, int naipe, int valor){
	int idx = indice(naipe, valor);
	return (est >> idx) & 1;
}

int initiate (JOGO e){ /*Inicializa as informações do jogo*/

	int i;

	for (i = 0 ; i < 3 ; i++)
		e->played[i] = (MAO) 0;

	e->jogador = 0;
	e->passar = 0;
	e->eu = 0;

	return 1;
}