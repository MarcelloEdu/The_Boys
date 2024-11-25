#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

//basicamente definir uma prioriade pra fila (crescente)
//se tiver a mesma, FiFO


// cria uma nova fila, informando o número máximo de itens
// retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria (){
    struct fprio_t *fprio = (struct fprio_t *) malloc (sizeof(struct fprio_t));

    if (!fprio) {
        return NULL;
    }

    fprio->num = 0;
    fprio->prim = NULL;

    return fprio;
}

// libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f){
    if (!f) return NULL;

    struct fpnodo_t *aux, *prox;
    aux = f->prim;

    while (aux) {
        prox = aux->prox;
        free(aux->item);//faltava isso no leak do tp5
        free(aux);
        aux = prox;
    }
    free(f);
    return NULL;
}

//  insere um novo item na fila, 
//  posicionado de acordo com o valor da prioridade informada
int fprio_insere(struct fprio_t *f, void *item, int tipo, int prio) {
    if (!f || !item) return -1; // verifica se a fila ou o item são nulos

    // verifica se o item já está na fila
    struct fpnodo_t *atual = f->prim;
    while (atual != NULL) {
        if (atual->item == item) {
            return -1; // item já existe na fila
        }
        atual = atual->prox;
    }

    // cria o novo nó
    struct fpnodo_t *novo = (struct fpnodo_t *)malloc(sizeof(struct fpnodo_t));
    if (!novo) return -1; // erro ao alocar memória

    novo->item = item;
    novo->tipo = tipo;
    novo->prio = prio;
    novo->prox = NULL;

    // insere o novo item na posição correta
    if (f->prim == NULL || f->prim->prio > prio) {
        novo->prox = f->prim;
        f->prim = novo;
    } else {
        atual = f->prim;
        while (atual->prox != NULL && atual->prox->prio <= prio) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }

    f->num++;
    return f->num;
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio){
    if(!f || !tipo || !prio) return NULL;
    
    struct fpnodo_t *temp = f->prim;
    void *item = temp->item;
    if (tipo) *tipo = temp->tipo;
    if (prio) *prio = temp->prio;

    f->prim = f->prim->prox;
    free(temp);
    f->num--;

    return item;
}

int fprio_tamanho (struct fprio_t *f){
    if (!f) return -1;
    return f->num;
}

void fprio_imprime (struct fprio_t *f){
    struct fpnodo_t *atual = f->prim;
    while (atual) {
        printf("(%d %d)", atual->tipo, atual->prio);
        if (atual->prox) {
            printf(" "); // espaço entre os itens
        }
        atual = atual->prox;
    }
}
