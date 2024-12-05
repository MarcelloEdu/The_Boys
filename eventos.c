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

/*
{
CHEGA (T, H, B):

atualiza base de H

se há vagas em B e a fila de espera em B está vazia:
    espera = true
senão:
    espera = (paciência de H) > (10 * tamanho da fila em B)

se espera:
    cria e insere na LEF o evento ESPERA (agora, H, B)
senão:
    cria e insere na LEF o evento DESISTE (agora, H, B)
}
*/
void chega(int tempo, struct heroi *h, struct base *b, struct fprio_t *fprio){
    h->base_atual = b->id;

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


/*
ESPERA (T, H, B):

adiciona H ao fim da fila de espera de B
cria e insere na LEF o evento AVISA (agora, B)
*/
void espera(int tempo, struct heroi *heroi, struct base *base, struct fprio_t *fprio) {
    enqueue(base->espera, heroi->id);
    CriaInsere(tempo, TIPO_AVISA, base, NULL, fprio);
    printf("%6d: ESPERA HEROI %2d BASE %d (%d) \n",
            tempo, 
            heroi->id, 
            base->id, 
            fila_tamanho(base->espera));
}

/*
DESISTE (T, H, B):

escolhe uma base destino D aleatória
cria e insere na LEF o evento VIAJA (agora, H, D)
*/
void desiste(int tempo, struct heroi *h, struct base *b, struct fprio_t *fprio){
    h->base_destino = rand() % N_BASES;
    CriaInsere(tempo, TIPO_VIAJA, h, b, fprio);
    printf("%6d: DESISTE HEROI %2d BASE %d \n",
            tempo, 
            h->id, 
            b->id);
}

/*
AVISA (T, B):

enquanto houver vaga em B e houver heróis esperando na fila:
    retira primeiro herói (H') da fila de B
    adiciona H' ao conjunto de heróis presentes em B
    cria e insere na LEF o evento ENTRA (agora, H', B)
*/
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

/*
ENTRA (T, H, B):

calcula TPB = tempo de permanência na base:
    TPB = 15 + paciência de H * aleatório [1...20]
cria e insere na LEF o evento SAI (agora + TPB, H, B)
*/
void entra(int tempo, struct heroi *h, struct base *b, struct fprio_t *fprio){
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

/*
SAI (T, H, B):

retira H do conjunto de heróis presentes em B
escolhe uma base destino D aleatória
cria e insere na LEF o evento VIAJA (agora, H, D)
cria e insere na LEF o evento AVISA (agora, B)
*/
void sai(int tempo, struct heroi *h, struct base *b, struct fprio_t *fprio){
    cjto_retira(b->presentes, h->id);
    h->base_destino = rand() % N_BASES;
    CriaInsere(tempo, TIPO_VIAJA, h, b, fprio);
    CriaInsere(tempo, TIPO_AVISA, b, NULL, fprio);

    printf("%6d: SAI HEROI %2d BASE %d (%d/%d) \n",
            tempo,
            h->id,
            b->id,
            cjto_card(b->presentes),
            b->lotacao);
}

/*
VIAJA (T, H, D):

calcula duração da viagem:
    distância = distância cartesiana entre a base atual de H e a base D 
    duração = distância / velocidade de H
cria e insere na LEF o evento CHEGA (agora + duração, H, D)
*/
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

}

/*
MORRE (T, H, B):

retira H do conjunto de heróis presentes em B
muda o status de H para morto 
cria e insere na LEF o evento AVISA (agora, B)
*/
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


