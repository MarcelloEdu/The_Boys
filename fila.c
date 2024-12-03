#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct fila *fila_cria ()
{
    /*casting para garantir que o malloc retorne o valor do tamanho de struct *fila*/
    struct fila *f = (struct fila *) malloc(sizeof(struct fila));
    if (f == NULL)
     return NULL;
    
    f->ini = NULL;
    f->fim = NULL;
    f->tamanho = 0;
    return f;   
}

void fila_destroi (struct fila **fila)
{
    struct nodo *aux;
    /*enquanto a fila não estiver vazia*/
    while ((*fila)->ini != NULL)
    {
        aux = (*fila)->ini;
        /*percorre a fila verificando seus valores*/
        (*fila)->ini = (*fila)->ini->prox;
        free(aux);
    }
    free(*fila);
    /* importante pra liberar o ponteiro */
    *fila = NULL;
}

int enqueue (struct fila *fila, int dado)
{
    /*controle de acesso inválido*/
    if (fila == NULL)
        return 0;

    /*aloca memória para novas operações a serem feitas na função*/
    struct nodo *novo = (struct nodo *) malloc(sizeof(struct nodo));
    /*controle de acesso inválido*/
    if (novo == NULL)
        return 0;

    novo->chave = dado;
    novo->prox = NULL; /*indica que ele não tem próximo, pois é o último elemento, seguindo a politica FIFO*/

    if (fila->fim != NULL)
        fila->fim->prox = novo; /*ajustando para que se a fila não estiver vazia o novo elemento seja o último*/
    else
        fila->ini = novo; /*caso contrário, o novo elemento é o primeiro*/

    fila->fim = novo; /*insere elementos no fim da fila*/
    fila->tamanho++;

    return 1;
}

int dequeue (struct fila *fila, int *dado)
{
    struct nodo *aux; /*auxiliar para armazenar o nó a ser removido*/

    if (fila->ini == NULL)
        return 0;

    *dado = fila->ini->chave; /*O valor da chave do primeiro nó é armazenado em *dado*/
    aux = fila->ini;
    fila->ini = fila->ini->prox;
    free(aux);
    fila->tamanho--;

    if (fila->ini == NULL)
        fila->fim = NULL;
    return 1;
}
 
int fila_tamanho (struct fila *fila)
{
    return fila->tamanho;
}

int fila_vazia (struct fila *fila)
{
    return fila->ini == NULL;
}

void fila_imprime (struct fila *fila)
{
    struct nodo *aux = fila->ini;
    printf("[ ");
    while (aux != NULL)
    {
        printf("%d ", aux->chave);
        aux = aux->prox;
    }
    printf("]\n");
}
