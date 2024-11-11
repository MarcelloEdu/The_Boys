#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

//cria uma lista vazia
struct lista_t *lista_cria (){
    struct lista_t *lst = (struct lista_t *) malloc(sizeof(struct lista_t));

    if (lst == NULL){
        return NULL;
    }

    lst->prim = NULL;
    lst->ult = NULL;
    
    lst->tamanho = 0;
    return lst;
}

//remove todos os itens da lista e libera a memória
struct lista_t *lista_destroi (struct lista_t *lst){
    struct item_t *aux = lst->prim;
    struct item_t *aux2;

    while (aux != NULL){
        aux2 = aux->prox;                                   //salva o próximo item
        free(aux);
        aux = aux2;                                         //avança para o próximo
    }

    free(lst);
    return NULL;
}

//insere o item na lista na posição indicada
int lista_insere(struct lista_t *lst, int item, int pos){

    struct item_t *aux = lst->prim;                         //auxiliar para percorrer a lista

    //Alocação usando casting para evitar warning
    struct item_t *novo = (struct item_t *) malloc(sizeof(struct item_t));
    if (novo == NULL) {                                     //erro de alocação
        return -1;
    }

    novo->valor = item;

    if (pos < 0 || pos > lst->tamanho) {
        pos = lst->tamanho;                                 //insere no fim
    }

    //Insere no início
    if (pos == 0) {
        novo->prox = lst->prim;
        novo->ant = NULL;
        if (lst->prim != NULL){
            lst->prim->ant = novo;
        }

        lst->prim = novo;
        if (lst->tamanho == 0){
            lst->ult = novo;                                //Se a lista estava vazia, o novo é o último também
        }
    } 
    else {                                                  //Insere no meio ou no fim
        for (int i = 0; i < pos - 1; i++) {
            aux = aux->prox;
        }
        novo->prox = aux->prox;
        novo->ant = aux;
        aux->prox = novo;
        if (novo->prox != NULL) {
            novo->prox->ant = novo;
        } else{
            lst->ult = novo;                                //Atualiza o último elemento se inseriu no fim
        }
    }
    lst->tamanho++;                                         //Incrementa o tamanho da lista
    return lst->tamanho;
}

int lista_retira(struct lista_t *lst, int *item, int pos)
{

    struct item_t *aux = lst->prim;
    struct item_t *anterior = NULL;
    *item = aux->valor;


    if(lst == NULL || item == NULL || lst->tamanho == 0){   //verifica a validade da lista
        return -1; 
    }

    if(pos < 0 || pos >= lst->tamanho){                     //verifica a validade da posição
        pos = lst->tamanho - 1;
    }

    for(int i = 0; i < pos; i++){                           //percorre a lista até a posição desejada
        anterior = aux; 
        aux = aux->prox; 
    }

    if(anterior == NULL){                                   //se for o primeiro item...
        lst->prim = aux->prox;
    }
    else{   //se não...
        anterior->prox = aux->prox;
    }

    if(aux == lst->ult){                                    //se for o último item...
        lst->ult = anterior;
    }

    free(aux);
    lst->tamanho--;                                         //decrementa o tamanho da lista para refletir a retirada
    return lst->tamanho;
}


//informa o valor do item na posição indicada, sem retirá-lo
int lista_consulta (struct lista_t *lst, int *item, int pos)
{
    if (lst == NULL || pos < 0 || pos >= lst->tamanho){     //verifica a validade da lista e da posição
        pos = -1;
    }
    
    if(pos == -1){                                          //verifica se a posição foi setada como -1 para retornar "não encontrado"
        return -1;
    }

    struct item_t *aux = lst->prim;

    for (int i = 0; i < pos; i++){                          //percorre a lista até a posição desejada
        aux = aux->prox;
    }

    *item = aux->valor;                                     //atribui o valor do item à variável item

    return 0;
}

//nforma a posição da 1ª ocorrência do valor informado na lista
int lista_procura (struct lista_t *lst, int valor){
    struct item_t *aux = lst->prim;                         
    int i = 0;

    if(lst == NULL){                                        //verifica a validade da lista
        return -1;
    }

    while (aux != NULL){
        if (aux->valor == valor){                           //encontrou o valor
            return i;
        }
        aux = aux->prox;                                    //passa pro proximo item
        i++;                                                //incrementa pra continuar o while
    }
    return -1;
}

//retorna o tamanho da lista
int lista_tamanho (struct lista_t *lst){
    if(lst == NULL){
        return -1;
    }
    return lst->tamanho;
}

//imprime o conteudo da lista
void lista_imprime (struct lista_t *lst){
    struct item_t *aux = lst->prim;                         //auxiliar para percorrer a lista

    while (aux != NULL){
        printf("%d ", aux->valor);                          //imprime o valor do item
        aux = aux->prox;                                    //avança pro proximo item
    }
}