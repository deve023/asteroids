#include "vector.h"
#include "config.h"

/*
Recibe dos variables p1 y p2 de tipo float **.
Intercambia lo apuntado por p1 con lo apuntado por p2.
*/
static void swap(float ** p1, float ** p2)
{
	float * aux = *p1;
	*p1 = *p2;
	*p2 = aux;
}

/*
Pide memoria dinamica para un vector de tipo float** de n elementos.
*/
static bool vector_pedir_memoria(float *** v, size_t n)
{
	*v = malloc(n * sizeof(float *));
	if(*v == NULL)
		return false;

	for(size_t i = 0; i < n; i++)
	{
		(*v)[i] = malloc(2 * sizeof(float));
		if((*v)[i] == false)
		{
			vector_destruir(*v,i);
			return false;
		}

	}

	return true;
}


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

bool vector_esta_arriba(float ** v, int n, float x, float y)
{
	for(int i=0; i<n-1; i++)
	{
		if(x<=v[i+1][0])
		{
			float x0=v[i][0];
			float y0=v[i][1];
			float x1=v[i+1][0];
			float y1=v[i+1][1];

			float y_recta = (y1-y0)/(x1-x0)*(x-x0)+y0;

			if(y>y_recta) return true;

			break;
		}
	}

	return false;
}

void vector_destruir(float ** v, size_t n)
{
	for(size_t i = 0; i < n; i++)
		free(v[i]);

	free(v);
}

float ** vector_desde_matriz(const float m[][2], size_t n)
{
	float ** v = NULL;

	bool mem_exito = vector_pedir_memoria(&v, n);
	if(mem_exito == false)
		return NULL;

	for(size_t i = 0; i < n; i++)
	{
		memcpy(v[i], m[i], 2 * sizeof(float));
	}

	return v;
}

float ** vector_densificar(float ** v, size_t nv, size_t nn, float margen)
{
	float ** v_nuevo = NULL;

	bool mem_exito = vector_pedir_memoria(&v_nuevo, nn);
	if(mem_exito == false)
		return NULL;

	for(size_t i = 0; i < nn-nv; i++) // Genera puntos aleatorios dentro del margen y los coloca en el vector v_nuevo
	{
		float x = randomf(v[0][0],v[nv-1][0]);

		float y_recta = vector_interpolar(v, nv, x);

		float y = randomf(y_recta-margen, y_recta+margen);

		v_nuevo[i][0] = x;
		v_nuevo[i][1] = y;
	}

	for(size_t i = nn-nv, j = 0; i<nn; i++, j++)
	{
		v_nuevo[i][0]=v[j][0];
		v_nuevo[i][1]=v[j][1];
	}
	vector_ordenar(v_nuevo, nn);

	return v_nuevo;
}

float ** crear_terreno (size_t * n)
{
	*n = 0;

	const float terreno_estatico [][2] =
	{
		{0 , 100}, 							// izquierda
		{ -1 , VENTANA_ALTO * 2.0 / 3} , 	// "medio"
		{ VENTANA_ANCHO , 100} 				// derecha
	};

	size_t nt = 3;
	float ** terreno = vector_desde_matriz(terreno_estatico ,nt);
	if(terreno == NULL)
		return NULL;

	// Asignamos la coordenada del medio aleatoriamente :
	terreno [1][0] = rand() % VENTANA_ANCHO;

	// Iterativamente densificamos 30 veces achicando el margen cada vez :
	for (size_t i = 1; i < 30; i ++)
	{
		float ** aux = vector_densificar(terreno, nt, 2 * (i + 5), 100 / i);
		vector_destruir (terreno, nt);
		if (aux == NULL) return NULL;
		terreno = aux ;
		nt = 2 * (i + 5);
	}

	* n = nt;
	return terreno;
}

void vector_graficar(float ** v, size_t n, float x, float y, float escala, SDL_Renderer * renderer)
{
		for(int i = 0; i < n-1; i++)
		{
			SDL_RenderDrawLine(
				renderer,
				v[i][0] * escala + x,
				-v[i][1] * escala + y,
				v[i+1][0] * escala + x,
				-v[i+1][1] * escala + y

			);
		}
}

void vector_ordenar(float ** v, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		for(size_t j = 0; j < n-1; j++)
		{
			if(v[j][0]>v[j+1][0])
				swap(&v[j],&v[j+1]);
		}
	}
}

float vector_interpolar(float **v, size_t n, float x)
{
	size_t i;
	for(i = 0; i < n-1; i++)
	{
		if(x <= v[i+1][0])
			break;
	}

	return calcular_y_recta(x, v[i][0], v[i][1], v[i+1][0], v[i+1][1]);
}
