#ifndef _NAVE_H_
#define _NAVE_H_

#include <math.h>

#include "config.h"
#include "graficador.h"

#define NAVE_SPRITE "SHIP"
#define CHORRO_SPRITE "THURST"

#define ESCALA_NAVE 1

struct nave;
typedef struct nave nave_t;

nave_t *nave_crear();
void nave_destruir(nave_t *n);

float nave_get_pos_x(const nave_t *n);
float nave_get_pos_y(const nave_t *n);

void nave_incrementar_angulo(nave_t *n, float angulo);
void nave_impulso(nave_t *n);

void nave_mover(nave_t *n, float dt);
bool nave_dibujar(const nave_t *n);

#endif