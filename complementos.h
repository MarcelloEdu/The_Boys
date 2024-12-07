#ifndef COMPLEMENTOS_H
#define COMPLEMENTOS_H

#include "conjunto.h"
#include "entidades.h"
#include "eventos.h"
#include "fprio.h"
#include "lista.h"
#include "mundo.h"

/* Headerfile de funções complementares que vão ser necessárias no desenvolvimento */
/* Pensada para facilitar a leitura do código e mantê-lo mais limpo */

// Função para criar um evento
struct evento_t* cria_evento(int tempo, int tipo, void* dado1, void* dado2);

// Função para destruir um evento e liberar a memória
void destroi_evento(struct evento_t* evento);

//quicksort adaptado do trabalho de algoritmos e estruturas de dados 2
//adaptado para lidar com vetores de ponteiros para struct base
//e ordenar as bases de acordo com a distância euclidiana entre a base e a missão
int quicksort(struct base *bases[], int inicio, int fim, const struct missao *m);

// Distancia euclideana pra calcular a distancia entre as bases e as missões
int distancia_euclidiana(struct coordenadas *coord1, struct coordenadas *coord2);

// Função para criar e inserir na LEF
void CriaInsere(int tempo, int tipo, void* dado1, void* dado2, struct fprio_t *fprio);

//função pra aleatorizar
int aleat(int min, int max);

#endif

