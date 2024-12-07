#ifndef EVENTOS_H
#define EVENTOS_H

#include "fprio.h"
#include "lista.h"
#include "entidades.h"
#include "mundo.h"
#include "complementos.h"

#define TIPO_CHEGA 1
#define TIPO_ESPERA 2
#define TIPO_DESISTE 3
#define TIPO_AVISA 4
#define TIPO_ENTRA 5
#define TIPO_SAI 6
#define TIPO_VIAJA 7
#define TIPO_MISSAO 8
#define TIPO_FIM 9
#define TIPO_MORRE 10


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
void chega(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);

/*
ESPERA (T, H, B):

adiciona H ao fim da fila de espera de B
cria e insere na LEF o evento AVISA (agora, B)
*/
void espera(int tempo, struct heroi *heroi, struct base *base, struct mundo *mundo, struct fprio_t *fprio);
/*
DESISTE (T, H, B):

escolhe uma base destino D aleatória
cria e insere na LEF o evento VIAJA (agora, H, D)
*/
void desiste(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);
/*
AVISA (T, B):

enquanto houver vaga em B e houver heróis esperando na fila:
    retira primeiro herói (H') da fila de B
    adiciona H' ao conjunto de heróis presentes em B
    cria e insere na LEF o evento ENTRA (agora, H', B)
*/
void avisa(int tempo, struct base *b, struct mundo *mundo, struct fprio_t *fprio);

/*
ENTRA (T, H, B):

calcula TPB = tempo de permanência na base:
    TPB = 15 + paciência de H * aleatório [1...20]
cria e insere na LEF o evento SAI (agora + TPB, H, B)
*/
void entra(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);

/*
SAI (T, H, B):

retira H do conjunto de heróis presentes em B
escolhe uma base destino D aleatória
cria e insere na LEF o evento VIAJA (agora, H, D)
cria e insere na LEF o evento AVISA (agora, B)
*/
void sai(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);
/*
VIAJA (T, H, D):

calcula duração da viagem:
    distância = distância cartesiana entre a base atual de H e a base D 
    duração = distância / velocidade de H
cria e insere na LEF o evento CHEGA (agora + duração, H, D)
*/
void viaja(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);
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
void missao(int tempo, struct missao *m, struct mundo *mundo, struct fprio_t *fprio);
/*
MORRE (T, H, B):

retira H do conjunto de heróis presentes em B
muda o status de H para morto 
cria e insere na LEF o evento AVISA (agora, B)
*/
void morre(int tempo, struct heroi *h, struct base *b, struct mundo *mundo, struct fprio_t *fprio);
/*
FIM (T):
  encerra a simulação
  apresenta estatísticas dos heróis
  apresenta estatísticas das bases
  apresenta estatísticas das missões
*/
void fim(struct mundo *mundo);


#endif