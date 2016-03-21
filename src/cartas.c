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

#define FORMATO "%lld_%lld_%lld_%lld_%d_%d_%d_%d_%lld_%d_%d_%d_%lld_%lld_%lld_%lld"

typedef long long int MAO;

struct state{
  MAO mao[4];
  MAO selecao;
  MAO played[4];
  int cartas[4];
  int action , passar, selecionar;
};

typedef struct state ESTADO;

ESTADO shuffle (ESTADO e);

char* estado2str (ESTADO e);

ESTADO str2estado (char* str);

ESTADO inicializa (ESTADO e);

/**
	Estado inicial com todas as 52 cartas do baralho
	Cada carta é representada por um bit que está
	a 1 caso ela pertença à mão ou 0 caso contrário
*/


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

MAO rem_cartas(MAO est , MAO rem){

	rem = ~rem;

	est = est & rem;

	return est;
}

/** \brief Conta o número de cartas presentes numa mão

*/

int bitsOne (MAO sel){


	int nCartas, count;

	for (nCartas = count = 0; count < 56 ; sel = sel >> 1 , count++)
			if (sel & ((MAO) 1)) nCartas++;

	return nCartas;

}

/** Verifica se uma determinada jogada é valida*/ /*AINDA É PROVISORIA, APENAS VERIFICA SE FOR SÓ UMA CARTA*/

int verificaJogada (MAO sel){

	int nCartas, flag = 0;

	nCartas = bitsOne (sel);

	if (nCartas == 1) flag = 1;

	if (nCartas == 2) flag = 1;

	return flag;
}


/** \brief Verifica se uma carta pertence ao estado

@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		1 se a carta existe e 0 caso contrário
*/
int carta_existe(long long int est, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return (est >> idx) & 1;
}

/** \brief Imprime o html correspondente a uma carta

@param path	o URL correspondente à pasta que contém todas as cartas
@param x A coordenada x da carta
@param y A coordenada y da carta
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
*/
void imprime_carta(int x, int y, ESTADO e , int m , int naipe, int valor){
	char *suit = NAIPES;
	char *rank = VALORES;
	char script[10240];

	if (carta_existe (e.selecao , naipe , valor)) y -= 30;
	
	/* e.mao[m] = rem_carta(e.mao[m], naipe , valor); */

	if (m == 3){
		if (carta_existe (e.selecao , naipe , valor)) e.selecao = rem_carta (e.selecao , naipe , valor);
		else e.selecao = add_carta(e.selecao , naipe , valor);
	}

	sprintf(script, "%s?%s" , SCRIPT, estado2str(e));
	/*sprintf(script, "%s?q=%lld", SCRIPT, rem_carta(ESTADO, naipe, valor));*/
	printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, BARALHO, rank[valor], suit[naipe]);
}

void imprime_mao (int x , int y , ESTADO e , MAO mao , int m){

	int n, v;

	if (m >= 3)	for(v = 0 ; v < 13 ; v++)
					for(n = 0; n < 4; n++){
						if(carta_existe(mao , n , v)){
							x += 40;
							imprime_carta(x , y , e , m , n , v);
						}
					}
	else for (v = 0 ; v < 13 ; v++)
			for (n = 0 ; n < 4 ; n++){
				if(carta_existe(mao , n , v)){
					y += 20;
					imprime_carta (x , y , e , m , n , v);
				}

			}
}

void imprime_botoes (int x , int y, ESTADO e){

	/*sprintf(script, "%s?q=%lld", SCRIPT, rem_carta(ESTADO, naipe, valor));*/

	char script[10240];


	e.action = 2;

		if (verificaJogada (e.selecao)){

			sprintf(script, "%s?%s" , SCRIPT, estado2str(e));
			printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%s\" /></a>\n", script, x, y, BARALHO, "play.png");
		}

		else printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%s\" />\n", x, y, BARALHO, "play.png");

	y += 70;

	e.action = 3;
		sprintf(script, "%s?%s" , SCRIPT, estado2str(e));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%s\" /></a>\n", script, x, y, BARALHO, "pass.png");
	
	y += 70;

	e.action = 1;
		sprintf(script, "%s?%s" , SCRIPT, estado2str(e));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%s\" /></a>\n", script, x, y, BARALHO, "shuffle.png");
	
	e.action = 0;
}

