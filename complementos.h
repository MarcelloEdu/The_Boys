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

// Timsort adaptado do trabalho de algortimos 2
int timsort(int vetor[], int tam);

// Distancia euclideana pra calcular a distancia entre as bases e as missões
int distancia_euclidiana(struct coordenadas *coord1, struct coordenadas *coord2);

// Função para criar e inserir na LEF
void CriaInsere(int tempo, int tipo, void* dado1, void* dado2, struct fprio_t *fprio);


#endif

