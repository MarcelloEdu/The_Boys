#ifndef ENTIDADES_H
#define ENTIDADES_H


#define T_INICIO 0
#define T_FIM 525600
#define N_HABILIDADES 10
#define TAMANHO_MUNDO 20000
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM / 100)

struct coordenadas
{
    int x;
    int y;
};

struct base
{
    int id;
    int lotacao;
    int missoes_cumpridas;
    struct cjto_t *habilidades;
    struct cjto_t *presentes;
    struct fila *espera;
    struct coordenadas *coord;
};


struct heroi 
{
    int id;
    int paciencia;
    int velocidade;
    int experiencia;
    struct base *base_atual;
    struct base *base_destino;
    struct cjto_t *habilidades;
    int status; //0 = vivo, 1 = morto
};

struct missao 
{
    int id;
    struct cjto_t *habilidades;
    struct coordenadas *coordenadas;
    int tentativas;
    int tentativas_max;
    int tentativas_min;
    int perigo;
};

struct heroi inicia_heroi(struct heroi *heroi, int id);

struct base inicia_base(struct base *b, int id);

void inicia_missao(struct missao *missao, int id);

#endif