#include "cartas.h"
#include "view.h"
#include "autoplay.h"
#include "verificaJogada.h"

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
int main(){	

	ESTADO est;

	/** Cabeçalhos necessários numa CGI
 */
	iniciaHTML();

	if (strlen (getenv("QUERY_STRING")) == 0) est = inicializa (est);	

/**
 Ler os valores passados à cgi que estão na variável ambiente e passá-los ao programa
 */
	parse(getenv("QUERY_STRING") , est);

	finalizaHTML();

	return 0;
}