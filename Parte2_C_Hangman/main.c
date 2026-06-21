#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ahorcado.h"

void mostrar_bienvenida(int f_inicio, int c_inicio) {
    char titulo_bienvenida[15][47] = {
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

    attron(COLOR_PAIR(1) | A_BOLD);
    for (int f = 0; f < 13; f++)
    {
        for (int c = 0; c < 47; c++)
        {
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

    attroff(COLOR_PAIR(1) | A_BOLD);

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
    
    clear();
    mostrar_bienvenida(2, 10);

    mvprintw(16, 18, "Presiona cualquier tecla para comenzar...");
    refresh();
    getch(); // Espera la acción del usuario

   
    int opcion;

    do {
        clear();
        mvprintw(2, 10,"------------------------------------");
        mvprintw(3, 10,"               MENU");
        mvprintw(4, 10,"------------------------------------");
        mvprintw(5, 10,"1. Jugar");
        mvprintw(6, 10,"2. Salir");
        mvprintw(7, 10,"------------------------------------");
        mvprintw(8, 10,"Elige una opcion: ");

        // Leer la entrada ddel usuario
        echo();
        char entrada[10];
        mvgetnstr(8, 28, entrada, 9); // Leer la opción en la coordenada del prompt
        noecho(); // Lo volvemos a apagar para el juego

        opcion = atoi(entrada); // Convertir la cadena a entero 

        if (opcion == 1) {
            jugar_partida(palabras, total_palabras);
        } else if (opcion != 2) {
            mvprintw(10, 10, "Opcion invalida, intenta de nuevo. Presiona una tecla...");
            refresh();
            getch();
        }

    } while (opcion != 2);

    printf("¡Gracias por jugar Hasta luego!\n");

    endwin();    // Cerrar NCURSES (Regresa la terminal a su estado original) 

    return 0;
}