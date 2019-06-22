#include "nave.h"

struct nave {
    float posicion[2];
    float velocidad[2];
    float angulo;
    float potencia;
};

nave_t *nave_crear() {
    nave_t *n = malloc(sizeof(nave_t));
    if(n == NULL)
        return NULL;

    (n->posicion)[0] = NAVE_X_INICIAL;
    (n->posicion)[1] = NAVE_Y_INICIAL;

    (n->velocidad)[0] = NAVE_VX_INICIAL;
    (n->velocidad)[1] = NAVE_VY_INICIAL;

    n->angulo = NAVE_ANGULO_INICIAL;
    n->potencia = NAVE_POTENCIA_INICIAL;

    return n;
}

void nave_destruir(nave_t *n) {
    free(n);
}

float nave_get_pos_x(const nave_t *n) {
    return (n->posicion)[0];
}
float nave_get_pos_y(const nave_t *n) {
    return (n->posicion)[1];
}

void nave_incrementar_angulo(nave_t *n, float angulo) {
    n->angulo += angulo;
}

void nave_impulso(nave_t *n) {
    n->potencia += NAVE_POTENCIA_IMPULSO;
}

void nave_mover(nave_t *n, float dt) {

    (n->velocidad)[0] += n->potencia * cos(n->angulo) * dt;
    (n->velocidad)[1] -= n->potencia * sin(n->angulo) * dt;

    (n->velocidad)[0] -= (n->velocidad)[0] * NAVE_VELOCIDAD_DECREC;
    (n->velocidad)[1] -= (n->velocidad)[1] * NAVE_VELOCIDAD_DECREC;

    (n->posicion)[0] += (n->velocidad)[0] * dt;
    (n->posicion)[1] += (n->velocidad)[1] * dt;
    graficador_ajustar_variables(n->posicion, n->posicion + 1);

    n->potencia *= (1 - NAVE_POTENCIA_DECREC);
}

bool nave_dibujar(const nave_t *n) {
    return graficador_dibujar(NAVE_SPRITE, ESCALA_NAVE, nave_get_pos_x(n), nave_get_pos_y(n), n->angulo) && graficador_dibujar(CHORRO_SPRITE, ESCALA_NAVE, nave_get_pos_x(n), nave_get_pos_y(n), n->angulo);
}
