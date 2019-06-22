#include "asteroide.h"

struct asteroide {
    float x;
    float y;
    float vx;
    float vy;
    float angulo;
    float radio;
    char * clase;
};



char * asteroide_clases[4] = {
	"ROCK1", "ROCK2", "ROCK3", "ROCK4"
};

float randomf(float min, float max)
{
    return (float)rand()/RAND_MAX*(max-min)+min;
}

asteroide_t *asteroide_crear(float x, float y, float radio)
{
    asteroide_t *a = malloc(sizeof(asteroide_t));
    if(a == NULL)
        return NULL;

    a->x = x;
    a->y = y;

    a->vx = randomf(1000/radio-100, 1000/radio+100);
    a->vy = randomf(1000/radio-100, 1000/radio+100);

    a->radio = radio;
    a->angulo = randomf(-PI/2, PI/2);
	a->clase = asteroide_clases[rand()%4];
    
    return a;
}

void asteroide_destruir(asteroide_t * a)
{
	free(a);
}

void asteroide_mover(asteroide_t * a, float dt)
{
	a->x = computar_posicion(a->x, a->vx, dt);
	a->y = computar_posicion(a->y, a->vy, dt);
	graficador_ajustar_variables(&(a->x), &(a->y));
}

bool asteroide_dibujar(const asteroide_t *a) {
    return graficador_dibujar(a->clase, a->radio, a->x, a->y, a->angulo);
}

bool asteroide_colision(const asteroide_t *a, float x, float y)
{
	return sqrt((x - a->x)*(x - a->x) + (y - a->y)*(y - a->y)) <= a->radio;
}

float asteroide_get_x(const asteroide_t *a) {
    return a->x;
}
float asteroide_get_y(const asteroide_t *a) {
    return a->y;
}

float asteroide_get_radio(const asteroide_t *a) {
    return a->radio;
}


