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


void chega(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio){
    h->base_atual->id = b->id; //atualiza base de H

    if (h->paciencia > 10 * fila_tamanho(b->espera)){
        printf("%6d: CHEGA  HEROI %2d BASE %d (%d/%d) ESPERA \n", 
                tempo, 
                h->id, 
                b->id, 
                cjto_card(b->presentes), 
                b->lotacao);
        CriaInsere(tempo, TIPO_ESPERA, h, b, fprio);
        mundo->eventos_tratados++;
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
        mundo->eventos_tratados++;
    }
}

void espera(int tempo, struct heroi *heroi, struct base *base, struct mundo *mundo, struct fprio_t *fprio) {
    enqueue(base->espera, heroi->id);
    CriaInsere(tempo, TIPO_AVISA, base, NULL, fprio);
    mundo->eventos_tratados++;
    printf("%6d: ESPERA HEROI %2d BASE %d (%d) \n",
            tempo, 
            heroi->id, 
            base->id, 
            fila_tamanho(base->espera));
}

void desiste(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio){
    h->base_destino->id = rand() % N_BASES;
    CriaInsere(tempo, TIPO_VIAJA, h, b, fprio);
    mundo->eventos_tratados++;
    printf("%6d: DESISTE HEROI %2d BASE %d \n",
            tempo, 
            h->id, 
            b->id);
}

void avisa(int tempo, struct base *b, struct mundo *mundo, struct fprio_t *fprio){
    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ",
            tempo,
            b->id, 
            cjto_card(b->presentes),
            b->lotacao);
    cjto_imprime(b->presentes);
    printf("] \n");
    
    int heroi_id;
    while (cjto_card(b->presentes) < b->lotacao && !fila_vazia(b->espera)){//enquanto houver vaga e fila de espera

        dequeue(b->espera, &heroi_id);//retira o heroi da fila de espera (&heroi_id é atribuido como referencia(&) ao primeiro heroi da fila)
    
        cjto_insere(b->presentes, heroi_id);//cjto_insere pede um int, por isso o uso de heroi_id
    
        CriaInsere(tempo, TIPO_ENTRA, &mundo->herois[heroi_id], b, fprio);
    
        mundo->eventos_tratados++;

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d \n",
                tempo,
                b->id,
                heroi_id);
    }
}

void entra(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio){
    cjto_uniao(b->habilidades, h->habilidades);//adiciona habilidades do heroi ao conjunto de habilidades da base
    
    int TPB = 15 + h->paciencia * (rand() % 20 + 1);
    
    tempo += TPB;
    
    CriaInsere(tempo, TIPO_SAI, h, b, fprio);
    
    mundo->eventos_tratados++;
    
    cjto_insere(b->presentes, h->id);

    printf("%6d: ENTRA HEROI %2d BASE %d (%d/%d) SAI %d \n",
            tempo,
            h->id,
            b->id,
            cjto_card(b->presentes),
            b->lotacao,
            tempo+TPB);
}

void sai(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio) {
    // Remover todas as habilidades do herói das habilidades da base
    int habilidade;
    while (cjto_card(h->habilidades) > 0) { // gambiarra para poder passar a habilidade como int
        habilidade = cjto_retira(h->habilidades, 0); // Pega e remove uma habilidade do herói
        cjto_retira(b->habilidades, habilidade);    // Remove da base
    }

    // Remover o herói do conjunto de presentes da base
    cjto_retira(b->presentes, h->id);

    // Atualizar o destino do herói
    h->base_destino->id = rand() % N_BASES;

    // Criar novos eventos
    CriaInsere(tempo, TIPO_VIAJA, h, b, fprio);
    mundo->eventos_tratados++;
    CriaInsere(tempo, TIPO_AVISA, b, NULL, fprio);
    mundo->eventos_tratados++;

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

    int base_anterior = h->base_atual->id;

    h->base_atual->id = h->base_destino->id;
    CriaInsere(tempo, TIPO_CHEGA, h, b, fprio);
    mundo->eventos_tratados++;

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d \n",
            tempo,
            h->id,
            base_anterior,
            h->base_atual->id,
            dist,
            h->velocidade,
            tempo+duracao);
}

