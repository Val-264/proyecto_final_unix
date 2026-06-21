#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ahorcado.h"

void mostrar_bienvenida(int f_inicio, int c_inicio) {
    char titulo_bienvenida[13][47] = {
    {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
    {'.','#','.','.','.','#','.','.','#','#','#','.','.','#','.','.','.','#','.','.','#','#','#','#','.','#','.','.','.','#','.','.','#','#','#','.','.','#','.','.','.','#'},
    {'.','#','.','.','.','#','.','#','.','.','.','#','.','#','#','.','.','#','.','#','.','.','.','.','.','#','#','.','#','#','.','#','.','.','.','#','.','#','#','.','.','#'},
    {'.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','#','.','#','.','#','.','.','.','.','.','#','.','#','.','#','.','#','.','.','.','#','.','#','.','#','.','#'},
    {'.','#','#','#','#','#','.','#','#','#','#','#','.','#','.','.','#','#','.','#','.','.','#','#','.','#','.','.','.','#','.','#','#','#','#','#','.','#','.','.','#','#'},
    {'.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#'},
    {'.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#'},
    {'.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#','.','.','#','#','#','#','.','#','.','.','.','#','.','#','.','.','.','#','.','#','.','.','.','#'},
    {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'}


    };

    attron(COLOR_PAIR(3) | A_BOLD);
    for (int f = 0; f < 13; f++)
    {
        for (int c = 0; c < 47; c++)
        {
            if (f > 2 && f < 10) {
                napms(10);
                refresh();
            }
            switch (titulo_bienvenida[f][c])
            {
            case '#': 
                mvaddch(f_inicio + f, c_inicio + (c * 2), ACS_CKBOARD);
                mvaddch(f_inicio + f, c_inicio + (c * 2) + 1, ACS_CKBOARD);
                break;
            case '.':
                mvaddch(f_inicio + f, c_inicio + (c * 2), ' ');
                mvaddch(f_inicio + f, c_inicio + (c * 2) + 1, ' ');
                break;
            }
        }
    }

    attroff(COLOR_PAIR(3) | A_BOLD);

}

int main() {
    srand(time(NULL));

    char palabras[MAX_PALABRAS][MAX_PALABRA];
    int total_palabras = cargar_palabras("palabras.txt", palabras);

    if (total_palabras == 0) {
        printf("No hay palabras cargadas, revisa el archivo palabras.txt\n");
        return 1;
    }

    // --- INICIALIZAR NCURSES ---
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Definir colores
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    
    clear();
    mostrar_bienvenida(2, 10);

    mvprintw(16, 18, "Presiona cualquier tecla para comenzar...");
    refresh();
    getch(); // Espera la acción del usuario

   
    int opcion_seleccionada = 0; // 0 = Jugar, 1 = Agregar, 2 = Salir
    const int num_opciones = 3;
    char *opciones[] = {
        "Jugar",
        "Agregar nueva palabra al juego",
        "Salir"
    };
    int tecla;
    int salir_juego = 0;

    do {
        while(1) {
            clear();
            attron(COLOR_PAIR(4) | A_BOLD);
            mvprintw(2, 10, "------------------------------------");
            mvprintw(3, 10, "               MENU                 ");
            mvprintw(4, 10, "------------------------------------");
            attroff(COLOR_PAIR(4) | A_BOLD);

            // Imprimir las opciones iterando sobre el arreglo
            for(int i = 0; i < num_opciones; i++) {
                if(i == opcion_seleccionada) {
                    attron(A_REVERSE); // Encender el resaltado (fondo blanco, texto negro)
                }
                
                // Imprimir la opción con un espacio extra para que el bloque resaltado se vea bonito
                mvprintw(5 + i, 10, " %s ", opciones[i]);
                
                if(i == opcion_seleccionada) {
                    attroff(A_REVERSE); // Apagar el resaltado para la siguiente línea
                }
            }

            mvprintw(9, 10, "------------------------------------");
            mvprintw(10, 10, "Usa las flechas Arriba/Abajo y presiona Enter");
            refresh();

            // Capturar la tecla presionada
            tecla = getch();

            if (tecla == KEY_UP) {
                opcion_seleccionada--;
                if (opcion_seleccionada < 0) {
                    opcion_seleccionada = num_opciones - 1; // Si se sube desde el tope, bajar al final
                }
            } else if (tecla == KEY_DOWN) {
                opcion_seleccionada++;
                if (opcion_seleccionada >= num_opciones) {
                    opcion_seleccionada = 0; // Si se baja desde el final, subir al inicio
                }
            } else if (tecla == '\n' || tecla == KEY_ENTER || tecla == 10) { 
                // Salir del bucle interno si se presionó enter 
                break; 
            }
        }

        // --- Lógica de la opción seleccionada (se ejecuta al dar Enter) ---
        if (opcion_seleccionada == 0) {
            jugar_partida(palabras, total_palabras);
        } else if (opcion_seleccionada == 1) {
            agregar_palabra("palabras.txt");
            total_palabras = cargar_palabras("palabras.txt", palabras);
        } else if (opcion_seleccionada == 2) {
            salir_juego = 1;
        } 

    } while (!salir_juego);

    printf("¡Gracias por jugar Hasta luego!\n");

    endwin();    // Cerrar NCURSES (Regresa la terminal a su estado original) 

    return 0;
}