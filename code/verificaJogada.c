#include "cartas.h"
#include "view.h"
#include "autoplay.h"
#include "verificaJogada.h"

void valueToStr (char str[], int valor , int naipe){

	char *suit = NAIPES;
	char *rank = VALORES;

	sprintf (str , "%c_%c" , rank[valor] , suit[naipe]);
}



/**
Devolver o valor de 0 a 12 de uma carta
*/

int returnValue (char carta[4]){

	int i;
	char *rank = VALORES;

	for (i = 0; carta[0] != rank[i] ; i++);

	return i;
}

/**
Devolver o naipe de 0 a 3 de uma carta
*/

int returnNaipe (char carta[4]){

	int i;
	
	char *suit = NAIPES;

	for (i = 0; carta[2] != suit[i] ; i++);

	return i;

}

/** \brief Coloca num array strings com o formato "(valor)_(naipe)" e devolve o número de strings colocadas
@param MATRIZ poderá ter até 56 cartas (strings de 3 elementos).
@param MAO O long long int que será lido.
@return NUM número de cartas.
*/

int maoRead (char cards[4][56] , MAO mao){

	int valor, naipe, n;

	char tmp[4];

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

int numCartas (MAO sel){


	int nCartas, count;

	for (nCartas = count = 0; count < 56 ; sel = sel >> 1 , count++)
			if (sel & ((MAO) 1)) nCartas++;

	return nCartas;
}

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
*/
int verificaSeguidos (int n1 , int n2){

	int flag = 0;

	if (n1 + 1 == n2 || n1 == n2 + 1) flag = 1;

	if ((n1 == 12 && n2 == 0) || (n1 == 0 && n2 == 12)) flag = 1; /*Para o caso de uma ser um 3 e outra 2*/

	return flag;
}

int sequencia (char cartas[4][56]){

	int valores[5], tmp[5];
	int i, n, ntmp, flag = 0;

	for (i = 0 ; i < 5 ; i++)
		valores[i] = returnValue(cartas[i]);

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

int flush (char cartas[4][56]){

	int naipes[5], i;

	for (i = 0 ; i < 5 ; i++)
		naipes[i] = returnNaipe (cartas[i]);

	for (i = 0 ; i + 1 < 5 && (naipes[i] == naipes[i + 1]) ; i++);

	return (i == 4);
}

int fullAndfour (char cartas[4][56]){ 

	int ranks[5], i;

	for (i = 0 ; i < 5 ; i++)
		ranks[i] = returnValue (cartas[i]);

	QSort (ranks , 5);

	return 	((ranks[0] == ranks[1] && ranks[1] == ranks[2] && ranks [2] == ranks[3]) ||
			(ranks[1] == ranks[2] && ranks[2] == ranks[3] && ranks[3] == ranks[4]) ||
			(ranks[0] == ranks[1] && ranks[2] == ranks[3] && ranks[3] == ranks[4]) || 
			(ranks[0] == ranks[1] && ranks[1] == ranks[2] && ranks[3] == ranks[4]));
}

int previousPlayer (ESTADO *e){

	int n = e->jogador; 

	if (n == 0) n = 3;

	else n--;

	n -= e->passar;

	if (n < 0) n += 4;

	return n;
}

/** \brief verifica se uma carta é maior que outra
@param MAIOR carta maior
@param MENOR carta menor
*/
int cartaMaior (char carta1[4] , char carta2[4]){

	int flag = 0, value1, value2, naipe1, naipe2;

	value1 = returnValue (carta1);
	value2 = returnValue (carta2);
	naipe1 = returnNaipe (carta1);
	naipe2 = returnNaipe (carta2);

	if (value1 > value2) flag = 1;

	else if (value1 == value2 && naipe1 > naipe2) flag = 1;

	return flag; 
}

int verificaDuas (char cartas[4][56] , char cartasAnteriores[4][56]){

	char maior[4], maiorAnterior[4];

	int flag = 0;

	if (returnValue(cartas[0]) == returnValue(cartas[1])){

		if (cartaMaior (cartas[0] , cartas[1])) strcpy (maior , cartas[0]);

		else strcpy (maior , cartas[1]);

		if (cartaMaior (cartasAnteriores[0] , cartasAnteriores[1])) strcpy (maiorAnterior , cartasAnteriores[0]);

		else strcpy (maiorAnterior , cartasAnteriores[1]);

		if (cartaMaior (maior , maiorAnterior)) flag = 1;

	}

	return flag;
}

int verificaTres (char cartas[4][56] , char cartasAnteriores[4][56]){ 
	char maior[4], maiorAnterior[4];

	int flag = 0;

	if (returnValue(cartas[0]) == returnValue(cartas[1]) && returnValue(cartas[1]) == returnValue(cartas[2])){

		if (cartaMaior (cartas[0] , cartas[1])) strcpy (maior , cartas[0]);
		
		else strcpy (maior , cartas[1]);

		if (!cartaMaior (maior , cartas[2])) strcpy (maior , cartas[2]);

		if (cartaMaior (cartasAnteriores[0] , cartasAnteriores[1])) strcpy (maiorAnterior , cartasAnteriores[0]);

		else strcpy (maiorAnterior , cartasAnteriores[1]);

		if (!cartaMaior (maiorAnterior , cartasAnteriores[2])) strcpy (maiorAnterior , cartasAnteriores[2]);

		if (cartaMaior (maior , maiorAnterior)) flag = 1;

	}

	return flag;
}

/** \brief Verifica se um straight flush é superior ao outro.
@param MAO E.seleçao, terá de ser superior.
@param MAO Mao jogada anteriormente, terá de ser inferior.
@return BOOL 1 caso as condiçoes se confirmem, 0 caso contrário.
*/

int comparaStraightFlush (MAO mao, MAO maoAnterior){

	CARTA maior, maiorAnterior;

	maiorAnterior = flushOrStraightValue (maoAnterior, maiorAnterior);

	maior = flushOrStraightValue(mao, maior);

	return (maior.valor > maiorAnterior.valor ||
		   (maior.valor == maiorAnterior.valor &&
		    maior.naipe > maiorAnterior.naipe));
}

/** \brief Verifica se um Four of a Kind é superior ao outro.
@param MAO E.seleçao, terá de ser superior.
@param MAO Mao jogada anteriormente, terá de ser inferior.
@return BOOL 1 caso as condiçoes se confirmem, 0 caso contrário.
*/

int comparaFourOfAKind (MAO mao, MAO maoAnterior){

	CARTA valor, valorAnterior;

	valor = fourOfAKindValue(mao, valor);

	valorAnterior = fourOfAKindValue(maoAnterior, valorAnterior);

	return (valor.valor > valorAnterior.valor);

}

int verificaCinco (MAO mao , MAO maoAnterior){

	int flag = 0;

	if (isStraight(maoAnterior) && isFlush(maoAnterior)) /*Se a jogada anterior tiver sido um straight flush*/
		if ((isStraight(mao) && isFlush(mao)) && comparaStraightFlush(mao, maoAnterior)) flag = 1;

	if (!flag && isFourOfAKind(maoAnterior))
		if ((isStraight(mao) && isFlush(mao)) || (isFourOfAKind(mao) && comparaFourOfAKind(mao, maoAnterior))) flag = 1;

	return flag;
}


/** \brief Verifica se uma determinada jogada é valida
@param ESTADO recebe o estado atual
*/ 

int verificaJogada (ESTADO e){

	int nCartas, flag = 0 , prosseguir = 1;

	char cartas[4][56], cartasAnteriores[4][56];

	nCartas = maoRead(cartas , e.selecao);

	maoRead(cartasAnteriores , e.played[previousPlayer (&e)]);

	if (e.nCartas == 0){ /*Abrir jogada*/

		if (nCartas == 1){
			if (carta_existe (e.mao[3] , 0 , 0)){
				if (carta_existe (e.selecao , 0 , 0)) flag = 1, e.nCartas = 1; 
			}
			else flag = 1;
		}

		if (nCartas == 2) if (returnValue (cartas[0]) == returnValue (cartas[1])){
			if (carta_existe (e.mao[3] , 0 , 0)){
				if (carta_existe (e.selecao , 0 , 0)) flag = 1, e.nCartas = 2;
			}
			else flag = 1;
		}

		if (nCartas == 3) if (returnValue (cartas[0]) == returnValue (cartas[1]) && returnValue (cartas[1]) == returnValue (cartas[2])){
			if (carta_existe (e.mao[3] , 0 , 0)){
				if (carta_existe (e.selecao , 0 , 0)) flag = 1, e.nCartas = 3;
			}
			else flag = 1;
		}

		if (nCartas == 5) if (sequencia (cartas) || flush (cartas) || fullAndfour (cartas)){
			if (carta_existe (e.mao[3] , 0 , 0)){
				if (carta_existe (e.selecao , 0 , 0)) flag = 1, e.nCartas = 5;
			}
			else flag = 1;
		}
		prosseguir = 0;
	}

	if (e.nCartas == 1 && prosseguir)

		if (nCartas == 1 && cartaMaior (cartas[0] , cartasAnteriores[0])) flag = 1;


	if (e.nCartas == 2 && prosseguir)

		if (nCartas == 2 && verificaDuas (cartas , cartasAnteriores)) flag = 1;


	if (e.nCartas == 3 && prosseguir)

		if (nCartas == 3 && verificaTres (cartas , cartasAnteriores)) flag = 1;

/*	if (e.nCartas == 5 && prosseguir)

		if (nCartas == 5 && verificaCinco (cartas ,  cartasAnteriores)) flag = 1;
*/

	return flag; 
}
