#include "vector.h"
#include "config.h"
#include "matematica.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void vector_rotar(float ** v, size_t n, double rad)
{
	float x, y;
	double coseno = cos(rad);
	double seno = sin(rad);

	for(int i=0;i<n;i++)
	{
		x=v[i][0];
		y=v[i][1];

		v[i][0]=x*coseno-y*seno;
		v[i][1]=x*seno+y*coseno;
	}
}

void vector_trasladar(float ** v, int n, float dx, float dy)
{
	for(int i=0;i<n;i++)
	{
		v[i][0]+=dx;
		v[i][1]+=dy;
	}
}

void vector_destruir(float ** v, size_t n)
{
	for(size_t i = 0; i < n; i++)
		free(v[i]);

	free(v);
}
