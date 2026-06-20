#ifndef AHORCADO_H
#define AHORCADO_H

#define MAX_PALABRA 50
#define MAX_INTENTOS 6
#define MAX_PALABRAS 100

// lee el archivo de palabras y devuelve cuantas palabras encontro
int cargar_palabras(const char *nombre_archivo, char palabras[MAX_PALABRAS][MAX_PALABRA]);

// escoge una palabra al azar de la lista
void elegir_palabra(char palabras[MAX_PALABRAS][MAX_PALABRA], int total, char *palabra_elegida);

// dibuja el muñequito del ahorcado segun los errores que lleva
void dibujar_ahorcado(int errores);

// muestra la palabra con guiones bajos en las letras que faltan
void mostrar_progreso(const char *palabra, int *adivinadas, int largo);

// revisa si una letra ya fue usada antes
int letra_ya_usada(char letra, char *usadas, int cantidad_usadas);

// el juego completo de una partida
void jugar_partida(char palabras[MAX_PALABRAS][MAX_PALABRA], int total);

#endif