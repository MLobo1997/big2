#include "cartas.h"
#include "view.h"
#include "autoplay.h"
#include "verificaJogada.h"



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

/**	\brief Distribui aleatoriamente as 56 cartas pelos 4 jogadores e determina qual começa a jogar.
@param ESTADO Estado do jogo atual.
*/

ESTADO shuffle (ESTADO e){

	int naipe, valor, randN, i;

	srandom (time (NULL));

	for (naipe = 3 ; naipe >= 0 ; naipe--){

		for (valor = 12 ; valor >= 0 ; valor--){

			randN = random()%4;

			while (numCartas(e.mao[randN]) == 13) /*para o caso de o jogador ja ter as 13 cartas*/
				randN = random() % 4; 

			e.mao[randN] = add_carta(e.mao[randN] , naipe , valor);

		}

	}

	i = 0;

	while (~(e.mao[i]) & (MAO) 1) i++;

	e.jogador = i;

	return e;
}

/** \brief Escreve o estado através da script.
@param SCRIPT script
*/
ESTADO str2estado (char* str){
	ESTADO e;
	sscanf(str, FORMATO, &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.selecao, &e.action , &e.played[0] , &e.played[1] , &e.played[2] , &e.played[3], &e.jogador, &e.nCartas, &e.passar, &e.sort, &e.end);
	return e;
}

/** \brief Escreve a script através do estado.
@param ESTADO estado
*/
char* estado2str (ESTADO e){
	static char res[10240];
	sprintf(res, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.selecao, e.action , e.played[0] , e.played[1] , e.played[2] , e.played[3], e.jogador, e.nCartas, e.passar, e.sort, e.end);
	return res;
}


/** \brief Inicializa o estado, alterando todos os valores para 0.
@param ESTADO estado
*/
ESTADO inicializa (ESTADO e){

	int i;
    for (i = 0 ; i < 4 ; i++){
    	e.mao[i]=0;
    	e.played[i]=0;
    }

    e.action = 0;
    e.nCartas = 0;
    e.selecao = 0;
    e.passar = 0;
    e.sort = 0;
    e.end = 0;

    return e;
}