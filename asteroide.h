#ifndef _ASTEROIDE_H_
#define _ASTEROIDE_H_

#include "graficador.h"
#include "config.h"
#include "fisica.h"

#include <math.h>

#include "matematica.h"

typedef struct asteroide asteroide_t;

//Crea un asteroide en la posicion (x,y) de radio r
asteroide_t *asteroide_crear(float x, float y, float r);

//Libera la memoria asociada al asteroide a
void asteroide_destruir(void * a);

//Actualiza las variables de estado del asteroide a segun un intervalo de tiempo dt
void asteroide_mover(asteroide_t * a, float dt);

//Grafica el asteroide a sobre la pantalla
bool asteroide_dibujar(const asteroide_t *a);

//Testea si el par de coordenadas (x,y) colisiona con el asteroide a
bool asteroide_colision(const asteroide_t *a, float x, float y);

//Devuelve la coordenada x del asteroide a
float asteroide_get_x(const asteroide_t *a);

//Devuelve la coordenada y del asteroide a
float asteroide_get_y(const asteroide_t *a);

//Devuelve el radio del asteroide a
float asteroide_get_radio(const asteroide_t *a);

#endif // _ASTEROIDE_H_
