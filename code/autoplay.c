#include "cartas.h"
#include "view.h"
#include "autoplay.h"
#include "verificaJogada.h"

/** \brief Recebendo uma mão APENAS com uma carta devolve o valor da mesma.
@param MAO Mão apenas com uma carta.
@return VALOR O valor da carta (inteiro entre 0 e 12)
*/
int returnValuelld (MAO mao){

	int valor, naipe;

	for (naipe = 0 ; naipe < 4 ; naipe++)
		for (valor = 0 ; valor < 13 ; valor++)
			if (carta_existe(mao , naipe , valor)) return valor;

	return -1;
}

/** \brief Recebendo uma mão APENAS com uma carta devolve o naipe da mesma.
@param MAO Mão apenas com uma carta.
@return NAIPE O naipe da carta (inteiro entre 0 e 3)
*/
int returnNaipelld (MAO mao){

	int valor, naipe;

	for (naipe = 0 ; naipe < 4 ; naipe++)
		for (valor = 0 ; valor < 13 ; valor++)
			if (carta_existe(mao , naipe , valor)) return naipe;

	return -1;
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

ESTADO jogaStraightFlush (ESTADO e , int naipe, int valor){


	while (naipe < 4 && e.played[e.jogador] == (MAO) 0){
		if (hasStraightFlush (e.mao[e.jogador] , naipe , valor)){

			e.played[e.jogador] = add_StraightFlush (e.played[e.jogador] , naipe , valor);
			e.mao[e.jogador] = rem_cartas (e.mao[e.jogador] , e.played[e.jogador]);
		}
		naipe++;
	}

	for (valor++ ; valor < 13 && e.played[e.jogador] == (MAO) 0 ; valor++)
		for (naipe = 0 ; naipe < 4 && e.played[e.jogador] == (MAO) 0 ; naipe++)
			if (hasStraightFlush (e.mao[e.jogador] , naipe , valor)){

			e.played[e.jogador] = add_StraightFlush (e.played[e.jogador] , naipe , valor);
			e.mao[e.jogador] = rem_cartas (e.mao[e.jogador] , e.played[e.jogador]);
			}
	
	return e;

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


ESTADO jogaFourofaKind (ESTADO e, int naipe, int valor){

	int numc = numCartas (e.mao[e.jogador]); /* redundante?*/

	if (numc >= 5){
		while (valor < 13 && e.played[e.jogador] == (MAO) 0 && naipe < 4){
	 		for (naipe = 0 ; naipe < 4 && carta_existe (e.mao[e.jogador] , naipe , valor) ; naipe++);
	 		if (naipe < 4) valor++;
	 	}


		if (naipe == 4){
		 	e.played[e.jogador] = add_FourofaKind (e.played[e.jogador] , valor);
			e.mao[e.jogador] = rem_cartas (e.mao[e.jogador] , e.played[e.jogador]);

			e.played[e.jogador] = add_valormaisbaixo (e.mao[e.jogador], e.played[e.jogador]);
			e.mao[e.jogador] = rem_cartas (e.mao[e.jogador] , e.played[e.jogador]);
		}
	}

	return e;

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

ESTADO jogaFullHouse(ESTADO e, int naipe, int valor){ /* (3,2) do mm valor */

	int counter;
	MAO backup = e.mao[e.jogador], tmp = (MAO) 0;


	for (counter = 0; valor < 13  && e.played[e.jogador] == (MAO) 0 ; valor++){
		for (naipe = 0, counter = 0; naipe < 4 && counter != 3; naipe++)
		 	if (carta_existe (e.mao[e.jogador] , naipe , valor)){
		 		
		 		e.played[e.jogador] = add_carta (e.played[e.jogador] , naipe , valor);
				counter++;
			}	

		if (counter != 3) e.played [e.jogador] = (MAO) 0;
	}


	e.mao[e.jogador] = rem_cartas(e.mao[e.jogador] , e.played[e.jogador]);
	
	for (valor = 0, counter = 0, naipe = 0; valor < 13 && e.played[e.jogador] != (MAO) 0 && tmp == (MAO) 0; valor++){
		for (naipe = 0, counter = 0; naipe < 4 && counter < 2 ; naipe++)
	 		if (carta_existe(e.mao[e.jogador] , naipe , valor)){
	 		
	 			tmp = add_carta(tmp , naipe , valor);
				counter++;
			}

		if (counter != 2) tmp = (MAO) 0;
	}

	if (tmp != (MAO) 0){
		e.played[e.jogador] = add_cartas(e.played[e.jogador], tmp);
		e.mao[e.jogador] = rem_cartas(e.mao[e.jogador], e.played[e.jogador]);
	}
	else e.mao[e.jogador] = backup, e.played[e.jogador] = (MAO) 0;

	return e;
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
@param ESTADO Estado do jogo em que vão ser feitas as jogadas.
@param NAIPE Naipe mínimo que pode ser jogado dentro do valor.
@param VALOR Valor mínimo a ser jogado.
@return ESTADO Estado pós as eventuais alterações.
*/
ESTADO jogaFlush (ESTADO e, int naipe, int valor){ /* 5 cartas do mesmo naipe */

	int i = 0; int valortmp; int verifica = 0;

	if(carta_existe(e.mao[e.jogador], 0, 0))/*Para o caso de estar a abrir o jogo*/
		for ( ; naipe < 4 && e.played[e.jogador] == 0 ; naipe++){ 
			for (i = 0, valor = 0; valor < 13 && i < 5 ; valor++)
				if (carta_existe(e.mao[e.jogador] , naipe , valor)){

					e.played[e.jogador]= add_carta(e.played[e.jogador], naipe, valor); 
					i++;
				}
		
			if (i < 5) e.played[e.jogador] = 0;
			else e.mao[e.jogador] = rem_cartas(e.mao[e.jogador] , e.played[e.jogador]);
		}	  	


	else{

		for (valortmp = 12 ; valortmp >= 0 && i < 5 ; valortmp--){	/* verifica o naipe q recebe */

			if (carta_existe (e.mao[e.jogador] , naipe , valortmp)){

				e.played[e.jogador]= add_carta(e.played[e.jogador], naipe, valortmp); 
				i++;
				if (valortmp >= valor) verifica = 1;
			}	
		}

		if (i == 5 && verifica) e.mao[e.jogador] = rem_cartas(e.mao[e.jogador] , e.played[e.jogador]);
		else e.played[e.jogador] = 0;

			

		for (naipe++ ; naipe < 4 && e.played[e.jogador] == 0 ; naipe++){  /* verifica os naipes seguintes */
			for (i = 0, valor = 0; valor < 13 && i < 5 ; valor++)
				if (carta_existe(e.mao[e.jogador] , naipe , valor)){

					e.played[e.jogador]= add_carta(e.played[e.jogador], naipe, valor); 
					i++;
				}
		
			if (i < 5) e.played[e.jogador] = 0;
			else e.mao[e.jogador] = rem_cartas(e.mao[e.jogador] , e.played[e.jogador]);
		}
	}

	return e;
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
ESTADO jogaStraight (ESTADO e, int naipe, int valor){

	while (naipe < 4 && e.played[e.jogador] == (MAO) 0){
		e.played[e.jogador] = addStraight(e.mao[e.jogador], naipe, valor);

		naipe++;
	}

	for (valor++ ; e.played[e.jogador] == (MAO) 0 && valor < 13 ; valor++)
		e.played[e.jogador] = addStraight(e.mao[e.jogador], 0, valor);

	if (e.played[e.jogador] != (MAO) 0) e.mao[e.jogador] = rem_cartas(e.mao[e.jogador], e.played[e.jogador]);

	return e;
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

/**	\brief Tenta fazer os bots abrir o jogo com combinações.
@param ESTADO Nada jogado.
@return ESTADO Joga uma combinação ou então nada.
*/

ESTADO abreJogo (ESTADO e){

	MAO backup = e.mao[e.jogador];

	e = jogaStraight(e, 0, 0);

	if (!carta_existe(e.played[e.jogador], 0, 0)) e.played[e.jogador] = (MAO) 0, e.mao[e.jogador] = backup, e = jogaFlush(e, 0, 0);

	if (!carta_existe(e.played[e.jogador], 0, 0)) e.played[e.jogador] = (MAO) 0, e.mao[e.jogador] = backup, e = jogaFullHouse(e, 0, 0);

	if (!carta_existe(e.played[e.jogador], 0, 0)) e.played[e.jogador] = (MAO) 0, e.mao[e.jogador] = backup, e = jogaFourofaKind(e, 0, 0);

	if (!carta_existe(e.played[e.jogador], 0, 0)) e.played[e.jogador] = (MAO) 0, e.mao[e.jogador] = backup, e = jogaStraightFlush(e, 0, 0);

	if (!carta_existe(e.played[e.jogador], 0, 0)) e.played[e.jogador] = (MAO) 0, e.mao[e.jogador] = backup;

	return e;
}

ESTADO autoplay (ESTADO e){

	MAO maoAnterior;

	int valor, naipe, proceder, flag = 1, count = 0;

	int minValue, minNaipe;

	e.played[e.jogador] = (MAO) 0;

	if (e.passar == 3){
		
		e.passar = 0;
		e.nCartas = 0;

		e.played[0] = (MAO) 0;
		e.played[1] = (MAO) 0;
		e.played[2] = (MAO) 0;
		e.played[3] = (MAO) 0;
	}

	if (e.nCartas == 0){

		if (carta_existe(e.mao[e.jogador], 0, 0)){ /*Para o caso de estar à abrir o jogo*/
			e = abreJogo(e);
			if (e.played[e.jogador] != (MAO) 0) e.nCartas = 5;
		}

		for (valor = 0 ; valor < 13 && e.played[e.jogador] == (MAO) 0; valor++)				
			for (naipe = 0 ; naipe < 4 && e.played[e.jogador] == (MAO) 0 ; naipe++)				
				if (carta_existe (e.mao[e.jogador] , naipe , valor)){

					e.played[e.jogador] = add_carta(e.played[e.jogador] , naipe , valor);
					e.mao[e.jogador] = rem_carta(e.mao[e.jogador] , naipe , valor);

					e.nCartas = 1;
				}
									
		flag = 0;
	}
	

	if (e.nCartas == 1 && flag){
		maoAnterior = e.played[previousPlayer(&e)];
		minValue = returnValuelld (maoAnterior);
		minNaipe = returnNaipelld (maoAnterior);

		proceder = 1;

		for (naipe = minNaipe, valor = minValue ; naipe < 4 && proceder ; naipe++)
					if (carta_existe (e.mao[e.jogador] , naipe , valor)){
					e.played[e.jogador] = add_carta(e.played[e.jogador] , naipe , valor);
					e.mao[e.jogador] = rem_carta(e.mao[e.jogador] , naipe , valor); 
					proceder = 0;
					flag = 0;
					}

		for (valor = ++minValue ; valor < 13 && proceder ; valor++)
			for (naipe = 0 ; naipe < 4 && proceder ; naipe++)
				if (carta_existe (e.mao[e.jogador] , naipe , valor)){
					e.played[e.jogador] = add_carta(e.played[e.jogador] , naipe , valor);
					e.mao[e.jogador] = rem_carta(e.mao[e.jogador] , naipe , valor); 
					proceder = 0;
					flag = 0;
				}
	}

	if (e.nCartas == 2 && flag){

		maoAnterior = e.played[previousPlayer(&e)];
		minValue = returnValuelld (maoAnterior);
		minNaipe = returnNaipelld (maoAnterior);

		for (valor = minValue + 1 ; valor < 13 && count < 2 ; valor++)
			for (naipe = count = 0 , e.played[e.jogador] = (MAO) 0 ; naipe < 4 && count < 2 ; naipe++)
				if (carta_existe (e.mao[e.jogador] , naipe , valor)){
					count++;
					e.played[e.jogador] = add_carta (e.played[e.jogador] , naipe , valor);
				}

		if (count == 2) e.mao[e.jogador] = rem_cartas(e.mao[e.jogador] , e.played[e.jogador]), flag = 0;

		else e.played[e.jogador] = (MAO) 0;


	}

	if (e.nCartas == 3 && flag){

		maoAnterior = e.played[previousPlayer(&e)];
		minValue = returnValuelld (maoAnterior);
		minNaipe = returnNaipelld (maoAnterior);

		for (valor = minValue + 1 ; valor < 13 && count < 3 ; valor++)
			for (naipe = count = 0 , e.played[e.jogador] = (MAO) 0 ; naipe < 4 && count < 3 ; naipe++)
				if (carta_existe (e.mao[e.jogador] , naipe , valor)){
					count++;
					e.played[e.jogador] = add_carta (e.played[e.jogador] , naipe , valor);
				}

		if (count == 3) e.mao[e.jogador] = rem_cartas(e.mao[e.jogador] , e.played[e.jogador]), flag = 0;

		else e.played[e.jogador] = (MAO) 0;
	}

	if (e.nCartas == 5 && flag){

		MAO	maoAnterior = e.played[previousPlayer(&e)];

		CARTA card;

		if (isStraight(maoAnterior) && isFlush(maoAnterior)){ /*StraightFlush*/

			card = straightValue(maoAnterior, card);

			e = jogaStraightFlush(e , card.naipe , card.valor);

			if (e.played[e.jogador] != (MAO) 0) flag = 0; 
		}

	    if (flag && isFourOfAKind(maoAnterior)){

			card = fourOfAKindValue(maoAnterior, card);

	    	e = jogaFourofaKind(e, card.naipe , card.valor);

	    	if (e.played[e.jogador] != (MAO) 0) flag = 0;

	    	else e = jogaStraightFlush(e, 0, 0);

	    	if (e.played[e.jogador] != (MAO) 0) flag = 0;
		}

		if (flag && isFullHouse(maoAnterior)){

			card = fullHouseValue(maoAnterior, card);

			e = jogaFullHouse(e, 0, card.valor);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;

			else e = jogaFourofaKind(e, 0, 0); 

			if (e.played[e.jogador] != (MAO) 0) flag = 0;

			else e = jogaStraightFlush(e, 0, 0);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;
		}

		if (flag && isFlush(maoAnterior) && !isStraight(maoAnterior)){

			card = flushValue(maoAnterior, card);

			e = jogaFlush(e, card.naipe, card.valor);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;

			else e = jogaFullHouse(e, 0, 0);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;

			else e = jogaFourofaKind(e, 0, 0);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;

			else e = jogaStraightFlush(e, 0, 0);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;
		}

		if (flag && isStraight(maoAnterior) && !isFlush(maoAnterior)){

			card = straightValue(maoAnterior, card);

			e = jogaStraight(e, card.naipe, card.valor);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;

			else e = jogaFlush(e, 0, 0);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;

			else e = jogaFullHouse(e, 0, 0);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;

			else e = jogaFourofaKind(e, 0, 0);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;

			else e = jogaStraightFlush(e, 0, 0);

			if (e.played[e.jogador] != (MAO) 0) flag = 0;

		}

	}

	if (flag) e.passar++, imprime_passar(e.jogador);

	else e.passar = 0;

	e.jogador++;

	return e;
	
}
