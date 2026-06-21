#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "ahorcado.h"

int cargar_palabras(const char *nombre_archivo, char palabras[MAX_PALABRAS][MAX_PALABRA]) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        // Como ncurses podría no estar iniciado aún, usamos un print estándar
        printf("No se pudo abrir el archivo %s\n", nombre_archivo);
        return 0;
    }

    int contador = 0;
    while (contador < MAX_PALABRAS && fgets(palabras[contador], MAX_PALABRA, archivo) != NULL) {
        palabras[contador][strcspn(palabras[contador], "\r\n")] = '\0';
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

// Ahora recibe la fila base para saber dónde dibujarse
void dibujar_ahorcado(int errores, int f_base) {
    mvprintw(f_base + 0, 5, "  +---+");
    mvprintw(f_base + 1, 5, "  |   |");
    mvprintw(f_base + 2, 5, "  %c   |", (errores >= 1 ? 'O' : ' '));

    if (errores >= 4) {
        mvprintw(f_base + 3, 5, " %c%c%c  |", (errores >= 3 ? '/' : ' '), (errores >= 2 ? '|' : ' '), (errores >= 4 ? '\\' : ' '));
    } else if (errores >= 2) {
        mvprintw(f_base + 3, 5, "  %c%c  |", (errores >= 3 ? '/' : ' '), (errores >= 2 ? '|' : ' '));
    } else {
        mvprintw(f_base + 3, 5, "      |");
    }

    if (errores >= 6) {
        mvprintw(f_base + 4, 5, " %c %c  |", '/', '\\');
    } else if (errores >= 5) {
        mvprintw(f_base + 4, 5, " %c    |", '/');
    } else {
        mvprintw(f_base + 4, 5, "      |");
    }

    mvprintw(f_base + 5, 5, "      |");
    mvprintw(f_base + 6, 5, "=========");
    mvprintw(f_base + 8, 5, "Intentos fallados: %d de %d", errores, MAX_INTENTOS);
}

// Imprime el progreso en una coordenada específica (f, c)
void mostrar_progreso(const char *palabra, int *adivinadas, int largo, int f, int c) {
    mvprintw(f, c, "Palabra: ");
    for (int i = 0; i < largo; i++) {
        if (adivinadas[i]) {
            printw("%c ", palabra[i]); // printw añade texto de forma contigua
        } else {
            printw("_ ");
        }
    }
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

    // Mensaje estático superior
    mvprintw(1, 5, "¡Comienza el juego! La palabra tiene %d letras", largo);

    // Cadena para guardar el estado del último intento
    char mensaje_feedback[100] = "¡Buena suerte!";

    while (errores < MAX_INTENTOS && letras_correctas < largo) {
        clear(); // Limpia la pantalla para evitar "fantasmas" visuales
        mvprintw(1, 5, "¡Comienza el juego! La palabra tiene %d letras", largo);
        
        // Dibujamos los componentes fijos en coordenadas ordenadas
        dibujar_ahorcado(errores, 3); // Ocupa desde fila 3 hasta la 11
        mostrar_progreso(palabra, adivinadas, largo, 5, 30); // Fila 5, Columna 30

        // Mostrar letras usadas a la derecha
        mvprintw(7, 30, "Letras usadas: ");
        for (int i = 0; i < cantidad_usadas; i++) {
            printw("%c ", letras_usadas[i]);
        }

        // Mostrar el resultado de la última acción debajo
        mvprintw(13, 5, "%s", mensaje_feedback);

        // Pedir la letra de forma instantánea sin Enter
        mvprintw(15, 5, "Ingresa una letra: ");
        refresh(); // Obligatorio para renderizar los cambios en la terminal

        int ch = getch(); // Captura un único carácter inmediatamente
        char letra = toupper(ch);

        if (!isalpha(letra)) {
            strcpy(mensaje_feedback, "Eso no es una letra valida, intenta de nuevo.");
            continue;
        }

        if (letra_ya_usada(letra, letras_usadas, cantidad_usadas)) {
            strcpy(mensaje_feedback, "Ya intentaste con esa letra, prueba otra.");
            continue;
        }

        letras_usadas[cantidad_usadas] = letra;
        cantidad_usadas++;

        int acerto = 0;
        for (int i = 0; i < largo; i++) {
            // Si tus palabras en el diccionario están en minúsculas, 
            // asegúrate de comparar correspondientemente (ej: toupper(palabra[i]))
            if (toupper(palabra[i]) == letra) {
                adivinadas[i] = 1;
                acerto = 1;
                letras_correctas++;
            }
        }

        if (!acerto) {
            errores++;
            strcpy(mensaje_feedback, "Esa letra NO esta en la palabra. :(");
        } else {
            strcpy(mensaje_feedback, "¡Bien! Esa letra SI esta. :)");
        }
    }

    // --- PANTALLA FINAL ---
    clear();
    dibujar_ahorcado(errores, 3);
    mostrar_progreso(palabra, adivinadas, largo, 5, 30);

    if (letras_correctas == largo) {
        mvprintw(13, 5, "¡Felicidades! GANASTE :) La palabra era: %s", palabra);
    } else {
        mvprintw(13, 5, "Perdiste :( La palabra era: %s", palabra);
    }

    mvprintw(16, 5, "Presiona cualquier tecla para salir...");
    refresh();
    
    getch();     // Espera una última tecla antes de cerrar
}
