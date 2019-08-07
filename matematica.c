#include "matematica.h"

#include <stdlib.h>

float randomf(float min, float max)
{
	return (float)rand()/RAND_MAX*(max-min)+min;
}
