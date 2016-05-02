#include "cartas.h"
#include "view.h"
#include "autoplay.h"
#include "verificaJogada.h"

/** \brief Função principal

Função principal do programa que imprime os cabeçalhos necessários e depois disso invoca
a função que vai imprimir o código html para desenhar as cartas
 */

void iniciaHTML (){

	/** Cabeçalhos necessários numa CGI
 */
	printf("Content-Type: text/html; charset=utf-8\n\n");
	printf("<header><title>Big2</title></header>\n");
	printf("<body>\n");

	printf("<h1>Big2</h1>\n");

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

	if (carta_existe (e.selecao , naipe , valor)) y -= 30; /*Sobe as cartas seleccionadas*/
	

	if (m == 3 && e.jogador == 3){ /*Adiciona e retira cartas do e.seleccao e imprime tudo*/
		if (carta_existe (e.selecao , naipe , valor)) e.selecao = rem_carta (e.selecao , naipe , valor);
		else e.selecao = add_carta(e.selecao , naipe , valor);

		sprintf(script, "%s?%s" , SCRIPT, estado2str(e));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, BARALHO, rank[valor], suit[naipe]);
	}
	/*imprime as cartas dos adversários sem link*/
	else printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" />\n", x, y, BARALHO, rank[valor], suit[naipe]);
}
/** \brief Imprime uma mão inteira de cartas.
@param COORX valor de abcissa do início da mão.
@param COORY valor de ordenada do início da mão.
@param ESTADO estado do jogo atual.
@param MAO composição da mão atual.
@param M identificação da mão
*/
void imprime_mao (int x , int y , ESTADO e , MAO mao , int m){

	int n, v;

	if (m == 3){
		if (e.sort == 0) for(v = 0 ; v < 13 ; v++) /*Para a mão própria*/
							for(n = 0; n < 4; n++){
								if(carta_existe(mao , n , v)){
									x += 40;
									imprime_carta(x , y , e , m , n , v);
								}
							}

		if (e.sort == 1) for(n = 0 ; n < 4 ; n++) /*Para a mão própria*/
							for(v = 0; v < 13; v++){
								if(carta_existe(mao , n , v)){
									x += 40;
									imprime_carta(x , y , e , m , n , v);
								}
							}

	}

	if (m >= 4) for(v = 0 ; v < 13 ; v++) /*Para as cartas que estão em jogo*/
					for(n = 0; n < 4; n++){
						if(carta_existe(mao , n , v)){
							x += 15;
							imprime_carta(x , y , e , m , n , v);
						}
					}

	if (m <= 2) for (v = 0 ; v < 13 ; v++) /*Para as mãos dos adversários*/
					for (n = 0 ; n < 4 ; n++){
						if(carta_existe(mao , n , v)){
							y += 14;
							x -= 5;
							imprime_carta (x , y , e , m , n , v);
						}

					}
}

/** \brief Imprime os botões do jogo.
@param COORX valor de abcissa do inicio dos botões.
@param COORY valor de abcissa do inicio dos botões.
@param ESTADO script do estado atual a ser imprimido nos botoes.
*/

void imprime_botoes (int x , int y, ESTADO e){

	/*sprintf(script, "%s?q=%lld", SCRIPT, rem_carta(ESTADO, naipe, valor));*/

	char script[10240];


	e.action = 2;

		if (verificaJogada (e)){

			sprintf(script, "%s?%s" , SCRIPT, estado2str(e));
			printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%s\" /></a>\n", script, x, y, BARALHO, "play.png");
		}

		else printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%s\" />\n", x, y, BARALHO, "play.png");

	y += 100;

	e.action = 3;
		if (e.nCartas != 0){
		sprintf(script, "%s?%s" , SCRIPT, estado2str(e));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%s\" /></a>\n", script, x, y, BARALHO, "pass.png");
		}
		else printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%s\" />\n", x, y, BARALHO, "pass.png");

	y += 100;

	e.action = 1;
		sprintf(script, "%s?%s" , SCRIPT, estado2str(e));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%s\" /></a>\n", script, x, y, BARALHO, "shuffle.png");

	e.action = 0;
	y += 100;

	if (e.sort == 1) e.sort = 0;
	else e.sort++;
		sprintf(script, "%s?%s" , SCRIPT, estado2str(e));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%s\" /></a>\n", script, x, y, BARALHO, "sort.png");


}


void imprime_passar (int n){

	if (n == 0)	printf("<text x=\"100\" y=\"400\" fill=\"black\" font-size=\"45\">Passou!</text>");

	if (n == 1)	printf("<text x=\"300\" y=\"400\" fill=\"black\" font-size=\"45\">Passou!</text>");

	if (n == 2)	printf("<text x=\"500\" y=\"400\" fill=\"black\" font-size=\"45\">Passou!</text>");

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
		e.nCartas = numCartas (e.selecao);
		e.selecao = 0;
		e.jogador = 0;
		e.passar = 0;
	}

	if (e.action == 3){/*PASSAR*/
	e.passar++;
	e.jogador = 0;
	}

	e.action = 0;

	while (e.jogador != 3) e = autoplay (e);

	if (e.passar == 3){
		
		e.passar = 0;
		e.nCartas = 0;

		e.played[0] = (MAO) 0;
		e.played[1] = (MAO) 0;
		e.played[2] = (MAO) 0;
		e.played[3] = (MAO) 0;
	}

	e.played[3] = (MAO) 0;

	imprime_mao (190 , 010 , e , e.mao[0] , 0);
	imprime_mao (390 , 010 , e , e.mao[1] , 1);
	imprime_mao (590 , 010 , e , e.mao[2] , 2);
	imprime_mao (50 , 650 , e , e.mao[3] , 3);

	/*Imprime as cartas que estão em jogo*/

	imprime_mao (100 , 350 , e , e.played[0] , 4);
	imprime_mao (300 , 350 , e , e.played[1] , 5);
	imprime_mao (500 , 350 , e , e.played[2] , 6);
	imprime_mao (300 , 530 , e , e.played[3] , 7);


	imprime_botoes (700 , 350 , e);

	printf("</svg>\n");
}


void finalizaHTML (){

	printf("</body>\n");
}