#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fprio.h"
#include "lista.h"
#include "fila.h"
#include "conjunto.h"
#include "eventos.h"
#include "entidades.h"
#include "mundo.h"
#include "complementos.h"


void chega(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio){

    if(h->status == 1)
        return;
    
    h->base_atual->id = b->id;


    if (cjto_card(b->presentes) < b->lotacao && fila_vazia(b->espera)) {
        //espera=true
        printf("%6d: CHEGA HEROI %2d BASE %d (%d/%d) ESPERA\n",
                tempo,
                h->id,
                b->id,
                cjto_card(b->presentes),
                b->lotacao);

        CriaInsere(tempo, TIPO_ESPERA, h->id, b->id, fprio);
        mundo->eventos_tratados++;
    }else
    {
        //espera =  (paciência de H) > (10 * tamanho da fila em B)
        if(h->paciencia > 10 * fila_tamanho(b->espera))
        {
                printf("%6d: CHEGA HEROI %2d BASE %d (%d/%d) ESPERA \n",
                tempo,
                h->id,
                b->id,
                cjto_card(b->presentes),
                b->lotacao);


            CriaInsere(tempo, TIPO_ESPERA, h->id, b->id, fprio);
            mundo->eventos_tratados++;
        }
        else
        {
                printf("%6d: CHEGA HEROI %2d BASE %d (%d/%d) DESISTE \n",
                tempo,
                h->id,
                b->id,
                cjto_card(b->presentes),
                b->lotacao);

            CriaInsere(tempo, TIPO_DESISTE, h->id, b->id, fprio);
            mundo->eventos_tratados++;
        }
    }

}

void espera(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio) {
    enqueue(b->espera, h->id);
    CriaInsere(tempo, TIPO_AVISA, b->id, -1, fprio);
    printf("%6d: ESPERA HEROI %2d BASE %d (%d) \n", 
           tempo, h->id, b->id, fila_tamanho(b->espera));
    mundo->eventos_tratados++;
}

void desiste(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio){
    h->base_destino->id = rand() % N_BASES;
    CriaInsere(tempo, TIPO_VIAJA, h->id, b->id, fprio);
    mundo->eventos_tratados++;
    printf("%6d: DESISTE HEROI %2d BASE %d \n",
            tempo, 
            h->id, 
            b->id);
}

void avisa(int tempo, struct base *b, struct mundo *mundo, struct fprio_t *fprio){
    int heroi_id = mundo->herois[0].id;
    while (cjto_card(b->presentes) < b->lotacao && !fila_vazia(b->espera)){//enquanto houver vaga e fila de espera

        dequeue(b->espera, &heroi_id);//retira o heroi da fila de espera (&heroi_id é atribuido como referencia(&) ao primeiro heroi da fila)

        printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA ",
                tempo,
                b->id, 
                cjto_card(b->presentes),
                b->lotacao);
            
        fila_imprime(b->espera);
    

        cjto_insere(b->presentes, heroi_id);//cjto_insere pede um int, por isso o uso de heroi_id
    
        CriaInsere(tempo, TIPO_ENTRA, heroi_id, b->id, fprio);
    
        mundo->eventos_tratados++;

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d \n",
                tempo,
                b->id,
                heroi_id);
    }
}

void entra(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio){

    if(h->status == 1)
        return;

    h->base_atual->id = b->id;//atualiza base atual do heroi

    struct cjto_t *novo_habilidades = cjto_uniao(b->habilidades, h->habilidades);
    cjto_destroi(b->habilidades); // Agora é seguro destruir o conjunto antigo
    b->habilidades = novo_habilidades; // Atualiza para o novo conjunto
    
    int TPB = 15 + h->paciencia * (rand() % 20 + 1);
        
    CriaInsere(tempo+TPB, TIPO_SAI, h->id, b->id, fprio);
    
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

    /* Verificação para retirar habilidades da base, mas só se não houver duplicatas de outros heróis */
    for (int i = 0; i < N_HABILIDADES; i++) {  // para cada habilidade
        if (cjto_pertence(b->habilidades, i) && i != h->id) {  // se a habilidade pertence à base e não é a do herói
            if (!(cjto_contem(h->habilidades, mundo->herois[i].habilidades))) {  // se a habilidade não pertence a outro herói que ainda está na base
                for (int j = 0; j < N_HABILIDADES; j++) {  // para cada habilidade
                    if (cjto_pertence(h->habilidades, j) && cjto_pertence(mundo->herois[i].habilidades, j)) {  // se a habilidade do herói for a mesma do herói que continua na base
                        cjto_retira(b->habilidades, j);  // retira a habilidade da base
                    }
                }
            }
        }
    }

    if(h->status == 1)
        return;

    // Remover o herói do conjunto de presentes da base
    cjto_retira(b->presentes, h->id);

    // Atualizar o destino do herói
    h->base_destino->id = rand() % N_BASES;

    // Criar novos eventos
    CriaInsere(tempo, TIPO_VIAJA, h->id, b->id, fprio);
    mundo->eventos_tratados++;
    CriaInsere(tempo, TIPO_AVISA, b->id, -1, fprio);
    mundo->eventos_tratados++;

    // Imprimir mensagem de saída apenas uma vez
    printf("%6d: SAI HEROI %2d BASE %d (%d/%d)\n",
           tempo,
           h->id,
           b->id,
           cjto_card(b->presentes),
           b->lotacao);
}

