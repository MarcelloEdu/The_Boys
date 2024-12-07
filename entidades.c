
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entidades.h"
#include "conjunto.h"
#include "fila.h"
#include "complementos.h"

struct base inicia_base(struct base *base, int id) 
{
    base->id = id;
    base->lotacao = aleat(3, 10);
    base->presentes = cjto_cria(base->lotacao);
    base->espera = fila_cria();
    base->habilidades = cjto_cria(N_HABILIDADES);
    
    base->coord = malloc(sizeof(struct coordenadas));
    base->coord->x = aleat(0, TAMANHO_MUNDO -1);
    base->coord->y = aleat(0, TAMANHO_MUNDO -1);
    return *base;
}

struct heroi inicia_heroi(struct heroi *heroi, int id) {
    heroi->id = id;
    heroi->experiencia = 0;
    heroi->paciencia = aleat(0, 100);
    heroi->velocidade = aleat(50, 5000);
    heroi->base_atual = NULL;
    int i;
    int numero_habilidades = aleat(1, 3); 
    heroi->habilidades = cjto_cria(numero_habilidades);

    for (i = 0; i < numero_habilidades; ++i) 
    {
        int habilidade = aleat(0, N_HABILIDADES - 1);
        cjto_insere(heroi->habilidades, habilidade);
    }

    return *heroi;
}

void inicia_missao(struct missao *missao, int id) {
    
    missao->coordenadas = malloc(sizeof(struct coordenadas));
    
    missao->id = id;
    missao->coordenadas->x = aleat(0, TAMANHO_MUNDO - 1);
    missao->coordenadas->y = aleat(0, TAMANHO_MUNDO - 1);
    int i;
    int numero_habilidades = aleat(6, 10); 
    missao->habilidades = cjto_cria(numero_habilidades);
    missao->tentativas = 1;
    missao->adiamentos = 0;
    missao->tentativas_max = 0;

    for (i = 0; i < numero_habilidades; ++i) 
    {
        int habilidade = aleat(0, N_HABILIDADES - 1);
        cjto_insere(missao->habilidades, habilidade);
    }
}