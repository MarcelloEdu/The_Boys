#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mundo.h"
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "eventos.h"

struct item_m
{
  int dado1 ;
  int dado2 ;
} ;

struct item_m *item_cria (int a, int b)
{
  struct item_m *item ;

  item = malloc (sizeof (struct item_m)) ;
  if (!item)
    abort () ;

  item->dado1  = a ;
  item->dado2  = b ;

  return (item) ;
}


// Função para inicializar o mundo
void inicia_mundo(struct mundo *mundo)
{
    mundo->quant_missoes = N_MISSOES;
    mundo->relogio = T_INICIO;
    mundo->missoes_cumpridas = 0;
    mundo->missoes_adiadas = 0;
    mundo->mortalidade = 0;
    mundo->eventos = fprio_cria();

    int tempo;
    int i;

    for(i=0; i < N_BASES; i++){
        inicia_base(&mundo->bases[i], i);
    }

    int base;

/*

struct fpnodo_t
{
  void *item ;			// item associado ao nodo
  int   tipo ;			// tipo do item
  int   prio ;			// prioridade do item
  struct fpnodo_t *prox;	// próximo nodo
};

*/
    for(i = 0; i < N_HEROIS; i++){
        base = aleat(0, N_BASES - 1);
        tempo = aleat(0, 4320);
        inicia_heroi(&mundo->herois[i], i);

        struct fpnodo_t *ev = fprio_insere(, i, TIPO_CHEGA. );//fprio, item, tipo, prioridade
    }
}

// Função para imprimir o mundo
void imprime_mundo(struct mundo* mundo) {


}
