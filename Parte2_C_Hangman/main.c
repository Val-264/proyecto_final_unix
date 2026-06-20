#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ahorcado.h"

int main() {
    srand(time(NULL));

    char palabras[MAX_PALABRAS][MAX_PALABRA];
    int total_palabras = cargar_palabras("palabras.txt", palabras);

    if (total_palabras == 0) {
        printf("No hay palabras cargadas, revisa el archivo palabras.txt\n");
        return 1;
    }

    int opcion;

    do {
        printf("------------------------------------\n");
        printf("         JUEGO DEL AHORCADO\n");
        printf("------------------------------------\n");
        printf("1. Jugar\n");
        printf("2. Salir\n");
        printf("------------------------------------\n");
        printf("Elige una opcion: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            jugar_partida(palabras, total_palabras);
            printf("\n");
        } else if (opcion != 2) {
            printf("Opcion invalida, intenta de nuevo\n");
        }

    } while (opcion != 2);

    printf("¡Gracias por jugar Hasta luego!\n");

    return 0;
}