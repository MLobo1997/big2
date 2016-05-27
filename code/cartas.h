#ifndef CARTAS_H_ 
#define CARTAS_H_

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

#define FORMATO "%lld_%lld_%lld_%lld_%lld_%d_%lld_%lld_%lld_%lld_%d_%d_%d_%d_%d"

typedef long long int MAO;

struct state{
  MAO mao[4]; /*Maos de cada jogador*/
  MAO selecao; /*Cartas selecionadas*/
  MAO played[4]; /*Cartas jogadas*/
  int action, /*valor de jogada do joador proprio*/
   	  jogador, /*jogadr em mao*/
   	  nCartas, /*Numero de cartas em jogo por mão*/
	  passar, /*Número de vezes consecutivas que o*/
	  sort, /*Identifca a ordem das minhas cartas*/
	  end; /*Diz se o jogo já acabou ou não*/
};


typedef struct state ESTADO;

int indice(int naipe, int valor);

long long int add_carta(long long int est, int naipe, int valor);

long long int rem_carta(long long int est, int naipe, int valor);

MAO rem_cartas(MAO est, MAO rem);

MAO add_cartas(MAO est, MAO add);

int carta_existe(long long int est, int naipe, int valor);



ESTADO shuffle (ESTADO e);

ESTADO str2estado (char* str);

char* estado2str (ESTADO e);

ESTADO inicializa (ESTADO e);

#endif