void viaja(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio){
    
    if(h->status == 1)
        return;

    int dist = distancia_euclidiana(b->coord, h->base_destino->coord);
    int duracao = dist / h->velocidade;

    int base_anterior = h->base_atual->id;

    h->base_atual->id = h->base_destino->id;

    int tempo_inserir = tempo + duracao;

    CriaInsere(tempo_inserir, TIPO_CHEGA, h->id, h->base_destino->id, fprio);
    mundo->eventos_tratados++;

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d \n",
            tempo,
            h->id,
            base_anterior,
            h->base_atual->id,
            dist,
            h->velocidade,
            tempo + duracao);
}

void missao(int tempo, struct missao *m, struct mundo *mundo, struct fprio_t *fprio){ 
    
    struct base *bases_aptas[N_BASES];//vetor de bases aptas a cumprir a missão
    int aptas = 0;
    int dia_seguinte = tempo + 24*60;
    mundo->tentativas_total++;

    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ",
            tempo,
            m->id,
            m->tentativas);

    cjto_imprime(m->habilidades);
    printf("] \n");

    //verifica se a base possui as habilidades necessárias para cumprir a missão
    for(int i = 0; i < N_BASES; i++){
        struct base *b = &mundo->bases[i]; //base atual
        if(cjto_contem(b->habilidades, m->habilidades) && b->lotacao != 0){
            bases_aptas[aptas] = b;//adiciona base atual (marcada como apta) ao vetor
            aptas++;
        }
    }

    //se não houver bases aptas a cumprir a missão, adia
    if(aptas == 0){
        printf("%6d: MISSAO %d IMPOSSIVEL \n",
                tempo,
                m->id);
        CriaInsere(dia_seguinte, TIPO_MISSAO, m->id, -1, fprio); //adia p dia seguinte
        mundo->eventos_tratados++;
        m->tentativas++;                                        //atualiza tentativas

            // Tratamento com min/max
    // Tratamento com min/max
    if (m->tentativas < mundo->tentativas_min) {
        mundo->tentativas_min = m->tentativas;
    }

    if (m->tentativas > mundo->tentativas_max) {
        mundo->tentativas_max = m->tentativas; // Atualiza o máximo
    }

        return;
    }

    quicksort(bases_aptas, 0, aptas-1, m);//ordena bases aptas pela distância euclidiana

    //base mais próxima da missão
    int BMP = bases_aptas[0]->id;


    printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [",
            tempo,
            m->id,
            BMP);
    
    cjto_imprime(mundo->bases[BMP].habilidades);
    printf(" ] \n");

    mundo->bases[BMP].missoes_cumpridas++;               //atualiza missões cumpridas
    mundo->missoes_cumpridas++;                         //atualiza missões cumpridas
    m->tentativas++;                                    //atualiza tentativas (essa bem sucedida)

    // Tratamento com min/max
    if (m->tentativas < mundo->tentativas_min) {
        mundo->tentativas_min = m->tentativas;
    }

    if (m->tentativas > mundo->tentativas_max) {
        mundo->tentativas_max = m->tentativas; // Atualiza o máximo
    }

    //para cada heroi presente na base mais próxima
    for(int i = 0; i < N_HEROIS; i++){
        if(cjto_pertence(mundo->bases[BMP].presentes, i)){
            if(mundo->herois->status == 0){
                int heroi_id = mundo->herois[i].id;
                struct heroi *h = &mundo->herois[heroi_id];

                int risco = m->perigo / (h->paciencia + h->experiencia + 1);

                if(risco > aleat(0, 30)){
                    CriaInsere(tempo, TIPO_MORRE, h->id, BMP, fprio);
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
    }
}

void morre(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio){
    cjto_retira(b->presentes, h->id);
    h->status = 1;
    CriaInsere(tempo, TIPO_AVISA, b->id, -1, fprio);
    mundo->eventos_tratados++;

}

void fim(struct mundo *mundo)
{
    int i;
    double percentual_missoes_cumpridas;

    printf("%d: FIM\n", T_FIM);

    //Heróis: vivos/mortos e estatísticas individuais
    for (int i = 0; i < N_HEROIS; i++) {
        struct heroi *h = &mundo->herois[i];

        if (h->status==1) {
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [", 
                   i, 
                   h->paciencia, 
                   h->velocidade, 
                   h->experiencia);

        } else {
            printf("HEROI %2d VIVO  PAC %3d VEL %4d EXP %4d HABS [", 
                   i,
                   h->paciencia, 
                   h->velocidade, 
                   h->experiencia);
        }

        // Imprime as habilidades do herói
        cjto_imprime(h->habilidades);
        printf("]\n");

        //desaloca os atributos do heroi
        destroi_heroi(h);
    }

    // Imprime informações sobre as bases
    for (i = 0; i < N_BASES; i++) {
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n",
            i,
            mundo->bases[i].lotacao,
            fila_tamanho(mundo->bases[i].espera),
            mundo->bases[i].missoes_cumpridas);

        //libera as coordenadas da base
        free(mundo->bases[i].coord);
        cjto_destroi(mundo->bases[i].habilidades); // Libera o conjunto de habilidades
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

    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f \n",
           mundo->tentativas_min,
           mundo->tentativas_max,
           (double)mundo->tentativas_total / mundo->quant_missoes);


    printf("TAXA MORTALIDADE: %.1f%%\n", (double)mundo->mortalidade * 100 / N_HEROIS);

    // Libera o conjunto de habilidades do mundo
    cjto_destroi(mundo->habilidades);

    // Libera a fila de fprio
    fprio_destroi(mundo->eventos); 

    // Libera as coordenadas das missões
    for (i = 0; i < N_MISSOES; i++) {
        free(mundo->missoes[i].coordenadas);
        cjto_destroi(mundo->missoes[i].habilidades);
    }


}
