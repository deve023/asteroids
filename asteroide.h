#ifndef _ASTEROIDE_H_
#define _ASTEROIDE_H_

#include "graficador.h"
#include "config.h"
#include "fisica.h"

#include <math.h>

#include "matematica.h"

typedef struct asteroide asteroide_t;

asteroide_t *asteroide_crear(float x, float y, float radio);
void asteroide_destruir(void * a);
void asteroide_mover(asteroide_t * a, float dt);
bool asteroide_dibujar(const asteroide_t *a);
bool asteroide_colision(const asteroide_t *a, float x, float y);
float asteroide_get_x(const asteroide_t *a);
float asteroide_get_y(const asteroide_t *a);
float asteroide_get_radio(const asteroide_t *a);

#endif // _ASTEROIDE_H_
