#ifndef _GRAFICADOR_H_
#define _GRAFICADOR_H_

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "lista.h"
#include "vector.h"

bool graficador_inicializar(const char *fn, SDL_Renderer *r);
void graficador_finalizar();

void graficador_ajustar_variables(float *x, float *y);
bool graficador_dibujar(const char *nombre, float escala, float x, float y, float angulo);

#endif // _GRAFICADOR_H_
