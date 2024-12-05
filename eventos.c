#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fprio.h"
#include "lista.h"
#include "fila.c"
#include "conjunto.h"
#include "eventos.h"
#include "entidades.h"
#include "mundo.h"
#include "complementos.h"


void chega(int tempo, struct heroi *h, struct base *b, struct fprio_t *fprio){
    h->base_atual->id = b->id; //atualiza base de H

    if (b->lotacao > cjto_card(b->presentes) && fila_vazia(b->espera))
    {
        printf ("%6d: CHEGA  HEROI %2d BASE %d (%d/%d) \n", 
                tempo, 
                h->id, 
                b->id, 
                cjto_card(b->presentes), 
                b->lotacao);

        cjto_insere(b->presentes, h->id); 
        
        CriaInsere(tempo, TIPO_ENTRA, h, b, fprio);
    }
    else if (h->paciencia > 10 * fila_tamanho(b->espera)) 
    {
        printf("%6d: CHEGA  HEROI %2d BASE %d (%d/%d) ESPERA \n", 
                tempo, 
                h->id, 
                b->id, 
                cjto_card(b->presentes), 
                b->lotacao);
        CriaInsere(tempo, TIPO_ESPERA, h, b, fprio);
    } 
    else
    {
        printf("%6d: CHEGA  HEROI %2d BASE %d (%d/%d) DESISTE \n", 
                tempo, 
                h->id, 
                b->id, 
                cjto_card(b->presentes), 
                b->lotacao);
        CriaInsere(tempo, TIPO_DESISTE, h, b, fprio);
    }
}

void espera(int tempo, struct heroi *heroi, struct base *base, struct fprio_t *fprio) {
    enqueue(base->espera, heroi->id);
    CriaInsere(tempo, TIPO_AVISA, base, NULL, fprio);
    printf("%6d: ESPERA HEROI %2d BASE %d (%d) \n",
            tempo, 
            heroi->id, 
            base->id, 
            fila_tamanho(base->espera));
}

void desiste(int tempo, struct heroi *h, struct base *b, struct fprio_t *fprio){
    h->base_destino->id = rand() % N_BASES;
    CriaInsere(tempo, TIPO_VIAJA, h, b, fprio);
    printf("%6d: DESISTE HEROI %2d BASE %d \n",
            tempo, 
            h->id, 
            b->id);
}

void avisa(int tempo, struct base *b, struct fprio_t *fprio){
    printf("%6d: AVISA PORTEIRO BASE %d (%d/%d) \n",
            tempo,
            b->id, 
            cjto_card(b->presentes),
            b->lotacao);
    int heroi_id;
    while (cjto_card(b->presentes) < b->lotacao && !fila_vazia(b->espera)){//enquanto houver vaga e fila de espera
        int id = dequeue(b->espera, &heroi_id);//heroi_id passado como referencia pois dequeue retorna o valor
        cjto_insere(b->presentes, heroi_id);
        CriaInsere(tempo, TIPO_ENTRA, id, b, fprio);

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d \n",
                tempo,
                b->id,
                heroi_id);
    }
}

void entra(int tempo, struct heroi *h, struct base *b, struct fprio_t *fprio){
    cjto_uniao(b->habilidades, h->habilidades);//adiciona habilidades do heroi ao conjunto de habilidades da base
    int TPB = 15 + h->paciencia * (rand() % 20 + 1);
    tempo += TPB;
    CriaInsere(tempo, TIPO_SAI, h, b, fprio);
    cjto_insere(b->presentes, h->id);

    printf("%6d: ENTRA HEROI %2d BASE %d (%d/%d) TPB %d \n",
            tempo,
            h->id,
            b->id,
            cjto_card(b->presentes),
            b->lotacao,
            TPB);
}

