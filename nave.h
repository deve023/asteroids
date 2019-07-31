#ifndef _NAVE_H_
#define _NAVE_H_

#include <stdbool.h>

#define NAVE_SPRITE "SHIP"
#define CHORRO_SPRITE "THURST"

#define NAVE_ESCALA 1

typedef struct nave nave_t;

//Crea una nave en la posicion (x,y) con angulo a
nave_t *nave_crear(float x, float y, float a);

//Libera la memoria asociada a la nave n
void nave_destruir(void *n);

//Actualiza las variables de estado de la nave n segun un intervalo de tiempo dt
void nave_mover(nave_t *n, float dt);

//Grafica la nave n en la pantalla
bool nave_dibujar(const nave_t *n);

//Incrementa angulo radianes el angulo de la nave n
void nave_incrementar_angulo(nave_t *n, float angulo);

//Incrementa la potencia de la nave n
void nave_impulso(nave_t *n);

//Devuelve la coordenada x de la nave n
float nave_get_x(const nave_t *n);

//Devuelve la coordenada y de la nave n
float nave_get_y(const nave_t *n);

//Devuelve el angulo de la nave n
float nave_get_angulo(const nave_t *n);





#endif // _NAVE_H_
