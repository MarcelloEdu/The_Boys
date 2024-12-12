// programa principal do projeto "The Boys - 2024/2"
// Autor: Marcello Eduardo, GRR 20244350

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "fila.h"
#include "eventos.h"
#include "entidades.h"
#include "mundo.h"
#include "complementos.h"

int main() {
    struct mundo mundo = {0};
    struct evento_t *ev;
    inicia_mundo(&mundo);

    while (mundo.relogio < T_FIM) {
        int tipo_evento, prio_evento;
        ev = fprio_retira(mundo.eventos, &tipo_evento, &prio_evento);

        mundo.relogio = ev->tempo;

        if (!ev)
            break;  // Se o evento for NULL, sai do loop

        switch (ev->tipo) {
            case TIPO_CHEGA:
                chega(mundo.relogio, &mundo.herois[ev->dado1], &mundo.bases[ev->dado2], &mundo, mundo.eventos);
                break;

            case TIPO_ESPERA:
                espera(mundo.relogio, &mundo.herois[ev->dado1], &mundo.bases[ev->dado2], &mundo, mundo.eventos);
                break;

            case TIPO_DESISTE:
                desiste(mundo.relogio, &mundo.herois[ev->dado1], &mundo.bases[ev->dado2], &mundo, mundo.eventos);
                break;

            case TIPO_AVISA:
                avisa(mundo.relogio, &mundo.bases[ev->dado1], &mundo, mundo.eventos);
                break;

            case TIPO_ENTRA:
                entra(mundo.relogio, &mundo.herois[ev->dado1], &mundo.bases[ev->dado2], &mundo, mundo.eventos);
                break;

            case TIPO_SAI:
                sai(mundo.relogio, &mundo.herois[ev->dado1], &mundo.bases[ev->dado2], &mundo, mundo.eventos);
                break;

            case TIPO_VIAJA:
                viaja(mundo.relogio, &mundo.herois[ev->dado1], &mundo.bases[ev->dado2], &mundo, mundo.eventos);
                break;

            case TIPO_MISSAO:
                missao(mundo.relogio, &mundo.missoes[ev->dado1], &mundo, mundo.eventos);
                break;

            case TIPO_MORRE:
                morre(mundo.relogio, &mundo.herois[ev->dado1], &mundo.bases[ev->dado2], &mundo, mundo.eventos);
                break;
                
            case TIPO_FIM:
                fim(&mundo);
                break;

            default:
                fprintf(stderr, "Evento desconhecido: %d\n", ev->tipo);
        }

        //desaloca todos os eventos
        destroi_evento(ev);

    }
    return 0;
}
