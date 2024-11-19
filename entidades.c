
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entidades.h"
#include "conjunto.h"

//função pra aleatorizar
int aleat(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

struct heroi inicia_heroi(struct heroi *heroi, int id)
{
    heroi->id = id;                                                                                                   
    heroi->experiencia = 0;                                                                                           
    heroi->paciencia = aleat(0, 100);                                                                                 
    heroi->velocidade = aleat(50, 5000);                                                                              
    heroi->base_atual = -1;                                                                                           
    int i;                                                                                                            
    int numero_habilidades = aleat(1, 3);                                                                             
    heroi->habilidades = cria_cjt(numero_habilidades);                                                                
                                                                                                                      
    for (i = 0; i < numero_habilidades; ++i)                                                                          
    {                                                                                                                 
        int habilidade = aleat(0, N_HABILIDADES - 1);                                                                 
        insere_cjt(heroi->habilidades, habilidade);                                                                   
    }                                                                                                                 
                                                                                                                      
    return *heroi;                                                                                                    
}

struct base inicia_base(struct base *base, int id)
{
    base->id = id;
    base->lotacao = aleat(3, 10);
    base->presentes = cria_cjt(base->lotacao);
    base->espera = fila_cria();

    base->coord = malloc(sizeof(struct coordenadas));
    base->coord->x = aleat(0, TAMANHO_MUNDO -1);                                                                      
    base->coord->y = aleat(0, TAMANHO_MUNDO -1);                                                                      
    return *base;                                                                                                     
}

void inicia_missao(struct missao *missao, int id) {                                                                   
                                                                                                                      
    missao->coordenadas = malloc(sizeof(struct coordenadas));                                                         
                                                                                                                      
    missao->id = id;                                                                                                  
    missao->coordenadas->x = aleat(0, TAMANHO_MUNDO - 1);                                                             
    missao->coordenadas->y = aleat(0, TAMANHO_MUNDO - 1);                                                             
    int i;                                                                                                            
    int numero_habilidades = aleat(6, 10);                                                                            
    missao->habilidades = cria_cjt(numero_habilidades);                                                               

    for (i = 0; i < numero_habilidades; ++i) 
    {
        int habilidade = aleat(0, N_HABILIDADES - 1);
        insere_cjt(missao->habilidades, habilidade);
    }
}
