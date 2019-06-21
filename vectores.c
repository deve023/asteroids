#include "vectores.h"

static void swap(float *a, float *b) {
	float aux = *a;
	*a = *b;
	*b = aux;
}

static float rand_float_entre(float a, float b){
	float rand_float = rand() / (float)RAND_MAX;
	float l = b - a;
	float r = rand_float * l;
	return a + r;
}

float **vector_reservar_memoria(size_t n, size_t m) {
	float **v = malloc(sizeof(float *) * n);
	if(v == NULL) return NULL;

	for(size_t i = 0; i < n; i++) {
		v[i] = malloc(sizeof(float) * m);
		if(v[i] == NULL) {
			vector_destruir(v, i);
			return NULL;
		}
	}

	return v;
}

float **vector_desde_matriz(const float m[][2], size_t n){
	float **v = vector_reservar_memoria(n, 2);
	if(v == NULL) return NULL;

	for(size_t i = 0; i < n; i++){
		v[i][0] = m[i][0];
		v[i][1] = m[i][1];
	}

	return v;
}

void vector_destruir(float **v, size_t n){

	for(size_t i = 0; i < n; i++)
		free(v[i]);

	free(v);
}

float **vector_densificar(float **v, size_t nv, size_t nn, float margen){
	float **r = vector_reservar_memoria(nn, 2);
	if(r == NULL) return NULL;

	size_t i;

	for(i = 0; i < nn - nv; i++){
		r[i][0] = rand_float_entre(v[0][0], v[nv-1][0]);

		float y = coord_y_en_recta(v, nv, r[i][0]);
		r[i][1] = rand_float_entre(y - margen, y + margen);
	}

	for(i = 0; i < nv; i++){
		r[nn - nv + i][0] = v[i][0];
		r[nn - nv + i][1] = v[i][1];
	}

	vector_ordenar(r, nn);

	return r;
	
}

bool vector_esta_arriba(float **v, size_t n, float x, float y){
	
	int i;
	float x0, x1, y0, y1, m;

	for(i = 1; x > v[i][0] && i < n; i++); // Busca entre que valores de las coordenadas se encuentra el x ingresado

	/* Entre los puntos que se encuentra, utilizando la ecuacion de una recta, 
	me fijo si la y esta por encima o por debajo de ella */

	x0 = v[i - 1][0];
	x1 = v[i][0];
	y0 = v[i - 1][1];
	y1 = v[i][1];

	m = (y1 - y0)/(x1 - x0); //Pendiente de la recta

	float y_intermedio = m*(x-x0)+y0;

	return y > y_intermedio;
}

void vector_trasladar(float **v, size_t n, float dx, float dy) {

	for(int i = 0; i < n; i++) {
		v[i][0] += dx;
		v[i][1] += dy;
	}
}

void vector_rotar(float **v, size_t n, double rad) {

	float x, y;
	double cosr = cos(rad);
	double sinr = sin(rad);

	for(int i = 0; i < n; i++) {
		x = v[i][0];
		y = v[i][1];
		v[i][0] = x*cosr - y*sinr;
		v[i][1] = x*sinr + y*cosr;
	}
}

void vector_escalar_factor(float **v, size_t n, double f) {

	for(int i = 0; i < n; i++) {
		v[i][0] *= f;
		v[i][1] *= f;
	}
}

void vector_ordenar(float **v, size_t n) {
	for(size_t i = 0; i < n - 1; i++) {
		for(size_t j = i + 1; j < n; j++) {
			if(v[i][0] > v[j][0]) {
				swap(&v[i][0], &v[j][0]);
				swap(&v[i][1], &v[j][1]);
			}
		}
	}
}

float coord_y_en_recta(float **v, size_t n, float x) {
	float y_inter, x0, x1, y0, y1, m;
	int i;

	for(i = 1; x > *(v[i]) && i < n; i++);

	x0 = v[i-1][0];
	x1 = v[i][0];
	y0 = v[i-1][1];
	y1 = v[i][1];

	m = (y1 - y0)/(x1 - x0);

	y_inter = m*(x - x0) + y0;

	return y_inter;
}