void sai(int tempo, struct heroi *h, struct base *b, struct fprio_t *fprio){
    cjto_retira(b->habilidades, h->habilidades);
    cjto_retira(b->presentes, h->id);
    h->base_destino->id = rand() % N_BASES;
    CriaInsere(tempo, TIPO_VIAJA, h, b, fprio);
    CriaInsere(tempo, TIPO_AVISA, b, NULL, fprio);

    printf("%6d: SAI HEROI %2d BASE %d (%d/%d) \n",
            tempo,
            h->id,
            b->id,
            cjto_card(b->presentes),
            b->lotacao);
}

void viaja(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio){
    
    int dist = distancia_euclidiana(b->coord, h->base_destino->coord);
    int duracao = dist / h->velocidade;
    tempo += duracao;
    h->base_atual = h->base_destino->id;
    CriaInsere(tempo, TIPO_CHEGA, h, b, fprio);

    printf("%6d: VIAJA HEROI %2d BASE %d -> BASE %d (%d) \n",
            tempo,
            h->id,
            h->base_atual,
            b->id,
            duracao);
}

/*
MISSAO (T, M):

calcula a distância de cada base ao local da missão M
encontra BMP = base mais próxima da missão cujos heróis possam cumpri-la 
se houver uma BMP:
    marca a missão M como cumprida
    para cada herói H presente na BMP:
        risco = perigo (M) / (paciência (H) + experiência (H) + 1.0)
        se risco > aleatório (0, 30):
            cria e insere na LEF o evento MORRE (agora, H)
        senão:
            incrementa a experiência de H
senão:
    cria e insere na LEF o evento MISSAO (T + 24*60, M) para o dia seguinte
*/
void missao(int tempo, struct missao *m, struct mundo *mundo, struct fprio_t *fprio){
    
    struct base *bases_aptas[N_BASES];//vetor de bases aptas a cumprir a missão
    int aptas = 0;
    int dia_seguinte = tempo + 24*60;

    printf("%6d: MISSAO %d HAB REQ: ",
            tempo,
            m->id);

    cjto_imprime(m->habilidades);

    //verifica se a base possui as habilidades necessárias para cumprir a missão
    for(int i = 0; i < N_BASES; i++){
        struct base *b = &mundo->bases[i]; //base atual

        if(cjto_contem(b->habilidades, m->habilidades)){
            bases_aptas[aptas] = b;//adiciona base atual (marcada como apta) ao vetor
            aptas++;
        }
    }

    //se não houver bases aptas a cumprir a missão, adia
    if(aptas == 0){
        printf("%6d: MISSAO %d IMPOSSIVEL \n",
                tempo,
                m->id);
        CriaInsere(dia_seguinte, TIPO_MISSAO, m, NULL, fprio);
        return;
    }

    quicksort(bases_aptas, 0, aptas-1, m);//ordena bases aptas pela distância euclidiana

    //base mais próxima da missão
    int BMP = bases_aptas[0]->id;

    //marca missão como cumprida
    mundo->missoes_cumpridas++;
    printf("%6d: MISSAO %2d CUMPRIDA HEROIS %d \n",
            tempo,
            m->id,
            cjto_card(bases_aptas[BMP]->presentes));

    //para cada heroi presente na base mais próxima
    for(int i = 0; i < cjto_card(bases_aptas[BMP]->presentes); i++){

        int heroi_id = cjto_card(bases_aptas[BMP]->presentes);
        struct heroi *h = &mundo->herois[heroi_id];

        float risco = m->perigo / (h->paciencia + h->experiencia + 1.0);

        if(risco > aleat(0, 30)){
            CriaInsere(tempo, TIPO_MORRE, h, bases_aptas[BMP], fprio);
        }else{
            h->experiencia++;
        }
    }

}

void morre(int tempo, struct heroi *h, struct base *b, struct fprio_t *fprio){
    cjto_retira(b->presentes, h->id);
    h->status = 1;
    CriaInsere(tempo, TIPO_AVISA, b, NULL, fprio);

}

/*
FIM (T):
  encerra a simulação
  apresenta estatísticas dos heróis
  apresenta estatísticas das bases
  apresenta estatísticas das missões
*/
void fim(int tempo, struct fprio_t *fprio){

}
