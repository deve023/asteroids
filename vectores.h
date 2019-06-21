#ifndef _VECTORES_H_
#define _VECTORES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

float **vector_reservar_memoria(size_t n, size_t m);
float **vector_desde_matriz(const float m[][2], size_t n);
void vector_destruir(float **v, size_t n);

float **vector_densificar(float **v, size_t nv, size_t nn, float margen);
bool vector_esta_arriba(float **v, size_t n, float x, float y);
void vector_trasladar(float **v, size_t n, float dx, float dy);
void vector_rotar(float **v, size_t n, double rad);
void vector_escalar_factor(float **v, size_t n, double f);
void vector_ordenar(float **v, size_t n);

float coord_y_en_recta(float **v, size_t n, float x);
#endif