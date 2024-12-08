#ifndef EVENTOS_H
#define EVENTOS_H

#include "fprio.h"
#include "lista.h"
#include "entidades.h"
#include "mundo.h"
#include "complementos.h"

#define TIPO_CHEGA 1
#define TIPO_ESPERA 2
#define TIPO_DESISTE 3
#define TIPO_AVISA 4
#define TIPO_ENTRA 5
#define TIPO_SAI 6
#define TIPO_VIAJA 7
#define TIPO_MISSAO 8
#define TIPO_FIM 9
#define TIPO_MORRE 10



void chega(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);

void espera(int tempo, struct heroi *heroi, struct base *base, struct mundo *mundo, struct fprio_t *fprio);

void desiste(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);

void avisa(int tempo, struct base *b, struct mundo *mundo, struct fprio_t *fprio);

void entra(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);

void sai(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);

void viaja(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);

void missao(int tempo, struct missao *m, struct mundo *mundo, struct fprio_t *fprio);

void morre(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);

void fim(struct mundo *mundo);

#endif