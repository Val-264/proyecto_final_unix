#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "ahorcado.h"

int cargar_palabras(const char *nombre_archivo, char palabras[MAX_PALABRAS][MAX_PALABRA]) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombre_archivo);
        return 0;
    }

    int contador = 0;
    while (contador < MAX_PALABRAS && fgets(palabras[contador], MAX_PALABRA, archivo) != NULL) {
        // quita el salto de linea que deja fgets al final (\n en linux, \r\n en windows)
        palabras[contador][strcspn(palabras[contador], "\r\n")] = '\0';
        // si la linea no quedo vacia, la cuenta como palabra valida
        if (strlen(palabras[contador]) > 0) {
            contador++;
        }
    }

    fclose(archivo);
    return contador;
}

void elegir_palabra(char palabras[MAX_PALABRAS][MAX_PALABRA], int total, char *palabra_elegida) {
    int indice = rand() % total;
    strcpy(palabra_elegida, palabras[indice]);
}

void dibujar_ahorcado(int errores) {
    printf("\n");
    printf("  +---+\n");
    printf("  |   |\n");
    printf("  %c   |\n", errores >= 1 ? 'O' : ' ');

    if (errores >= 4) {
        printf(" %c%c%c  |\n", errores >= 3 ? '/' : ' ', errores >= 2 ? '|' : ' ', errores >= 4 ? '\\' : ' ');
    } else if (errores >= 2) {
        printf("  %c%c  |\n", errores >= 3 ? '/' : ' ', errores >= 2 ? '|' : ' ');
    } else {
        printf("      |\n");
    }

    if (errores >= 6) {
        printf(" %c %c  |\n", '/', '\\');
    } else if (errores >= 5) {
        printf(" %c    |\n", '/');
    } else {
        printf("      |\n");
    }

    printf("      |\n");
    printf("=========\n");
    printf("Intentos fallados: %d de %d\n\n", errores, MAX_INTENTOS);
}

void mostrar_progreso(const char *palabra, int *adivinadas, int largo) {
    printf("Palabra: ");
    for (int i = 0; i < largo; i++) {
        if (adivinadas[i]) {
            printf("%c ", palabra[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

int letra_ya_usada(char letra, char *usadas, int cantidad_usadas) {
    for (int i = 0; i < cantidad_usadas; i++) {
        if (usadas[i] == letra) {
            return 1;
        }
    }
    return 0;
}

void jugar_partida(char palabras[MAX_PALABRAS][MAX_PALABRA], int total) {
    char palabra[MAX_PALABRA];
    elegir_palabra(palabras, total, palabra);

    int largo = strlen(palabra);
    int adivinadas[MAX_PALABRA] = {0};
    char letras_usadas[26];
    int cantidad_usadas = 0;
    int errores = 0;
    int letras_correctas = 0;

    printf("\n¡Empieza el juego! La palabra tiene %d letras.\n", largo);

    while (errores < MAX_INTENTOS && letras_correctas < largo) {
        dibujar_ahorcado(errores);
        mostrar_progreso(palabra, adivinadas, largo);

        printf("Letras usadas: ");
        for (int i = 0; i < cantidad_usadas; i++) {
            printf("%c ", letras_usadas[i]);
        }
        printf("\n");

        printf("Ingresa una letra: ");
        char entrada[10];
        scanf("%9s", entrada);
        char letra = toupper(entrada[0]);

        if (!isalpha(letra)) {
            printf("Eso no es una letra valida, intenta de nuevo.\n");
            continue;
        }

        if (letra_ya_usada(letra, letras_usadas, cantidad_usadas)) {
            printf("Ya intentaste con esa letra, prueba otra.\n");
            continue;
        }

        letras_usadas[cantidad_usadas] = letra;
        cantidad_usadas++;

        int acerto = 0;
        for (int i = 0; i < largo; i++) {
            if (palabra[i] == letra) {
                adivinadas[i] = 1;
                acerto = 1;
                letras_correctas++;
            }
        }

        if (!acerto) {
            errores++;
            printf("Esa letra no esta en la palabra.\n");
        } else {
            printf("¡Bien! Esa letra si esta.\n");
        }
    }

    dibujar_ahorcado(errores);

    if (letras_correctas == largo) {
        printf("¡Felicidades, ganaste! La palabra era: %s\n", palabra);
    } else {
        printf("Perdiste. La palabra era: %s\n", palabra);
    }
}