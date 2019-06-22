#include "fisica.h"

double computar_velocidad(double vi, double a, double dt)
{
	return dt*a+vi;
}

double computar_posicion(double pi, double vi, double dt)
{
	return dt*vi+pi;
}