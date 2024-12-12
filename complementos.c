#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "complementos.h"
#include "eventos.h"
#include "fprio.h"
#include "entidades.h"
#include "mundo.h"



#define RUN 32


int aleat(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

// Função para criar um evento
// Recebe o tempo, o tipo do evento, e dois ponteiros para inteiros
// que serão utilizados para passar os dados do evento
// Retorna um ponteiro para o evento criado
struct evento_t* cria_evento(int tempo, int tipo, int dado1, int dado2){
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

//função para particionar o vetor
//escolhe um pivô e coloca os elementos menores à esquerda e os maiores à direita
//custo = n
int particionar(struct base *bases[], int inicio, int fim, const struct missao *m) 
{
    // Escolhe aleatoriamente o pivô
    int aleatorio = aleat(inicio, fim);
    struct base *pivo = bases[aleatorio];
    
    int i = (inicio - 1); // índice do menor elemento

    for (int j = inicio; j <= fim - 1; j++) 
    {
        // Calcula a distância entre a missão e a base
        int distPivo = distancia_euclidiana(pivo->coord, m->coordenadas);
        int distBaseJ = distancia_euclidiana(bases[j]->coord, m->coordenadas);

        if (distBaseJ <= distPivo) 
        {
            i++; // incrementa o índice do menor elemento
            // Troca as bases
            struct base *temp = bases[i];
            bases[i] = bases[j];
            bases[j] = temp;
        }
    }
    // Coloca o pivô na posição correta
    struct base *temp = bases[i + 1];
    bases[i + 1] = bases[fim];
    bases[fim] = temp;

    return (i + 1); // retorna o índice onde o pivô está agora
}


//função de "ordenação rápida" recursiva
//escolhe um pivô, particiona o vetor e ordena os subvetores
//custo = n*log(n)(melhor caso) ou n^2(pior caso)
//o pior caso ocorre quando o vetor está ordenado ou quase ordenado
//o melhor caso ocorre quando o pivô divide o vetor em duas partes iguais
//escolhi fazer com pivo aleatório para evitar o pior caso
void quicksort(struct base *bases[], int inicio, int fim, const struct missao *m) 
{
    if (inicio < fim) 
    {
        // Particiona as bases e obtém o índice do pivô
        int pi = particionar(bases, inicio, fim, m);
        // Ordena recursivamente as duas partes
        quicksort(bases, inicio, pi - 1, m);
        quicksort(bases, pi + 1, fim, m);
    }
}

// Função para calcular a distância euclidiana entre duas coordenadas
// Utilizada para calcular a distância entre as bases e as missões
// será util para selecionar a base mais próxima da missão
// pois a base mais próxima é a que tem a menor distância euclidiana
// que será calculada com essa função
// e ordenada com o timsort
int distancia_euclidiana(struct coordenadas *coord1, struct coordenadas *coord2){
    if (!coord1 || !coord2) {
        fprintf(stderr, "Erro: coordenadas inválidas.\n");
        return -1;  // Ou outro valor que indique erro
    }

    int x = coord1->x - coord2->x;
    int y = coord1->y - coord2->y;

    return sqrt(x*x + y*y);
}

//como em todas as funções de eventos é necessário criar um evento e inserir na fila de prioridade
//essa função foi criada para facilitar a leitura do código e manter ele mais limpo
void CriaInsere(int tempo, int tipo, int dado1, int dado2, struct fprio_t *fprio){
    struct evento_t *evento = cria_evento(tempo, tipo, dado1, dado2);
    fprio_insere(fprio, evento, tipo, tempo);
}

void destroi_missao(struct missao *missao) {
    if (missao->habilidades) {
        cjto_destroi(missao->habilidades);
        missao->habilidades = NULL;
    }
    if (missao->coordenadas) {
        free(missao->coordenadas);
        missao->coordenadas = NULL;
    }
}



void destroi_heroi(struct heroi *h) {
    if (h->habilidades) {
        cjto_destroi(h->habilidades);
        h->habilidades = NULL;
    }
    if (h->base_atual) {
        free(h->base_atual->coord);
        free(h->base_atual);
        h->base_atual = NULL;
    }
    if (h->base_destino) {
        free(h->base_destino->coord);
        free(h->base_destino);
        h->base_destino = NULL;
    }
}


