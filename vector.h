#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdbool.h>
#include <SDL2/SDL.h>


/*
Recibe un vector de coordenadas v de n elementos y un angulo rad.
Rota cada una de las coordenadas rad radianes respecto al origen de coordenadas.
*/
void vector_rotar(float ** v, size_t n, double rad);

/*
Recibe un vector de coordenadas v de n elementos y dos numeros dx y dy.
Modifica cada una de las coordenadas del vector mediante la adicion de dx y dy.
*/
void vector_trasladar(float ** v, int n, float dx, float dy);

/*
Recibe un vector dinamico v de n elementos y libera la memoria
asociada al vector.
*/
void vector_destruir(float ** v, size_t n);


#endif // _VECTOR_H_
