#define VALORES		"3456789TJQKA2"

#define NAIPES		"DCHS"


int readAction (JOGO e, char *action);

int maoWrite (char cards[52][3] , MAO mao);

void valueToStr (char str[], int valor , int naipe);

MAO maoRead (char cards[13][3], int N);

int readMove (JOGO e, char *action, char cards[13][3], int N);

int readHand (JOGO e, char *action, char cards[13][3], int N);
