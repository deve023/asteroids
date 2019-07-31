#include "matematica.h"

#include <stdlib.h>

float calcular_y_recta(float x, float x0, float y0, float x1, float y1)
{
	return (y1-y0)/(x1-x0)*(x-x0)+y0;
}

float randomf(float min, float max)
{
	return (float)rand()/RAND_MAX*(max-min)+min;
}
