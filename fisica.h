#ifndef _FISICA_H_
#define _FISICA_H_

#include <math.h>

/*
Recibe la velocidad actual vi, la aceleracion a y el paso temporal dt.
Devuelve la velocidad del proximo instante.
*/
double computar_velocidad(double vi, double a, double dt);

/*
Recibe la posicion actual pi, la velocidad actual vi y el paso temporal dt.
Devuelve la posicion del proximo instante.
*/
double computar_posicion(double pi, double vi, double dt);

#endif // _FISICA_H_