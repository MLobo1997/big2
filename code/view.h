#ifndef VIEW_H_ 
#define VIEW_H_

void iniciaHTML ();

void imprime_carta(int x, int y, ESTADO e , int m , int naipe, int valor);

void imprime_mao (int x , int y , ESTADO e , MAO mao , int m);

void imprime_botoes (int x , int y, ESTADO e);

void imprime_passar (int n);

void imprime(ESTADO e);

void finalizaHTML ();

#endif