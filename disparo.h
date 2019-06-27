#ifndef _DISPARO_H_
#define _DISPARO_H_

#include "graficador.h"
#include "config.h"
#include "fisica.h"

#include <math.h>

#define DISPARO_SPRITE "SHOT"

typedef struct disparo disparo_t;

//Crea un disparo en la posicion (x,y) de angulo a
disparo_t * disparo_crear(float x, float y, float a);

//Libera la memoria asociada al disparo d
void disparo_destruir(void *d);

//Actualiza las variables de estado del disparo d segun un intervalo de tiempo dt
void disparo_mover(disparo_t * d, float dt);

//Grafica el disparo d sobre la pantalla
bool disparo_dibujar(disparo_t * d);

//Devuelve la coordenada x del disparo d
float disparo_get_x(const disparo_t *d);

//Devuelve la coordenada y del disparo d
float disparo_get_y(const disparo_t *d);

//Devuelve el angulo del disparo d
float disparo_get_angulo(const disparo_t *d);

//Devuelve el tiempo de vida del disparo d
float disparo_get_tiempo(const disparo_t *d);

#endif // _DISPARO_H_
