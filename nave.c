#include "nave.h"

struct nave {
    float x;
    float y;
    float vx;
    float vy;
    float angulo;
    float potencia;
};

nave_t *nave_crear() {
    nave_t *n = malloc(sizeof(nave_t));
    if(n == NULL)
        return NULL;

    n->x = NAVE_X_INICIAL;
    n->y = NAVE_Y_INICIAL;

    n->vx = NAVE_VX_INICIAL;
    n->vy = NAVE_VY_INICIAL;

    n->angulo = NAVE_ANGULO_INICIAL;
    n->potencia = NAVE_POTENCIA_INICIAL;

    return n;
}

void nave_destruir(nave_t *n) {
    free(n);
}

float nave_get_x(const nave_t *n) {
    return n->x;
}
float nave_get_y(const nave_t *n) {
    return n->y;
}

float nave_get_angulo(const nave_t *n) {
    return n->angulo;
}

void nave_incrementar_angulo(nave_t *n, float angulo) {
    n->angulo += angulo;
}

void nave_impulso(nave_t *n) {
    n->potencia += NAVE_POTENCIA_IMPULSO;
}

void nave_mover(nave_t *n, float dt) {

    n->vx = computar_velocidad(n->vx, n->potencia*cos(n->angulo), dt);
    n->vy = computar_velocidad(n->vy, n->potencia*sin(n->angulo), dt);

    n->vx *= 1 - NAVE_VELOCIDAD_DECREC;
    n->vy *= 1 - NAVE_VELOCIDAD_DECREC;

    n->x = computar_posicion(n->x, n->vx, dt);
    n->y = computar_posicion(n->y, n->vy, dt);

    graficador_ajustar_variables(&(n->x), &(n->y));

    n->potencia *= 1 - NAVE_POTENCIA_DECREC;
}

bool nave_dibujar(const nave_t *n) {
    bool dibuja_nave = graficador_dibujar(NAVE_SPRITE, ESCALA_NAVE, n->x, n->y, n->angulo);

    bool dibuja_chorro; 
    if(n->potencia > 1)
        dibuja_chorro = graficador_dibujar(CHORRO_SPRITE, ESCALA_NAVE, n->x, n->y, n->angulo); 
    else dibuja_chorro = true;

    return dibuja_nave && dibuja_chorro;
}
