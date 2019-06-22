#include "graficador.h"
#include "config.h"
#include "fisica.h"

#include <math.h>


typedef struct asteroide asteroide_t;

asteroide_t *asteroide_crear(float x, float y, float radio);
void asteroide_destruir(asteroide_t * a);
void asteroide_mover(asteroide_t * a, float dt);
bool asteroide_dibujar(const asteroide_t *a);
bool asteroide_colision(const asteroide_t *a, float x, float y);