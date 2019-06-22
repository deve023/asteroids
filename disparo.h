#include "graficador.h"
#include "config.h"
#include "fisica.h"

#include <math.h>

typedef struct disparo disparo_t;

disparo_t * disparo_crear(float x, float y, float angulo);
void disparo_destruir(disparo_t * d);
void disparo_mover(disparo_t * d, float dt);
bool disparo_dibujar(disparo_t * d);
float disparo_get_x(const disparo_t *d);
float disparo_get_y(const disparo_t *d);
float disparo_get_angulo(const disparo_t *d);
float disparo_get_tiempo(const disparo_t *d);
