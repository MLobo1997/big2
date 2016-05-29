#include "jogador_inteligente.h"
#include "read.h"
#include "jogar.h"
#include "output.h"

/** \brief Recebendo uma mão APENAS com uma carta devolve o valor da mesma.
@param MAO Mão apenas com uma carta.
@return VALOR O valor da carta (inteiro entre 0 e 12)
*/
int returnValuelld (MAO mao){

	int valor, naipe;

	for (valor = 12 ; valor >= 0 ; valor--)
		for (naipe = 3 ; naipe >= 0 ; naipe--)
			if (carta_existe(mao , naipe , valor)) return valor;

	return -1;
}

/** \brief Recebendo uma mão APENAS com uma carta devolve o naipe da mesma.
@param MAO Mão apenas com uma carta.
@return NAIPE O naipe da carta (inteiro entre 0 e 3)
*/
int returnNaipelld (MAO mao){

	int valor, naipe;

	for (valor = 12 ; valor >= 0 ; valor--)
		for (naipe = 3 ; naipe >= 0 ; naipe--)
			if (carta_existe(mao , naipe , valor)) return naipe;

	return -1;
}


/** \brief subsitui os valores de dois elementos do array um pelo outro.
@param INT Índice do 1º elemento
@param INT Índice do 2º elemento
*/
void swap(int v[], int i, int j){

	int temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

/** \brief Particiona e troca os elementos de um array.
@param ARRAY Array a ser ordenado
@param INT Número de elementos
@return INT Posição de centro
*/
int partition (int v[], int N){

	int i = 0, p = 0;
	while (i < N - 1) {
		if (v[i] > v[N-1]) i++;
		else{
			swap (v , i , p);
			p++;
			i++;
		}
	}

	swap (v , p , N-1);
	return p;
}
/** \brief Ordena um array
@param ARRAY Array a ser ordenado.
@param INT Número de elementos do array.
*/
void QSort (int v[], int N){

	int p;
	if (N > 1){
		p = partition (v , N);

		QSort (v , p);
		QSort (&v[p+1], N-p-1);
	}
}

/** \brief Recebe dois valores de cartas e verifica se são seguidas
@param INT Valor nº 1.
@param INT Valor nº 2.
@return INT 1 se forem seguidos, 0 caso contrário.
*/
int verificaSeguidos (int n1 , int n2){

	int flag = 0;

	if (n1 + 1 == n2 || n1 == n2 + 1) flag = 1;

	if ((n1 == 12 && n2 == 0) || (n1 == 0 && n2 == 12)) flag = 1; /*Para o caso de uma ser um 3 e outra 2*/

	return flag;
}

/** \brief Identifica o número de cartas numa determinada mão.
@param MAO A mão.
@return INT Número de cartas
*/
int numCartas (MAO sel){


	int nCartas, count;

	for (nCartas = count = 0; count <= 52 ; sel = sel >> 1 , count++)
			if (sel & ((MAO) 1)) nCartas++;

	return nCartas;
}

/** \brief Recebe um valor de uma carta e devolve o valor anterior (no caso de ser um 3 devolve o valor de 2).

@param VALOR O valor da carta (inteiro entre 0 e 12)
@return VALORANTERIOR O valor da carta imediatamente inferior numa ordem funçao de valores (inteiro entre 0 e 12)
	
*/
int valorAnterior (int * valor){

	if (*valor == 0) *valor = 12;

	else (*valor)--;

	return *valor;
}

/** \brief Recebe um naipe de uma carta e devolve o valor anterior (no caso de ser um 3 devolve o naipe 2).
@param NAIPE O naipe da carta (inteiro entre 0 e 3)
@return NAIPEANTERIOR O valor da naipe imediatamente inferior numa ordem funçao de naipe (inteiro entre 0 e 3)
*/
int naipeAnterior (int * naipe){

	if (*naipe == 0) *naipe = 3;

	else (*naipe)--;

	return *naipe;
}
/** \brief Verifica se é possível formar um straight flush a partir de uma determinada carta.
@param MAO Mão a ser analisada.
@param INT Naipe a verificar.
@param INT Valor a partir do qual se vai verificar o straight flush (assumindo que é o máximo).
@return INT 1 caso possível, 0 caso contrário.
*/
int hasStraightFlush (MAO mao, int naipe, int valor){

	return (carta_existe (mao , naipe , valor) &&
			carta_existe (mao , naipe , valorAnterior(&valor)) &&
			carta_existe (mao , naipe , valorAnterior(&valor)) &&
			carta_existe (mao , naipe , valorAnterior(&valor)) &&
			carta_existe (mao , naipe , valorAnterior(&valor)));
}
/** \brief Adiciona um straight flush a partir de uma determinada carta.
@param MAO Mão a ser alterada.
@param INT Naipe a adicionar.
@param INT Valor a partir do qual se vai verificar o straight flush (assumindo que é o máximo).
@return MAO Mão alterada.
*/
MAO add_StraightFlush (MAO mao, int naipe, int valor){

	mao = add_carta (mao , naipe , valor);
	mao = add_carta (mao , naipe , valorAnterior(&valor));
	mao = add_carta (mao , naipe , valorAnterior(&valor));
	mao = add_carta (mao , naipe , valorAnterior(&valor));
	mao = add_carta (mao , naipe , valorAnterior(&valor));

	return mao;
}
/** \brief Tenta jogar um Straight Flush.
@param JOGO Estado do jogo.
@param NAIPE Naipe mínimo que pode ser jogado dentro do valor.
@param VALOR Valor mínimo a ser jogado.
@return MAO Mão vazia ou com straight flush.
*/
MAO jogaStraightFlush (JOGO e, int naipe, int valor){

	MAO mao = (MAO) 0, tmp = e->mao;

	while (naipe < 4 && mao == (MAO) 0){
		if (hasStraightFlush (tmp, naipe , valor)){

			mao = add_StraightFlush(mao, naipe , valor);
			tmp = rem_cartas(tmp, mao);
		}
		naipe++;
	}

	for (valor++ ; valor < 13 && mao == (MAO) 0 ; valor++)
		for (naipe = 0 ; naipe < 4 && mao == (MAO) 0 ; naipe++)
			if (hasStraightFlush (tmp , naipe , valor)){

			mao = add_StraightFlush (mao , naipe , valor);
			tmp = rem_cartas (tmp , mao);
			}
	
	return mao;

}

/** \brief Adiciona um four of a kind.
@param MAO Mão a ser alterada.
@param INT Valor a ser adicionado.
@return MAO Mão já alterada.
*/
MAO add_FourofaKind (MAO mao, int valor){
	mao = add_carta (mao , 0 , valor);
	mao = add_carta (mao , 1 , valor);
	mao = add_carta (mao , 2 , valor);
	mao = add_carta (mao , 3 , valor);
	

	return mao;


}
/** \brief Adiciona a carta mais baixa da mão.
@param MAO Mão a ser retirada carta.
@param MAO Mão a ser adicionada a carta.
@return MAO Mão já alterada.
*/
MAO add_valormaisbaixo (MAO mao,MAO maoplayed){

	int valor = 0, naipe = 0;

	while (valor < 13 && !carta_existe(mao, naipe, valor)){
		for (naipe = 0 ; naipe < 4 && !carta_existe(mao, naipe, valor); naipe++);
		if (!carta_existe(mao, naipe, valor)) valor++;
	}

	maoplayed = add_carta(maoplayed, naipe, valor);


	return maoplayed;
}

/** \brief Tenta jogar um four of a kind.
@param JOGO Estado do jogo.
@param NAIPE Naipe mínimo que pode ser jogado dentro do valor.
@param VALOR Valor mínimo a ser jogado.
@return MAO Mão vazia ou com four of a kind.
*/
MAO jogaFourofaKind (JOGO e, int naipe, int valor){

	MAO mao = (MAO) 0, tmp = e->mao;

	int numc = numCartas (tmp); 

	if (numc >= 5){
		while (valor < 13 && mao == (MAO) 0 && naipe < 4){
	 		for (naipe = 0 ; naipe < 4 && carta_existe (tmp , naipe , valor) ; naipe++);
	 		if (naipe < 4) valor++;
	 	}


		if (naipe == 4){
		 	mao = add_FourofaKind (mao , valor);
			tmp = rem_cartas (tmp , mao);

			mao = add_valormaisbaixo (tmp, mao);
			tmp= rem_cartas (tmp , mao);
		}
	}

	return mao;

}

/** \brief Calcula o menor valor superior para um determinado four of a kind.
@param MAO Four of a kind a ser analisada.
@param CARTA transportadora de resultado.
@return CARTA Valor e naipe mínimo a ser jogado.
*/

CARTA fourOfAKindValue (MAO mao, CARTA card){

	int valor, naipe, counter = 0;

	for (valor = 12 ; valor >= 0 && counter < 4 ; valor--)
		for (naipe = 3, counter = 0 ; naipe >= 0 && carta_existe(mao, naipe, valor) ; naipe--, counter++);

	card.valor = valor + 2;
	card.naipe = 0;

	return card;
}
/** \brief Verifica se uma determinada jogada é um four of a kind.
@param MAO jogada de 5 cartas.
@return BOOL 1 caso a jogada seja um four of a kind, 0 caso contrário.
*/
int isFourOfAKind (MAO mao){

	int valor, naipe, counter = 0;

	for (valor = 0 ; valor < 13 && counter != 4 ; valor++)
		for (naipe = counter = 0 ; naipe < 4 && carta_existe (mao, naipe, valor) ; naipe++, counter++);

	return (counter == 4);

}
/** \brief Tenta jogar um full house.
@param JOGO Estado do jogo.
@param NAIPE Naipe mínimo que pode ser jogado dentro do valor.
@param VALOR Valor mínimo a ser jogado.
@return MAO Mão vazia ou com full house.
*/
MAO jogaFullHouse(JOGO e, int naipe, int valor){ /* (3,2) do mm valor */

	int counter;
	MAO backup = e->mao, tmp = (MAO) 0;
	MAO mao = (MAO) 0;


	for (counter = 0; valor < 13  && mao == (MAO) 0 ; valor++){
		for (naipe = 0, counter = 0; naipe < 4 && counter != 3; naipe++)
		 	if (carta_existe (e->mao , naipe , valor)){
		 		
		 		mao = add_carta (mao , naipe , valor);
				counter++;
			}	

		if (counter != 3) mao = (MAO) 0;
	}


	e->mao = rem_cartas(e->mao , mao);
	
	for (valor = 0, counter = 0, naipe = 0; valor < 13 && mao != (MAO) 0 && tmp == (MAO) 0; valor++){
		for (naipe = 0, counter = 0; naipe < 4 && counter < 2 ; naipe++)
	 		if (carta_existe(e->mao , naipe , valor)){
	 		
	 			tmp = add_carta(tmp , naipe , valor);
				counter++;
			}

		if (counter != 2) tmp = (MAO) 0;
	}

	if (tmp != (MAO) 0){
		mao = add_cartas(mao, tmp);
		e->mao = rem_cartas(e->mao, mao);
	}

	else mao = (MAO) 0;

	e->mao = backup;

	return mao;
}

/** \brief Verifica se uma determinada jogada é um full house.
@param MAO jogada de 5 cartas.
@return BOOL 1 caso a jogada seja um full house, 0 caso contrário.
*/
int isFullHouse (MAO mao){

	int i = 0, valores[5], valor, naipe;

	for (naipe = 0 ; naipe < 4 ; naipe++)
		for (valor = 0 ; valor < 13 ; valor++)
			if (carta_existe (mao, naipe, valor)) valores[i] = valor, i++;

	QSort (valores , 5);

	return ((valores[0] == valores[1] &&
			 valores[2] == valores[3] &&
			 valores[3] == valores[4])||
			(valores[0] == valores[1] &&
			 valores[1] == valores[2] &&
			 valores[3] == valores[4]));

}
/** \brief Calcula o menor valor superior para um determinado full house.
@param MAO Full house a ser analisada.
@param CARTA transportadora de resultado.
@return CARTA Valor e naipe mínimo a ser jogado.
*/
CARTA fullHouseValue(MAO mao, CARTA card){

	int valor = 12, naipe = 3, counter = 0;

	while (valor >= 0 && counter < 3){
		for (naipe = 3, counter = 0; naipe >= 0; naipe--)
			if (carta_existe(mao, naipe, valor)) counter++;

		if (counter < 3) valor--;
	}

	card.valor = ++valor;
	card.naipe = 0;

	return card;

}

/** \brief Verifica se uma determinada jogada é um flush.
@param MAO jogada de 5 cartas.
@return BOOL 1 caso a jogada seja um flush, 0 caso contrário.
*/
int isFlush (MAO mao){

	int naipes[5], i = 0, valor, naipe;

	for (naipe = 0 ; naipe < 4 ; naipe++)
		for (valor = 0 ; valor < 13 ; valor++)
			if (carta_existe(mao , naipe, valor)) naipes[i] = naipe, i++;

	for (i = 0 ; i + 1 < 5 && (naipes[i] == naipes[i + 1]) ; i++);

	return (i == 4);

}

/** \brief Tenta jogar um flush.
@param JOGO Estado do jogo.
@param NAIPE Naipe mínimo que pode ser jogado dentro do valor.
@param VALOR Valor mínimo a ser jogado.
@return MAO Mão vazia ou com flush.
*/
MAO jogaFlush (JOGO e, int naipe, int valor){ /* 5 cartas do mesmo naipe */

	int i = 0; int valortmp; int verifica = 0;

	MAO mao = (MAO) 0, backup = e->mao;

	if(carta_existe(e->mao, 0, 0))/*Para o caso de estar a abrir o jogo*/
		for ( ; naipe < 4 && mao == 0 ; naipe++){ 
			for (i = 0, valor = 0; valor < 13 && i < 5 ; valor++)
				if (carta_existe(e->mao , naipe , valor)){

					mao= add_carta(mao, naipe, valor); 
					i++;
				}
		
			if (i < 5) mao = 0;
			else e->mao = rem_cartas(e->mao , mao);
		}	  	


	else{

		for (valortmp = 12 ; valortmp >= 0 && i < 5 ; valortmp--){	/* verifica o naipe q recebe */

			if (carta_existe (e->mao , naipe , valortmp)){

				mao= add_carta(mao, naipe, valortmp); 
				i++;
				if (valortmp >= valor) verifica = 1;
			}	
		}

		if (i == 5 && verifica) e->mao = rem_cartas(e->mao , mao);
		else mao = 0;

			

		for (naipe++ ; naipe < 4 && mao == 0 ; naipe++){  /* verifica os naipes seguintes */
			for (i = 0, valor = 0; valor < 13 && i < 5 ; valor++)
				if (carta_existe(e->mao , naipe , valor)){

					mao= add_carta(mao, naipe, valor); 
					i++;
				}
		
			if (i < 5) mao = 0;
			else e->mao = rem_cartas(e->mao , mao);
		}
	}

	e->mao = backup;

	return mao;
}

/** \brief Calcula o menor valor superior para um determinado flush.
@param MAO Flush a ser analisada.
@param CARTA transportadora de resultado.
@return CARTA Valor e naipe mínimo a ser jogado.
*/
CARTA flushValue (MAO mao, CARTA card){

	int naipe = 3, valor = 12;

 	while (valor >= 0 && !carta_existe(mao, naipe, valor)){
		for (naipe = 3 ; naipe >= 0 && !carta_existe(mao, naipe, valor) ; naipe--);
 		
 		if (naipe < 0) valor--;
 	}

 	if (valor == 12) valor = 0, naipe++;

 	else valor++;

 	card.valor = valor;

 	card.naipe = naipe;

	return card;
}
/** \brief Recebe um valor e um naipe e tenta verifica se consegue jogar alguma sequência em que o elemento superior tenha aquele valor e aquele naipe ou um superior.
@param MAO Mão que vai ser lida.
@param NAIPE Naipe mínimo a ser jogado.
@param VALOR Valor mínimo a ser jogado.
@return MAO Mão com uma sequência ou vazia.
*/
MAO addStraight (MAO mao, int naipe, int valor){

	int counter;

	MAO new = (MAO) 0;

	for (counter = 5 ; naipe < 4 && !carta_existe(mao, naipe, valor) ; naipe++);

	if (naipe < 4) new = add_carta(new, naipe, valor);

	for (valorAnterior(&valor), counter-- ; counter && naipe < 4 ; valorAnterior(&valor)){

		for (naipe = 0; naipe < 4 && !carta_existe(mao, naipe, valor); naipe++);

		if (naipe < 4) new = add_carta(new, naipe, valor), counter--;
	}

	if (counter) new = (MAO) 0;

	return new;
}
/** \brief Tenta jogar uma sequência.
@param JOGO Estado do jogo.
@param NAIPE Naipe mínimo que pode ser jogado dentro do valor.
@param VALOR Valor mínimo a ser jogado.
@return MAO Mão vazia ou com straight.
*/
MAO jogaStraight (JOGO e, int naipe, int valor){

	MAO mao = (MAO) 0, backup = e->mao;

	while (naipe < 4 && mao == (MAO) 0){
		mao = addStraight(e->mao, naipe, valor);

		naipe++;
	}

	for (valor++ ; mao == (MAO) 0 && valor < 13 ; valor++)
		mao = addStraight(e->mao, 0, valor);

	if (mao != (MAO) 0) e->mao = rem_cartas(e->mao, mao);

	e->mao = backup;

	return mao;
}

/** \brief Verifica se uma determinada jogada é uma sequência.
@param MAO jogada de 5 cartas.
@return BOOL 1 caso a jogada seja uma sequência, 0 caso contrário.
*/

int isStraight (MAO mao){

	int i, n = 0, naipe, valor, valores[5], flag = 0, tmp[5], ntmp;

	for (naipe = 0 ; naipe < 4 ; naipe++)
		for (valor = 0 ; valor < 13 ; valor++)
			if (carta_existe (mao , naipe , valor)) valores[n] = valor, n++;

	QSort (valores , 5);

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
/** \brief Calcula o menor valor superior a uma determinada sequência (incluindo straightflush's).
@param MAO Sequência a ser analisada.
@param CARTA transportadora de resultado.
@return CARTA Valor e naipe mínimo a ser jogado.
*/

CARTA straightValue (MAO mao, CARTA card){

	int naipe = 0, valor = 0;

	while (valor < 13 && !carta_existe(mao, naipe, valor)){
		for (naipe = 0; naipe < 4 && !carta_existe(mao, naipe, valor) ; naipe++);
		if (!carta_existe(mao, naipe, valor)) valor++;
	}

	while (carta_existe(mao, naipe, valor)){
		for (valor++, naipe = 0 ; naipe < 4 && !carta_existe(mao, naipe, valor) ; naipe++);
		if (carta_existe(mao, naipe, valor)) card.naipe = naipe, card.valor = valor;
	}

	if (card.naipe == 3) card.naipe = 0, card.valor++;

	else card.naipe++;

	return card;
}

/** \brief Recebe uma MAO e dá a média geométrica tendo em conta os valores e os naipes das cartas
@param MAO A mão da qual queremos a média.
@return DOUBLE A média geométrica da mão.
*/
double mediaMao (MAO mao){

	int valor, naipe; double soma,isoma; soma = 1; isoma = 0;

	for (naipe = 0 ; naipe < 4 ; naipe++){
		for (valor = 0 ; valor < 13 ; valor++){
			if (carta_existe(mao , naipe , valor)) {
				soma *=(valor)+((naipe+1)*0.25);
				isoma++;
			}
		}
	}
	return (pow(soma,(1/isoma))); /*Média geométrica*/
}

/** \brief Calcula de várias mãos a que tem uma média de valores mais baixa.
@param ARRAY Array de no máximo de 5 mãos.
@param INT Número de arrays.
@return INT Índice do elemento do array com média de valores mais baixa.
*/
int melhorCombinacao (MAO mao[5], int N){
	double valor = 12;
	int i, r = 0;

	for(i = 0 ; i < N ; i++){
		if (mediaMao(mao[i]) < valor)
			valor = mediaMao(mao[i]), r = i;
	}

	return r;
}

/** \brief	Tenta fazer a jogada de 5 cartas com valores mais baixos possível.
@param JOGO Estado a ser alterado.
@return MAO Mão vazia ou com combinação.
*/

MAO joga5(JOGO e){

	MAO mao[5] =  {(MAO) 0}; /*Onde serão colocadas todas a jogadas de 5 cartas possíveis*/

	CARTA card;

	int i = 0;

	if (isStraight(e->played) && isFlush(e->played)){ /*Se for straight flush*/
		card = straightValue(e->played, card);
		mao[i] = jogaStraightFlush(e, card.naipe, card.valor); /*Tenta jogar straight flush*/
	
		if (mao[i] != (MAO) 0) i++; /*Se tiver sido encontrado uma jogada passa ao próximo elemento de jogadas possíveis.*/
	}


	if (isFourOfAKind(e->played)){ /*Se for four of a kind*/ 

		mao[i] = jogaStraightFlush(e, 0, 0); /*Tenta jogar straight flush*/

		if (mao[i] != (MAO) 0) i++; 

		card = fourOfAKindValue(e->played, card);
		mao[i] = jogaFourofaKind(e, card.naipe, card.valor); /*Tenta jogar four of a kind*/

		if (mao[i] != (MAO) 0) i++;

		
	}

	if (isFullHouse(e->played)){ /*Se for full house*/

		mao[i] = jogaStraightFlush(e, 0, 0); /*Tenta jogar straight flush*/

		if (mao[i] != (MAO) 0) i++; 

		mao[i] = jogaFourofaKind(e, 0, 0); /*Tenta jogar four of a kind*/

		if (mao[i] != (MAO) 0) i++; 

		card = fullHouseValue(e->played, card);
		mao[i] = jogaFullHouse(e, card.naipe, card.valor); /*Tenta jogar full house*/

		if (mao[i] != (MAO) 0) i++;

		
	}

	if (isFlush(e->played) && !isStraight(e->played)){ /*Se for só flush*/

		mao[i] = jogaStraightFlush(e, 0, 0); /*Tenta jogar straight flush*/

		if (mao[i] != (MAO) 0) i++; 

		mao[i] = jogaFourofaKind(e, 0, 0); /*Tenta jogar four of a kind*/

		if (mao[i] != (MAO) 0) i++; 

		mao[i] = jogaFullHouse(e, 0, 0); /*Tenta jogar full house*/

		if (mao[i] != (MAO) 0) i++;

		card = flushValue(e->played, card);
		mao[i] = jogaFlush(e, card.naipe, card.valor); /*Tenta jogar flush*/

		if (mao[i] != (MAO) 0) i++;

	}

	if (!isFlush(e->played) && isStraight(e->played)){ /*Se for só straight*/

		mao[i] = jogaStraightFlush(e, 0, 0); /*Tenta jogar straight flush*/

		if (mao[i] != (MAO) 0) i++; 

		mao[i] = jogaFourofaKind(e, 0, 0); /*Tenta jogar four of a kind*/

		if (mao[i] != (MAO) 0) i++; 

		mao[i] = jogaFullHouse(e, 0, 0); /*Tenta jogar full house*/

		if (mao[i] != (MAO) 0) i++;

		mao[i] = jogaFlush(e, 0, 0); /*Tenta jogar flush*/

		if (mao[i] != (MAO) 0) i++;

		card = straightValue(e->played, card);
		mao[i] = jogaStraight(e, card.naipe, card.valor); /*Tenta jogar straight*/

		if (mao[i] != (MAO) 0) i++;
	}

	i = melhorCombinacao(mao, i); /*Calcula qual o índice da melhor a ser jogada*/

	return mao[i];
}

/** \brief Joga 3 cartas conforme o jogo.
@param INT Valor mínimo a ser jogado.
@return MAO Mão vazia ou com um trio.
*/
MAO jogaTrio (MAO tmp, int valor){
	
	int counter;
	MAO mao = (MAO) 0;
	int naipe;

	for (counter = 0 ; valor < 13  && mao == (MAO) 0 ; valor++){
		for (naipe = 0, counter = 0; naipe < 4 && counter != 3; naipe++)
		 	if (carta_existe (tmp , naipe , valor)){
		 		
		 		mao = add_carta (mao , naipe , valor);
				counter++;
			}	

		if (counter != 3) mao = (MAO) 0;
	}

	return mao;

}
/** \brief Joga 3 cartas conforme o jogo.
@param JOGO Estado do jogo.
@return MAO Mão vazia ou com um trio.
*/
MAO joga3 (JOGO e){

	int minValue = returnValuelld (e->played);

	return jogaTrio(e->mao, minValue + 1);
}

/** \brief Joga 3 cartas conforme o jogo.
@param INT Valor mínimo a ser jogado.
@param INT Naipe mínimo a ser jogado.
@return MAO Mão vazia ou com um trio.
*/
MAO jogaPar (MAO mao, int valor, int minNaipe){

	int counter = 0, flag = 1, naipe;
	MAO tmp = (MAO) 0;

	for (naipe = 3 ; counter < 2 && naipe >= 0 ; naipe--)
		if (carta_existe(mao , naipe , valor)){
	 			if (naipe >= minNaipe) flag = 0;
	 			tmp = add_carta(tmp , naipe , valor);
				counter++;
		}

	if (flag || counter != 2) tmp = (MAO) 0;

	for (valor++ ; valor < 13 && tmp == (MAO) 0; valor++){
		for (naipe = counter = 0; naipe < 4 && counter < 2 ; naipe++)
	 		if (carta_existe(mao , naipe , valor)){
	 		
	 			tmp = add_carta(tmp , naipe , valor);
				counter++;
			}

		if (counter != 2) tmp = (MAO) 0;
	}

	return tmp;
}

/** \brief Joga 2 cartas conforme o jogo.
@param JOGO Estado do jogo.
@return MAO Mão vazia ou com um par.
*/
MAO joga2 (JOGO e){

	int minValue = returnValuelld(e->played);
	int minNaipe = returnNaipelld(e->played); 

	return jogaPar(e->mao, minValue, minNaipe + 1);
}

/** \brief Joga 1 cartas conforme o jogo.
@param JOGO Estado do jogo atual.
@param INT Valor mínimo a ser jogado.
@param INT Naipe mínimo a ser jogado.
@return MAO Mão vazia ou com uma carta.
*/
MAO jogaUma (JOGO e, int valor, int naipe){

	MAO mao = (MAO) 0;

	while (naipe < 4 && mao == (MAO) 0){
		if (carta_existe(e->mao, naipe, valor) && !carta_existe(e->bestCombo, naipe, valor))
			mao = add_carta(mao, naipe, valor);
		naipe++;
	}

	for (valor++ ; valor < 13 && mao == (MAO) 0 ; valor++)
		for (naipe = 0 ; naipe < 4 && mao == (MAO) 0 ; naipe++)
			if (carta_existe(e->mao, naipe, valor) && !carta_existe(e->bestCombo, naipe, valor))
				mao = add_carta(mao, naipe, valor);

	return mao;
}

/** \brief Joga 1 carta conforme o jogo.
@param JOGO Estado do jogo.
@return MAO Mão vazia ou com uma carta.
*/
MAO joga1 (JOGO e){

	int minValue = returnValuelld(e->played);
	int minNaipe = returnNaipelld(e->played); 

	return jogaUma(e, minValue, minNaipe + 1);
}

/** \brief Abre um jogo, quando não existem cartas em jogo.
@param JOGO Estado do jogo
@return MAO Mão do jogo
*/
MAO abreJogo (JOGO e){

	/*Primeiro tenta jogar combinações de 5*/
	MAO maos[5] = {(MAO) 0}, mao = (MAO) 0; 

	int i = 0;

	if (carta_existe(e->mao, 0, 0)){ /*No caso de o 3 de ouros existir na mão*/

		maos[i] = jogaStraight(e, 0, 0);

		if (!carta_existe(maos[i], 0, 0)) maos[i] = (MAO) 0; 

		else i++;

		maos[i] = jogaFlush(e, 0, 0);

		if (!carta_existe(maos[i], 0, 0)) maos[i] = (MAO) 0; 

		else i++; 

		maos[i] = jogaFullHouse(e, 0, 0);

		if (!carta_existe(maos[i], 0, 0)) maos[i] = (MAO) 0; 

		else i++; 

		maos[i] = jogaFourofaKind(e, 0, 0);

		if (!carta_existe(maos[i], 0, 0)) maos[i] = (MAO) 0; 

		else i++; 

		maos[i] = jogaStraightFlush(e, 0, 0);

		if (!carta_existe(maos[i], 0, 0)) maos[i] = (MAO) 0;

		else i++;

		i = melhorCombinacao(maos, i); /*Calcula qual o índice da melhor a ser jogada*/

		mao = maos[i];

		e->nCartas = 5;
		/*Se não conseguir tenta trio*/
		if (!carta_existe(mao, 0, 0)) mao = jogaTrio(e->mao, 0), e->nCartas = 3;
		/*Se não conseguir tenta par*/
		if (!carta_existe(mao, 0, 0)) mao = jogaPar(e->mao, 0, 0), e->nCartas = 2;
		/*Se não conseguir tenta uma carta*/
		if (!carta_existe(mao, 0, 0)) mao = jogaUma(e, 0, 0), e->nCartas = 1;

	}

	else{	

		maos[i] = jogaStraightFlush(e, 0, 0); /*Tenta jogar straight flush*/

		if (maos[i] != (MAO) 0) i++; 

		maos[i] = jogaFourofaKind(e, 0, 0); /*Tenta jogar four of a kind*/

		if (maos[i] != (MAO) 0) i++; 

		maos[i] = jogaFullHouse(e, 0, 0); /*Tenta jogar full house*/

		if (maos[i] != (MAO) 0) i++;

		maos[i] = jogaFlush(e, 0, 0); /*Tenta jogar flush*/

		if (maos[i] != (MAO) 0) i++;

		maos[i] = jogaStraight(e, 0, 0); /*Tenta jogar straight*/

		if (maos[i] != (MAO) 0) i++;

		i = melhorCombinacao(maos, i); /*Calcula qual o índice da melhor a ser jogada*/

		mao = maos[i];

		e->nCartas = 5;
		/*Se não conseguir tenta trio*/
		if (mao == (MAO) 0) mao = jogaTrio(e->mao, 0), e->nCartas = 3;
		/*Se não conseguir tenta par*/
		if (mao == (MAO) 0) mao = jogaPar(e->mao, 0, 0), e->nCartas = 2;
		/*Se não conseguir tenta uma carta*/
		if (mao == (MAO) 0) mao = jogaUma(e, 0, 0), e->nCartas = 1;
	}

	return mao;
}

/** \brief Joga.
@param Jogo Estado do jogo atual.
@return	1 se a carta existe e 0 caso contrário.
*/
int jogar (JOGO e){

	MAO mao = (MAO) 0;

	if (e->nCartas == 5) mao = joga5(e);

	if (e->nCartas == 3) mao = joga3(e);

	if (e->nCartas == 2) mao = joga2(e);

	if (e->nCartas == 1) mao = joga1(e);

	if (e->nCartas == 0) mao = abreJogo(e);

	if (mao != (MAO) 0){
		e->mao = rem_cartas(e->mao, mao);
		e->played = mao;
		e->passar = 0;
		jogaOut(mao, e->nCartas);

		if ((e->bestCombo & mao) != (MAO) 0) e->bestCombo = 0; /*No caso de ter sido jogada alguma carta do bestCombo este é eliminado*/
	}

	else{
		if (e->passar == 2){ /*Se for a 3ª passagem consecutiva, os parâmetros de jogada são reinicializados*/
			e->passar = 0; 
			e->nCartas = 0;
			e->played = (MAO) 0;
		}
		else e->passar++; /*Senão o e->passar aumenta*/

		passarOut();
	}


	
	return 1;
}