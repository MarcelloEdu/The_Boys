
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entidades.h"
#include "conjunto.h"
#include "fila.h"
#include "complementos.h"


struct base inicia_base(struct base *base, int id) {
    base->id = id;
    base->missoes_cumpridas = 0;
    base->lotacao = aleat(3, 10);
    base->presentes = cjto_cria(N_HEROIS);
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

    heroi->base_atual = malloc(sizeof(struct base));
    heroi->base_atual->coord = malloc(sizeof(struct coordenadas));
    heroi->base_atual->coord->x = 0;
    heroi->base_atual->coord->y = 0;

    heroi->base_destino = malloc(sizeof(struct base));
    heroi->base_destino->coord = malloc(sizeof(struct coordenadas));
    heroi->base_destino->coord->x = 0;
    heroi->base_destino->coord->y = 0;

    int tam_hab = aleat(1, 3);
    heroi->habilidades = cjto_aleat(tam_hab, N_HABILIDADES);

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
    missao->perigo = aleat(0, 100);

    for (i = 0; i < numero_habilidades; ++i) 
    {
        int habilidade = aleat(0, N_HABILIDADES - 1);
        cjto_insere(missao->habilidades, habilidade);
    }
}