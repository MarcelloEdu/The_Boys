#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mundo.h"
#include "fprio.h"


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
        inicia_base(&mundo->bases)
    }
}

// Função para imprimir o mundo
void imprime_mundo(struct mundo* mundo) {


}
