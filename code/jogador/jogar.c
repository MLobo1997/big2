#include "jogador_inteligente.h"
#include "read.h"
#include "jogar.h"

void swap(int v[], int i, int j){

	int temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}


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

/** \brief Recebe um nai+e de uma carta e devolve o valor anterior (no caso de ser um 3 devolve o naipe 2).

@param NAIPE O naipe da carta (inteiro entre 0 e 3)
@return NAIPEANTERIOR O valor da naipe imediatamente inferior numa ordem funçao de naipe (inteiro entre 0 e 3)
	
*/
int naipeAnterior (int * naipe){

	if (*naipe == 0) *naipe = 3;

	else (*naipe)--;

	return *naipe;
}

int hasStraightFlush (MAO mao, int naipe, int valor){

	return (carta_existe (mao , naipe , valor) &&
			carta_existe (mao , naipe , valorAnterior(&valor)) &&
			carta_existe (mao , naipe , valorAnterior(&valor)) &&
			carta_existe (mao , naipe , valorAnterior(&valor)) &&
			carta_existe (mao , naipe , valorAnterior(&valor)));
}

MAO add_StraightFlush (MAO mao, int naipe, int valor){

	mao = add_carta (mao , naipe , valor);
	mao = add_carta (mao , naipe , valorAnterior(&valor));
	mao = add_carta (mao , naipe , valorAnterior(&valor));
	mao = add_carta (mao , naipe , valorAnterior(&valor));
	mao = add_carta (mao , naipe , valorAnterior(&valor));

	return mao;
}

MAO jogaStraightFlush (JOGO e, int naipe, int valor){

	MAO mao = (MAO) 0;

	while (naipe < 4 && mao == (MAO) 0){
		if (hasStraightFlush (e->mao, naipe , valor)){

			mao = add_StraightFlush(mao, naipe , valor);
			e->mao = rem_cartas(e->mao, mao);
		}
		naipe++;
	}

	for (valor++ ; valor < 13 && mao == (MAO) 0 ; valor++)
		for (naipe = 0 ; naipe < 4 && mao == (MAO) 0 ; naipe++)
			if (hasStraightFlush (e->mao , naipe , valor)){

			mao = add_StraightFlush (mao , naipe , valor);
			e->mao = rem_cartas (e->mao , mao);
			}
	
	return mao;

}

MAO add_FourofaKind (MAO mao, int valor){
	mao = add_carta (mao , 0 , valor);
	mao = add_carta (mao , 1 , valor);
	mao = add_carta (mao , 2 , valor);
	mao = add_carta (mao , 3 , valor);
	

	return mao;


}

MAO add_valormaisbaixo (MAO mao,MAO maoplayed){

	int valor = 0, naipe = 0;

	while (valor < 13 && !carta_existe(mao, naipe, valor)){
		for (naipe = 0 ; naipe < 4 && !carta_existe(mao, naipe, valor); naipe++);
		if (!carta_existe(mao, naipe, valor)) valor++;
	}

	maoplayed = add_carta(maoplayed, naipe, valor);


	return maoplayed;
}


MAO jogaFourofaKind (JOGO e, int naipe, int valor){

	MAO mao = (MAO) 0;

	int numc = numCartas (e->mao); 

	if (numc >= 5){
		while (valor < 13 && mao == (MAO) 0 && naipe < 4){
	 		for (naipe = 0 ; naipe < 4 && carta_existe (e->mao , naipe , valor) ; naipe++);
	 		if (naipe < 4) valor++;
	 	}


		if (naipe == 4){
		 	mao = add_FourofaKind (mao , valor);
			e->mao = rem_cartas (e->mao , mao);

			mao = add_valormaisbaixo (e->mao, mao);
			e->mao = rem_cartas (e->mao , mao);
		}
	}

	return mao;

}

CARTA fourOfAKindValue (MAO mao, CARTA card){

	int valor, naipe, counter = 0;

	for (valor = 12 ; valor >= 0 && counter < 4 ; valor--)
		for (naipe = 3, counter = 0 ; naipe >= 0 && carta_existe(mao, naipe, valor) ; naipe--, counter++);

	card.valor = valor + 2;
	card.naipe = 0;

	return card;
}

int isFourOfAKind (MAO mao){

	int valor, naipe, counter = 0;

	for (valor = 0 ; valor < 13 && counter != 4 ; valor++)
		for (naipe = counter = 0 ; naipe < 4 && carta_existe (mao, naipe, valor) ; naipe++, counter++);

	return (counter == 4);

}

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
	else e->mao = backup, mao = (MAO) 0;

	return mao;
}

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
@param JOGO Estado do jogo em que vão ser feitas as jogadas.
@param NAIPE Naipe mínimo que pode ser jogado dentro do valor.
@param VALOR Valor mínimo a ser jogado.
@return JOGO Estado pós as eventuais alterações.
*/
MAO jogaFlush (JOGO e, int naipe, int valor){ /* 5 cartas do mesmo naipe */

	int i = 0; int valortmp; int verifica = 0;

	MAO mao = (MAO) 0;

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
@param ESTADO Estado do jogo em que vão ser feitas as jogadas.
@param NAIPE Naipe mínimo que pode ser jogado dentro do valor.
@param VALOR Valor mínimo a ser jogado.
@return ESTADO Estado pós as eventuais alterações.
*/
MAO jogaStraight (JOGO e, int naipe, int valor){

	MAO mao = (MAO) 0;

	while (naipe < 4 && mao == (MAO) 0){
		mao = addStraight(e->mao, naipe, valor);

		naipe++;
	}

	for (valor++ ; mao == (MAO) 0 && valor < 13 ; valor++)
		mao = addStraight(e->mao, 0, valor);

	if (mao != (MAO) 0) e->mao = rem_cartas(e->mao, mao);

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



int joga5(JOGO e){

	MAO mao = (MAO) 0;

	CARTA card;

	if (isStraight(e->played) && isFlush(e->played)){ /*Se for straight flush*/
		card = straightValue(e->played, card);
		mao = jogaStraightFlush(e, card.naipe, card.valor); /*Tenta jogar straight flush*/
	}

	if (mao == (MAO) 0 && isFourOfAKind(e->played)){ /*Se for four of a kind*/ 

		mao = jogaStraightFlush(e, 0, 0); /*Tenta jogar straight flush*/

		if (mao == (MAO) 0){
			card = fourOfAKindValue(e->played, card);
			mao = jogaFourofaKind(e, card.naipe, card.valor); /*Tenta jogar four of a kind*/
		}
	}

	return mao;

}

/** \brief Joga.
@param Jogo e.
@return	1 se a carta existe e 0 caso contrário.
*/
int jogar (JOGO e){

	if (e->nCartas == 5) joga5(e);

	return 1;
}