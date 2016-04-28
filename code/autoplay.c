#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
URL da CGI
*/
#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"
/**
URL da pasta com as cartas
*/
#define BARALHO		"http://127.0.0.1/cards"

/**
Ordem dos naipes
*/
#define NAIPES		"DCHS"
/**
Ordem das cartas
*/
#define VALORES		"3456789TJQKA2"

#define TAM_MAX_ESTADO      1024

#define FORMATO "%lld_%lld_%lld_%lld_%lld_%d_%lld_%lld_%lld_%lld_%d_%d_%d_%d"

typedef long long int MAO;

typedef struct carta {
	int naipe, valor;
} CARTA;

struct state{
  MAO mao[4]; /*Maos de cada jogador*/
  MAO selecao; /*Cartas selecionadas*/
  MAO played[4]; /*Cartas jogadas*/
  int action, /*valor de jogada do joador proprio*/
   	  jogador, /*jogadr em mao*/
   	  nCartas, /*Numero de cartas em jogo por mão*/
	  passar, /*Número de vezes consecutivas que o*/
	  sort; /*Identifca a ordem das minhas cartas*/
};

typedef struct state ESTADO;

long long int add_carta(long long int est, int naipe, int valor);

long long int rem_carta(long long int est, int naipe, int valor);

int carta_existe(long long int est, int naipe, int valor);

int previousPlayer (ESTADO *e);

MAO rem_cartas(MAO est , MAO rem);

void imprime_passar (int n);

int maoRead (char cards[4][56] , MAO mao);

int numCartas (MAO sel);

int verificaSeguidos (int n1 , int n2);

void QSort (int v[], int N);

int returnValuelld (MAO mao){

	int valor, naipe;

	for (naipe = 0 ; naipe < 4 ; naipe++)
		for (valor = 0 ; valor < 13 ; valor++)
			if (carta_existe(mao , naipe , valor)) return valor;

	return -1;
}


int returnNaipelld (MAO mao){/**/

	int valor, naipe;

	for (naipe = 0 ; naipe < 4 ; naipe++)
		for (valor = 0 ; valor < 13 ; valor++)
			if (carta_existe(mao , naipe , valor)) return naipe;

	return -1;
}

/** \brief Recebe um valor de uma carta e devolve o valor anterior (no caso de ser um 3 devolve o valor de 2).
@param VALOR
	
*/
int valorAnterior (int * valor){

	if (*valor == 0) *valor = 12;

	else (*valor)--;

	return *valor;
}

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

CARTA StraightFlushValue (MAO mao, CARTA card){

	int naipe = 3, valor = 12;

	while (naipe > 0 && !carta_existe (mao , naipe , valor)){
		for (valor = 12 ; valor > 0 && !carta_existe (mao , naipe , valor) ; valor--);
		if (!carta_existe (mao , naipe , valor)) naipe--;
	}

	if (valor == 12) valor = 0, naipe++;

	else valor++;

	card.valor = valor;
	card.naipe = naipe;

	return card;
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

	if (numc >=5){
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

CARTA FourOfAKindValue (MAO mao, CARTA card){

	int valor, naipe, counter = 0;

	for (valor = 12 ; valor >= 0 && counter < 4 ; valor--)
		for (naipe = 3, counter = 0 ; naipe >= 0 && carta_existe(mao, naipe, valor) ; naipe--, counter++);

	card.valor = valor + 2;
	card.naipe = 0;

	return card;
}

MAO add3_FullHouse (MAO mao, int naipe, int valor){

	mao = add_carta (mao , naipe , valor);
	mao = add_carta (mao , naipeAnterior (&naipe) , valor);
	mao = add_carta (mao , naipeAnterior (&naipe) , valor);
 
	return mao;

}

MAO add2_FullHouse (MAO mao, int naipe, int valor){

	mao = add_carta (mao , naipe , valor);
	mao = add_carta (mao , naipeAnterior (&naipe) , valor);
 
	return mao;

}


ESTADO jogaFullHouse(ESTADO e, int naipe, int valor){

	int valortmp;

	for (valortmp = valor ; valortmp < 13 && e.played[e.jogador] == (MAO) 0 ; valortmp++)
		 for (naipe = 0 ; naipe < 4 && e.played[e.jogador] == (MAO) 0 && carta_existe (e.mao[e.jogador] , naipe , valortmp) ; naipe++) 
		 	if (naipe==3){
		 		e.played[e.jogador] = add3_FullHouse (e.played[e.jogador] , naipe , valortmp);
		 		e.mao[e.jogador] = rem_cartas (e.mao[e.jogador] , e.played[e.jogador]);
		}

	for ( ; valor < 13 && e.played[e.jogador] == (MAO) 0 ; valor++)
		 for (naipe = 0 ; naipe < 4 && e.played[e.jogador] == (MAO) 0 && carta_existe (e.mao[e.jogador] , naipe , valor) ; naipe++) 
			if (naipe==2){
				e.played[e.jogador] = add2_FullHouse (e.played[e.jogador] , naipe , valor);
		 		e.mao[e.jogador] = rem_cartas (e.mao[e.jogador] , e.played[e.jogador]);	
			}

	return e;

}

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

int isFlush (MAO mao){

	int naipes[5], i = 0, valor, naipe;

	for (naipe = 0 ; naipe < 4 ; naipe++)
		for (valor = 0 ; valor < 13 ; valor++)
			if (carta_existe(mao , naipe, valor)) naipes[i] = naipe, i++;

	for (i = 0 ; i + 1 < 5 && (naipes[i] == naipes[i + 1]) ; i++);

	return (i == 4);

}


int isFourOfAKind (MAO mao){

	int valor, naipe, counter = 0;

	for (valor = 0 ; valor < 13 && counter != 4 ; valor++)
		for (naipe = counter = 0 ; naipe < 4 && carta_existe (mao, naipe, valor) ; naipe++, counter++);

	return (counter == 4);

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

	if (e.nCartas == 0){ 	for (proceder = 1, valor = 0 ; valor < 13 && proceder ; valor++)
								for (naipe = 0 ; naipe < 4 && proceder ; naipe++)
									if (carta_existe (e.mao[e.jogador] , naipe , valor)){

										e.played[e.jogador] = add_carta (e.played[e.jogador] , naipe , valor);
										e.mao[e.jogador] = rem_carta (e.mao[e.jogador] , naipe , valor);

										e.nCartas = 1;
										proceder = 0;
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

			card = StraightFlushValue(maoAnterior, card);

			e = jogaStraightFlush(e , card.naipe , card.valor);

			if (e.played[e.jogador] != (MAO) 0) flag = 0; 
		}

	    if (flag && isFourOfAKind(maoAnterior)){

			card = FourOfAKindValue(maoAnterior, card);

	    	e = jogaFourofaKind(e, card.naipe , card.valor);

	    	if (e.played[e.jogador] != (MAO) 0) flag = 0;

	    	else e = jogaStraightFlush(e, 0, 0);

	    	if (e.played[e.jogador] != (MAO) 0) flag = 0;
		}

		if (flag && isFullHouse (maoAnterior)){


			if (e.played[e.jogador] != (MAO) 0) flag = 0; 
		}

		if (flag && isFlush (maoAnterior)){


			if (e.played[e.jogador] != (MAO) 0) flag = 0; 
		}

		if (flag && isStraight (maoAnterior)){



		}

	}

	if (flag) e.passar++, imprime_passar(e.jogador);

	else e.passar = 0;

	e.jogador++;

	return e;
	
}
