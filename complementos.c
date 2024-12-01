#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "complementos.h"


#define RUN 32

void merge(int vetor[], int inicio, int meio, int fim) {
    int tam_esq = meio - inicio + 1; //calcula o tam do subvetor esquerdo
    int tam_dir = fim - meio; //calcula o tam do subvetor direito

    //Aloca memória para os subvetores esquerdo e direito
    int *esq = (int *)malloc(tam_esq * sizeof(int));
    int *dir = (int *)malloc(tam_dir * sizeof(int));
    if (esq == NULL || dir == NULL) {
        printf("Falha ao alocar memória para os subvetores\n");
        exit(1);
    }

    //copia os elementos para os subvetores esquerdo e direito
    for (int i = 0; i < tam_esq; i++)
        esq[i] = vetor[inicio + i];
    for (int j = 0; j < tam_dir; j++)
        dir[j] = vetor[meio + 1 + j];

    int i = 0;
    int j = 0;
    int k = inicio;

    //Mescla os subvetores ordenados
    while (i < tam_esq && j < tam_dir) {
        if (esq[i] <= dir[j]) {
            vetor[k] = esq[i];
            i++;
        } else {
            vetor[k] = dir[j];
            j++;
        }
        k++;
    }

    //copia os elementos restantes do subvetor esquerdo, se houver
    while (i < tam_esq) {
        vetor[k] = esq[i];
        i++;
        k++;
    }

    //copia os elementos restantes do subvetor direito, se houver
    while (j < tam_dir) {
        vetor[k] = dir[j];
        j++;
        k++;
    }

    //Libera a memória alocada para os subvetores
    free(esq);
    free(dir);
}

int busca(int vetor[], int tam, int valor) {
    int inicio = 0;
    int fim = tam - 1; 
    
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        
        if (vetor[meio] == valor) {
            return meio;
        }
        
        if (vetor[meio] < valor) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return inicio; // Ponto de inserção
}

int inserir(int vetor[], int inicio, int fim) {
    int posicao = busca(vetor, inicio + fim, vetor[fim]);
    int i = fim;

    while (i > posicao) {
        trocar (&vetor[i], &vetor[i - 1]);
        i -= 1;
    }
    return i - inicio;
}

void insertionSort(int vetor[], int tam) {
    for (int i = 1; i < tam; i++) {
        inserir(vetor, 0, i);
    }
}

//TimSort
//função hibrida que combina mergeSort e insertionSort
//divide o vetor em runs e ordena cada run com insertionSort
//mescla os runs adjacentes
//custo = n*log(n)
//desnecessário para esse trabalho pois os vetores não serão muito grandes
//porém pensando se for usada para uma simulação maior ou em outro contexto
//pode ser útil
int timsort(int vetor[], int tam){

    // Ordena cada run de tamanho RUN usando insertionSort
    for (int i = 0; i < tam; i += RUN) {
        int fim = i + RUN - 1;
        if (fim >= tam) {
            fim = tam - 1;
        }
        insertionSort(vetor + i, fim - i + 1); // Passa o subarray e o tamanho
    }

    // Mescla os runs adjacentes
    for (int tamAtual = RUN; tamAtual < tam; tamAtual *= 2) {
        for (int esquerda = 0; esquerda < tam; esquerda += 2 * tamAtual) {
            int meio = esquerda + tamAtual - 1;
            int direita = esquerda + 2 * tamAtual - 1;

            if (meio >= tam) {
                break;
            }
            if (direita >= tam) {
                direita = tam - 1;
            }

            merge(vetor, esquerda, meio, direita);
        }
    }
}

// Função para calcular a distância euclidiana entre duas coordenadas
// Utilizada para calcular a distância entre as bases e as missões
// será util para selecionar a base mais próxima da missão
// pois a base mais próxima é a que tem a menor distância euclidiana
// que será calculada com essa função
// e ordenada com o timsort
int distancia_euclidiana(struct coordenadas *coord1, struct coordenadas *coord2){
    int x = coord1->x - coord2->x;
    int y = coord1->y - coord2->y;

    return sqrt(x*x + y*y);
}

//como em todas as funções de eventos é necessário criar um evento e inserir na fila de prioridade
//essa função foi criada para facilitar a leitura do código e manter ele mais limpo
void CriaInsere(int tempo, int tipo, void* dado1, void* dado2, struct fprio_t *fprio){
    struct evento_t *evento = cria_evento(tempo, tipo, dado1, dado2);
    fprio_insere(fprio, evento, tipo, tempo);
}

