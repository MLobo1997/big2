typedef struct carta{

	int valor, naipe;

}CARTA;

int naipeAnterior (int * naipe);

int numCartas (MAO sel);

int valorAnterior (int * valor);

int hasStraightFlush (MAO mao, int naipe, int valor);

MAO add_StraightFlush (MAO mao, int naipe, int valor);

MAO jogaStraightFlush (JOGO e , int naipe, int valor);

CARTA StraightFlushValue (MAO mao, CARTA card);

CARTA straightValue (MAO mao, CARTA card);

MAO add_FourofaKind (MAO mao, int valor);

MAO add_valormaisbaixo (MAO mao,MAO maoplayed);

MAO jogaFourofaKind (JOGO e, int naipe, int valor);

CARTA fourOfAKindValue (MAO mao, CARTA card);

MAO jogaFullHouse(JOGO e, int naipe, int valor);

int hasFlush_naipeinicial (JOGO e, int naipe,int valor);

MAO jogaFlush (JOGO e, int naipe, int valor);

int isStraight (MAO mao);

int isFlush (MAO mao);

CARTA flushValue (MAO mao, CARTA card);

int isFourOfAKind (MAO mao);

int isFullHouse (MAO mao);

CARTA fullHouseValue(MAO mao, CARTA card);

MAO addStraight (MAO mao, int naipe, int valor);

MAO jogaStraight (JOGO e, int naipe, int valor);

int jogar (JOGO e);