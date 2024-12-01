#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "fprio.h"
#include "entidades.h"
#include "conjunto.h"
#include "eventos.h"

// Função para criar um evento
struct evento_t* cria_evento(int tempo, int tipo, void* dado1, void* dado2)
{
    struct evento_t* evento = malloc(sizeof(struct evento_t));
    if (evento == NULL) {
        return NULL;
    }

    evento->tempo = tempo;
    evento->tipo = tipo;
    evento->dado1 = dado1;
    evento->dado2 = dado2;

    return evento;
}

// Função para destruir um evento e liberar a memória
void destroi_evento(struct evento_t* evento)
{
    if (evento != NULL) {
        free(evento);  // Libera a memória do evento
    }
}

// Função para inicializar o mundo
void inicia_mundo(struct mundo *mundo)
{
    mundo->relogio = T_INICIO;
    mundo->missoes_cumpridas = 0;
    mundo->missoes_adiadas = 0;
    mundo->quant_missoes = N_MISSOES;
    mundo->habilidades = cria_cjt(N_HABILIDADES);
    mundo->eventos = fprio_cria();  // Usando fprio_t ao invés de LEF

    int tempo;
    int i;

    // Inicializa as bases
    for (i = 0; i < N_BASES; i++)
    {
        inicia_base(&mundo->bases[i], i);
    }

    // Inicializa os heróis e insere eventos na fila de prioridade
    int base;
    for (i = 0; i < N_HEROIS; i++)
    {        
        base = aleat(0, N_BASES - 1);
        tempo = aleat(0, 4320);  // Define o tempo de chegada do herói
        inicia_heroi(&mundo->herois[i], i);

        // Cria o evento de chegada e insere na fila de prioridade
        // Passa o id do herói como dado1 e a base como dado2
        struct evento_t *ev = cria_evento(tempo, TIPO_CHEGA, &mundo->herois[i], &mundo->bases[base]);
        fprio_insere(mundo->eventos, ev, TIPO_CHEGA, tempo);
    }
    
    // Inicializa as missões e insere eventos na fila de prioridade
    for (i = 0; i < N_MISSOES; i++)
    {
        inicia_missao(&mundo->missoes[i], i);
        tempo = aleat(0, T_FIM);  // Define o tempo para a missão acontecer
        
        // Cria o evento de missão e insere na fila de prioridade
        // Passa a missão como dado1 e o tempo como dado2
        struct evento_t *ev = cria_evento(tempo, TIPO_MISSAO, &mundo->missoes[i], NULL);
        fprio_insere(mundo->eventos, ev, TIPO_MISSAO, tempo);
        mundo->quant_missoes++;
    }

    // Cria o evento de fim e insere na fila de prioridade
    struct evento_t *fim_ = cria_evento(T_FIM, TIPO_FIM, NULL, NULL);
    fprio_insere(mundo->eventos, fim_, TIPO_FIM, T_FIM);
}

// Função para imprimir o mundo
void imprime_mundo(struct mundo *mundo)
{
    fprio_imprime(mundo->eventos);  // Supondo que fprio_imprime imprime os eventos da fila
}
