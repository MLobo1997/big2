#include "jogador_inteligente.h"
#include "read.h"
#include "jogar.h"
#include "output.h"

int main (){

	int n = 1;

	JOGO e = (JOGO) malloc(sizeof(struct jogo));

	char action[10]; /*Leitura de todas as ações que ocorreram no jogo e.g.: "PASSOU", "ACABOU"*/
	char cards[13][3]; /*Mao total, no maximo 13 cartas, identificadas por uma string de dois caracteres mais o '\0'*/

	initiate(e);

	while (n != 0){

		n = 0;

		n = readInput(action, cards);
		/*A variável n identifica o número de palavras que foram lidas o scanf é executado até ler alguma coisa*/
		n = n;

		if (n == 1) readAction(e, action); /*se só tem um comando*/

		if (n > 1 && n <= 6) readMove(e, action, cards, n); /*Lê qualquer jogada entre 1 e 5 cartas*/

		if (n == 14) readHand(e, action, cards, n); /*Lê a mão que me foi atribuida*/

	}

	return 0;
}