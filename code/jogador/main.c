#include "jogador_inteligente.h"
#include "read.h"

int main (){

	int n;

	JOGO e = (JOGO) malloc(sizeof(struct jogo));

	char action[7]; /*Leitura de todas as ações que ocorreram no jogo e.g.: "PASSOU", "ACABOU"*/
	char cards[13][3]; /*Mao total, no maximo 13 cartas, identificadas por uma string de dois caracteres mais o '\0'*/

	initiate(e);

	while (1){

		while ((n = scanf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s", action, cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6], cards[7], cards[8], cards[9], cards[10], cards[11], cards[12])) == 0);
		/*A variável n identifica o número de palavras que foram lidas o scanf é executado até ler alguma coisa*/

		if (n == 1) readAction(e, action); /*se só tem um comando*/

		if (n > 1 && n <= 6) readMove(e, action, cards, n); /*Lê qualquer jogada entre 1 e 5 cartas*/

		if (n == 14) readHand(e, action, cards, n); /*Lê a mão que me foi atribuida*/

	}

	return 0;
}