/** \brief Imprime o estado

Esta função está a imprimir o estado em quatro colunas: uma para cada naipe
@param path	o URL correspondente à pasta que contém todas as cartas
@param ESTADO	O estado atual
*/
void imprime(ESTADO e){

	printf("<svg height = \"800\" width = \"800\">\n");
	printf("<rect x = \"0\" y = \"0\" height = \"800\" width = \"800\" style = \"fill:#007700\"/>\n");

	if (e.action == 1){ /*BARALHAR*/
		e = inicializa (e);
		e = shuffle (e);
	}
	
	if (e.action == 2){ /*JOGAR*/
		e.played[3] = e.selecao;
		e.mao[3] = rem_cartas (e.mao[3] , e.selecao);
		e.selecao = 0;
	}


	imprime_mao (150 , 010 , e , e.mao[0] , 0);
	imprime_mao (350 , 010 , e , e.mao[1] , 1);
	imprime_mao (550 , 010 , e , e.mao[2] , 2);
	imprime_mao (070 , 650 , e , e.mao[3] , 3);

	/*Imprime as cartas que estão em jogo*/

	imprime_mao (400 , 400 , e , e.played[0] , 4);
	imprime_mao (400 , 400 , e , e.played[1] , 5);
	imprime_mao (400 , 400 , e , e.played[2] , 6);
	imprime_mao (300 , 520 , e , e.played[3] , 7);


	imprime_botoes (700 , 550 , e);

	printf("</svg>\n");
}



ESTADO shuffle (ESTADO e){

	int naipe, valor, randN;

	srandom (time (NULL));

	for (naipe = 3 ; naipe >= 0 ; naipe--){

		for (valor = 12 ; valor >= 0 ; valor--){

			randN = random()%4;

			while (e.cartas[randN] == 13)
				randN = random() % 4; 

			e.cartas[randN]++;

			e.mao[randN] = add_carta(e.mao[randN] , naipe , valor);

		}

	}

	return e;
}

ESTADO str2estado (char* str) {
	ESTADO e;
	sscanf(str, FORMATO, &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.cartas[0], &e.cartas[1], &e.cartas[2],&e.cartas[3], &e.selecao, &e.passar, &e.selecionar, &e.action , &e.played[0] , &e.played[1] , &e.played[2] , &e.played[3]);
	return e;
}

char* estado2str (ESTADO e){
	static char res[10240];
	sprintf(res, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.cartas[0],e.cartas[1],e.cartas[2],e.cartas[3], e.selecao, e.passar, e.selecionar, e.action , e.played[0] , e.played[1] , e.played[2] , e.played[3]);
	return res;
}


ESTADO inicializa (ESTADO e){

	int i;
    for (i = 0 ; i < 4 ; i++){
    	e.mao[i]=0;
    	e.cartas[i]=0;
    	e.played[i]=0;
    }
    e.selecao = 0; 
    e.action = 0;
    e.passar = e.selecionar = 0;

    return e;
}

/** \brief Trata os argumentos da CGI

Esta função recebe a query que é passada à cgi-bin e trata-a.
Neste momento, a query contém o estado que é um inteiro que representa um conjunto de cartas.
Cada carta corresponde a um bit que está a 1 se essa carta está no conjunto e a 0 caso contrário.
Caso não seja passado nada à cgi-bin, ela assume que todas as cartas estão presentes.
@param query A query que é passada à cgi-bin
 */
void parse(char *query , ESTADO e){

	if (strlen (query) == 0)
		e = shuffle (e);

	else e = str2estado (query); 

	imprime(e);
}

/** \brief Função principal

Função principal do programa que imprime os cabeçalhos necessários e depois disso invoca
a função que vai imprimir o código html para desenhar as cartas
 */
int main() {

	ESTADO est;
	
	/** Cabeçalhos necessários numa CGI
 */
	printf("Content-Type: text/html; charset=utf-8\n\n");
	printf("<header><title>Big2</title></header>\n");
	printf("<body>\n");

	printf("<h1>Big2</h1>\n");

	if (strlen (getenv("QUERY_STRING")) == 0) est = inicializa (est);	

/**
 Ler os valores passados à cgi que estão na variável ambiente e passá-los ao programa
 */
	parse(getenv("QUERY_STRING") , est);

	printf("</body>\n");

	return 0;
}
