#include "disparo.h"

struct disparo{
	float x;
	float y;
	float vx;
	float vy;
	float angulo;
	float tiempo;
};

disparo_t *disparo_crear(float x, float y, float angulo)
{
	disparo_t * d = malloc(sizeof(disparo_t));
	if(d == NULL)
		return NULL;

	d->x = x;
	d->y = y;
	d->angulo = angulo;

	d->vx = DISPARO_VELOCIDAD * cos(angulo);
	d->vy = -DISPARO_VELOCIDAD * sin(angulo);

	d->tiempo = 0;

	return d;
}

void disparo_destruir(disparo_t * d)
{
	free(d);
}

void disparo_mover(disparo_t * d, float dt)
{
	d->x = computar_posicion(d->x, d->vx, dt);
	d->y = computar_posicion(d->y, d->vy, dt);
	graficador_ajustar_variables(&(d->x), &(d->y));
}

bool disparo_dibujar(disparo_t * d)
{
	return graficador_dibujar("SHOT", 1, d->x, d->y, d->angulo);
}

float disparo_get_x(const disparo_t *d) {
    return d->x;
}
float disparo_get_y(const disparo_t *d) {
    return d->y;
}

float disparo_get_angulo(const disparo_t *d) {
    return d->angulo;
}

