#ifndef _DICCIONARIO_H_
#define _DICCIONARIO_H_

#include "caracteres.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

typedef const int (*matriz_t)[2];



/*
Recibe un caracter y un puntero a una variable n.
Devuelve por el nombre la matriz correspondiente al caracter, definida en caracteres.h.
Devuelve por la interfaz a la variable n el numero de coordenadas de la matriz.
*/
matriz_t caracter_a_matriz(char caracter, size_t * n);

/*
Recibe una matriz de n elementos que representa un caracter, una posicion (x,y), un factor de escala y un renderer.
Dibuja en la pantalla el caracter trasladado a (x,y) y dimensionado por escala.
*/
void caracter_graficar(matriz_t matriz, size_t n, float x, float y, float escala, SDL_Renderer * renderer);

/*
Recibe una cadena de caracteres, una posicion (x,y), un factor de escala y un renderer.
Dibuja en la pantalla la cadena trasladada a (x,y) y dimensionada por escala.
*/
void cadena_graficar(char * cadena, float x, float y, float escala, SDL_Renderer * renderer);

/*
Recibe un entero contador de n digitos, una posicion (x,y), un factor de escala y un renderer.
Dibuja en la pantalla el contador trasladado a (x,y) y dimensionado por escala, completando con ceros a la izquierda.
*/
void contador_graficar_ceros(int contador, size_t n, float x, float y, float escala, SDL_Renderer * renderer);

/*
Recibe un entero contador de n digitos, una posicion (x,y), un factor de escala y un renderer.
Dibuja en la pantalla el contador trasladado a (x,y) y dimensionado por escala, alineado a la derecha.
*/
void contador_graficar_derecha(int contador, size_t n, float x, float y, float escala, SDL_Renderer * renderer);

#endif // _DICCIONARIO_H_