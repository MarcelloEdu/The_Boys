#ifndef MUNDO_H
#define MUNDO_H

#include "entidades.h"

struct mundo
{
    struct heroi herois[N_HEROIS];
    struct base bases[N_BASES];
    struct missao missoes[N_MISSOES];
    struct conjunto *habilidades;
    struct lef_t *eventos;
    int quant_missoes;
    int relogio;
    int missoes_cumpridas;
    int missoes_adiadas;
    int mortalidade;
};

struct equipes{
    int base;
    int distancia;
};

struct mundo *inicia_mundo(struct  mundo *mundo);

void imprime_mundo(struct mundo *mundo);

#endif