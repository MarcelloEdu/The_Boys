#ifndef EVENTOS_H
#define EVENTOS_H

#include "fprio.h"
#include "lista.h"
#include "entidades.h"
#include "mundo.h"

#define TIPO_CHEGA 1
#define TIPO_ESPERA 2
#define TIPO_DESISTE 3
#define TIPO_AVISA 4
#define TIPO_ENTRA 5
#define TIPO_SAI 6
#define TIPO_VIAJA 7
#define TIPO_MISSAO 8
#define TIPO_MORRE 9
#define TIPO_FIM 10

void chega();//tempo, heroi, base
void espera();//tempo, heroi, base
void desiste();//tempo, heroi, base
void avisa();//tempo, base
void entra();//tempo, heroi, base
void sai();//tempo, heroi, base
void viaja();//tempo, heroi, destino
void missao();//tempo, mundo
void morre();//tempo, heroi, base
void fim(); //tempo

#endif