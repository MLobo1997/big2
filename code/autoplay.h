#ifndef AUTOPLAY_H_ 
#define AUTOPLAY_H_

typedef struct carta{

	int valor, naipe;

}CARTA;

int returnValuelld (MAO mao);

int returnNaipelld (MAO mao);

int valorAnterior (int * valor);

int naipeAnterior (int * naipe);

int hasStraightFlush (MAO mao, int naipe, int valor);

MAO add_StraightFlush (MAO mao, int naipe, int valor);

ESTADO jogaStraightFlush (ESTADO e , int naipe, int valor);

CARTA StraightFlushValue (MAO mao, CARTA card);

MAO add_FourofaKind (MAO mao, int valor);

MAO add_valormaisbaixo (MAO mao,MAO maoplayed);

ESTADO jogaFourofaKind (ESTADO e, int naipe, int valor);

CARTA fourOfAKindValue (MAO mao, CARTA card);

ESTADO jogaFullHouse(ESTADO e, int naipe, int valor);

int hasFlush_naipeinicial (ESTADO e, int naipe,int valor);

ESTADO jogaFlush (ESTADO e, int naipe, int valor);

int isStraight (MAO mao);

int isFlush (MAO mao);

CARTA flushValue (MAO mao, CARTA card);

int isFourOfAKind (MAO mao);

int isFullHouse (MAO mao);

CARTA fullHouseValue(MAO mao, CARTA card);

MAO addStraight (MAO mao, int naipe, int valor);

ESTADO jogaStraight (ESTADO e, int naipe, int valor);

ESTADO autoplay (ESTADO e);


#endif