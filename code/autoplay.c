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

MAO add_FourofaKind (MAO mao, int naipe,int valor){
	mao = add_carta (mao , naipe , valor);
	mao = add_carta (mao , naipeAnterior (&naipe) , valor);
	mao = add_carta (mao , naipeAnterior (&naipe) , valor);
	mao = add_carta (mao , naipeAnterior (&naipe) , valor);
	

	return mao;


}

MAO add_valormaisbaixo (MAO mao,MAO maoplayed){

	int valor,naipe;

		 for (valor=0; valor < 13 ; valor++)
		 	for (naipe = 0 ; naipe < 4 && !(carta_existe (mao,naipe,valor)) ; naipe++); 

	maoplayed = add_carta (maoplayed,naipe,valor);



return maoplayed;


}


ESTADO jogaFourofaKind (ESTADO e, int naipe, int valor){

	int numc = numCartas (e.mao[e.jogador]); /* redundante?*/

	if (numc >=5){
		 for ( ; valor < 13 && e.played[e.jogador] == (MAO) 0 ; valor++)
		 	for (naipe = 0 ; naipe < 4 && e.played[e.jogador] == (MAO) 0 && carta_existe (e.mao[e.jogador] , naipe , valor) ; naipe++) 
		 		if (naipe==4){
		 			e.played[e.jogador] = add_FourofaKind (e.played[e.jogador] , naipe , valor);
		 			e.mao[e.jogador] = rem_carta (e.mao[e.jogador] , naipe , valor);

					e.played[e.jogador] = add_valormaisbaixo (e.mao[e.jogador], e.played[e.jogador]);
					e.mao[e.jogador] = rem_cartas (e.mao[e.jogador] , e.played[e.jogador]);

				}
	}

	return e;

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

int isStraightFLush (MAO mao){

	int naipe = 0, valor, n;

	while (naipe < 4 && !carta_existe(mao , naipe , valor)){
		for (valor = 0 ; valor < 13 && !carta_existe(mao , naipe , valor) ; valor++);
		if (!carta_existe(mao , naipe , valor)) naipe++; 
	}

	for (n = 1, valor++ ; n <= 5 && carta_existe(mao , naipe , valor) ; valor++, n++);

	return (n > 5);
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

		if (isStraightFlush (maoAnterior)){

			naipe = 3;
			valor = 12;

			while (naipe > 0 && !carta_existe (maoAnterior , naipe , valor)){
				for (valor = 12 ; valor > 0 && !carta_existe (maoAnterior , naipe , valor) ; valor--);
				if (!carta_existe (maoAnterior , naipe , valor)) naipe--;
			}

			e = jogaStraightFlush(e , naipe , valor);
		}

		if (e.played[e.jogador] != (MAO) 0) flag = 0; 

	}

	if (flag) e.passar++, imprime_passar(e.jogador);

	else e.passar = 0;

	e.jogador++;

	return e;
	
}
