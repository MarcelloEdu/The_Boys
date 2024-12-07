#ifndef MUNDO_H
#define MUNDO_H

#include "fprio.h"
#include "entidades.h"  //(heróis, missões, bases)

// Estrutura que representa um evento
struct evento_t {
    int tempo;      // Tempo em que o evento ocorre
    int tipo;       // Tipo do evento (CHEGA, MISSAO, FIM, etc.)
    int dado1;    // Dado associado ao evento (pode ser um ID de herói, por exemplo)
    int dado2;    // Segundo dado associado ao evento (pode ser um ID de missão ou outro dado)
};

// Estrutura que representa o "mundo" do jogo
struct mundo {
    struct heroi herois[N_HEROIS];
    struct base bases[N_BASES];
    struct missao missoes[N_MISSOES];
    struct missao tentativas_min;
    struct missao tentativas_max;
    struct cjto_t *habilidades;
    struct fprio_t *eventos;
    int quant_missoes;
    int relogio;
    int missoes_cumpridas;
    int missoes_adiadas;
    int mortalidade;
    int eventos_tratados;
};

// Funções para criar, manipular e remover eventos
void destroi_evento(struct evento_t* evento);  // Função para destruir eventos
// Funções para manipular o mundo
void inicia_mundo(struct mundo *mundo);  // Função para inicializar o mundo
void imprime_mundo(struct mundo *mundo);  // Função para imprimir o estado do mundo

#endif
