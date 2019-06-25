#include "nave.h"

struct nave {
    float x;
    float y;
    float vx;
    float vy;
    float angulo;
    float potencia;
};

nave_t *nave_crear(float x, float y, float angulo)
{
    nave_t *n = malloc(sizeof(nave_t));
    if(n == NULL)
        return NULL;

    n->x = x;
    n->y = y;

    n->vx = NAVE_VX_INICIAL;
    n->vy = NAVE_VY_INICIAL;

    n->angulo = angulo;
    n->potencia = NAVE_POTENCIA_INICIAL;

    return n;
}

void nave_destruir(void *n)
{
    free((nave_t*)n);
}

float nave_get_x(const nave_t *n)
{
    return n->x;
}

float nave_get_y(const nave_t *n)
{
    return n->y;
}

float nave_get_angulo(const nave_t *n)
{
    return n->angulo;
}

void nave_incrementar_angulo(nave_t *n, float angulo)
{
    n->angulo += angulo;

    if(n->angulo >= 2*PI)
        n->angulo = 0;
    else if(n->angulo <= 0)
        n->angulo = 2*PI;
}

void nave_impulso(nave_t *n)
{
    n->potencia += NAVE_POTENCIA_IMPULSO;
}

void nave_mover(nave_t *n, float dt)
{
    n->vx = computar_velocidad(n->vx, n->potencia*cos(n->angulo), dt);
    n->vy = computar_velocidad(n->vy, n->potencia*sin(n->angulo), dt);

    n->vx *= 1 - NAVE_VELOCIDAD_DECREC;
    n->vy *= 1 - NAVE_VELOCIDAD_DECREC;

    n->x = computar_posicion(n->x, n->vx, dt);
    n->y = computar_posicion(n->y, n->vy, dt);

    graficador_ajustar_variables(&(n->x), &(n->y));

    n->potencia *= 1 - NAVE_POTENCIA_DECREC;
}

bool nave_dibujar(const nave_t *n)
{
    bool dibuja_nave = graficador_dibujar(NAVE_SPRITE, NAVE_ESCALA, n->x, n->y, n->angulo);

    bool dibuja_chorro;
    if(n->potencia > 1)
        dibuja_chorro = graficador_dibujar(CHORRO_SPRITE, NAVE_ESCALA, n->x, n->y, n->angulo);
    else dibuja_chorro = true;

    return dibuja_nave && dibuja_chorro;
}
