#ifndef _DISPARO_H_
#define _DISPARO_H_

#include "graficador.h"
#include "config.h"
#include "fisica.h"

#include <math.h>

#define DISPARO_SPRITE "SHOT"

typedef struct disparo disparo_t;

disparo_t * disparo_crear(float x, float y, float angulo);
void disparo_destruir(void *d);
void disparo_mover(disparo_t * d, float dt);
bool disparo_dibujar(disparo_t * d);
float disparo_get_x(const disparo_t *d);
float disparo_get_y(const disparo_t *d);
float disparo_get_angulo(const disparo_t *d);
float disparo_get_tiempo(const disparo_t *d);

#endif // _DISPARO_H_