void missao(int tempo, struct missao *m, struct mundo *mundo, struct fprio_t *fprio){
    
    struct base *bases_aptas[N_BASES];//vetor de bases aptas a cumprir a missão
    int aptas = 0;
    int dia_seguinte = tempo + 24*60;

    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ",
            tempo,
            m->id,
            m->tentativas);

    cjto_imprime(m->habilidades);
    printf("] \n");

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
        CriaInsere(dia_seguinte, TIPO_MISSAO, m, NULL, fprio); //adia p dia seguinte
        mundo->eventos_tratados++;
        m->tentativas++;                                        //atualiza tentativas
        m->adiamentos++;                                        //atualiza adiamentos
        return;
    }

    quicksort(bases_aptas, 0, aptas-1, m);//ordena bases aptas pela distância euclidiana

    //base mais próxima da missão
    int BMP = bases_aptas[0]->id;

    printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [",
            tempo,
            m->id,
            BMP);
    
    cjto_imprime(bases_aptas[BMP]->habilidades);
    printf("] \n");


    bases_aptas[BMP]->missoes_cumpridas++;               //atualiza missões cumpridas
    mundo->missoes_cumpridas++;                         //atualiza missões cumpridas
    m->tentativas++;                                    //atualiza tentativas (essa bem sucedida)
    m->tentativas_max = m->tentativas + m->adiamentos;//atualiza tentativas máximas

    //para cada heroi presente na base mais próxima
    for(int i = 0; i < cjto_card(bases_aptas[BMP]->presentes); i++){

        int heroi_id = cjto_card(bases_aptas[BMP]->presentes);
        struct heroi *h = &mundo->herois[heroi_id];

        float risco = m->perigo / (h->paciencia + h->experiencia + 1.0);

        if(risco > aleat(0, 30)){
            CriaInsere(tempo, TIPO_MORRE, h, bases_aptas[BMP], fprio);
            mundo->eventos_tratados++;
            mundo->mortalidade++;
            printf("%6d: MORRE HEROI %2d MISSAO %d \n",
                    tempo,
                    h->id,
                    m->id);
        }else{
            h->experiencia++;
        }
    }

}

void morre(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio){
    cjto_retira(b->presentes, h->id);
    h->status = 1;
    CriaInsere(tempo, TIPO_AVISA, b, NULL, fprio);
    mundo->eventos_tratados++;

}

/*
FIM (T):
  encerra a simulação
  apresenta estatísticas dos heróis
  apresenta estatísticas das bases
  apresenta estatísticas das missões
*/
void fim(struct mundo *mundo)
{
    int i;
    double percentual_missoes_cumpridas;

    int tentativas_total;

    for(i = 0; i < N_MISSOES; i++){
        tentativas_total += mundo->missoes[i].tentativas;//calcula quantas tentativas foram feitas no total
    }

    printf("%d: FIM\n", T_FIM);

    //Heróis: vivos/mortos e estatísticas individuais
    for (int i = 0; i < N_HEROIS; i++) {
        struct heroi *h = &mundo->herois[i];

        if (h->status==1) {
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [", 
                   i, h->paciencia, h->velocidade, h->experiencia);

        } else {
            printf("HEROI %2d VIVO  PAC %3d VEL %4d EXP %4d HABS [", 
                   i, h->paciencia, h->velocidade, h->experiencia);
        }

        // Imprime as habilidades do herói
        cjto_imprime(h->habilidades);
        printf("]\n");
    }

    // Imprime informações sobre as bases
    for (i = 0; i < N_BASES; i++) {
        printf("BASE %d LOT %d FILA MAX %d MISSOES %2d\n",
            i,
            mundo->bases[i].lotacao,
            fila_tamanho(mundo->bases[i].espera),
            mundo->bases[i].missoes_cumpridas);

        cjto_destroi(mundo->bases[i].presentes); // Libera o conjunto de presentes
        fila_destroi(&mundo->bases[i].espera); // Libera a fila de espera
    }

    // Calcula e exibe as estatísticas de missões
    percentual_missoes_cumpridas = (double)mundo->missoes_cumpridas * 100 / mundo->quant_missoes;

    printf("EVENTOS TRATADOS: %d\n", mundo->eventos_tratados);
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n",
        mundo->missoes_cumpridas,
        mundo->quant_missoes,
        percentual_missoes_cumpridas);

    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1d",
        mundo->tentativas_min,
        mundo->tentativas_max,
        tentativas_total / mundo->quant_missoes);

    printf("TAXA MORTALIDADE: %.1f%%\n", (double)mundo->mortalidade * 100 / N_HEROIS);

    // Libera as missões
    for (i = 0; i < N_MISSOES; i++) {
        cjto_destroi(mundo->missoes[i].habilidades); // Libera as habilidades necessárias
    }

    // Libera o conjunto de habilidades do mundo
    cjto_destroi(mundo->habilidades);

    // Libera a fila de prioridade (fprio)
    fprio_destroi(mundo->eventos); // Adapte conforme o nome da função de destruição

    // Libera o próprio mundo
    free(mundo);
    mundo = NULL;
}
