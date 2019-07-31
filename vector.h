#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdbool.h>
#include <SDL2/SDL.h>


/*
Recibe un vector de coordenadas v de n elementos y un angulo rad.
Rota cada una de las coordenadas rad radianes respecto al origen de coordenadas.
*/
void vector_rotar(float ** v, size_t n, double rad);

/*
Recibe un vector de coordenadas v de n elementos y dos numeros dx y dy.
Modifica cada una de las coordenadas del vector mediante la adicion de dx y dy.
*/
void vector_trasladar(float ** v, int n, float dx, float dy);

/*
Recibe un vector de coordenadas v de n elementos y un punto de coordenadas (x,y).
Devuelve true si el el punto esta arriba de la recta delimitada por las coordenadas de v.
Devuelve false si esa condicion no se cumple.
*/
bool vector_esta_arriba(float ** v, int n, float x, float y);

/*
Recibe un vector dinamico v de n elementos y libera la memoria
asociada al vector.
*/
void vector_destruir(float ** v, size_t n);

/*
Recibe una matriz m de n x 2 elementos y devuelve un vector dinamico
con los mismos elementos. Si falla la asignacion de memoria la funcion devuelve NULL.
*/
float ** vector_desde_matriz(const float m[][2], size_t n);

/*
Recibe un vector v de nv coordenadas en R2.
Devuelve un vector de nn coordenadas con un margen de +-margen con respecto a las rectas definidas por v.
En caso de falla devuelve NULL.
*/
float ** vector_densificar(float ** v, size_t nv, size_t nn, float margen);

/*
Recibe un vector de coordenadas v de n elementos, una posicion (x,y), un factor de escala y un renderer.
Dibuja en la pantalla el vector trasladado a (x,y) y dimensionado por escala.
*/
void vector_graficar(float ** v, size_t n, float x, float y, float escala, SDL_Renderer * renderer);

/*
Recibe un vector v de n coordenadas en R2.
Ordena los elementos del vector en forma creciente de acuerdo a la
coordenada x.
*/
void vector_ordenar(float ** v, size_t n);

/*
Recibe un vector v de n coordenadas en R2 y un valor de la coordenada x.
Devuelve la coordenada y del punto (x,y) que pertenece al segmento de v
correspondiente a x.
*/
float vector_interpolar(float **v, size_t n, float x);

#endif // _VECTOR_H_
