#include "diccionario.h"
#include "caracteres.h"

#include <stdio.h>
#include <math.h>

#define MAX_CARACTERES 100
#define CANT_CARACTERES 41


typedef struct
{
	char caracter;
	int tam;
	matriz_t matriz;
}car_t;

const car_t caracteres_dic[]=
{
{'0', 5, caracter_0},
{'1', 2, caracter_1},
{'2', 6, caracter_2},
{'3', 7, caracter_3},
{'4', 5, caracter_4},
{'5', 6, caracter_5},
{'6', 5, caracter_6},
{'7', 3, caracter_7},
{'8', 7, caracter_8},
{'9', 5, caracter_9},
{'A', 7, caracter_a},
{'B', 12, caracter_b},
{'C', 4, caracter_c},
{'D', 7, caracter_d},
{'E', 7, caracter_e},
{'F', 6, caracter_f},
{'G', 7, caracter_g},
{'H', 6, caracter_h},
{'I', 6, caracter_i},
{'J', 4, caracter_j},
{'K', 6, caracter_k},
{'L', 3, caracter_l},
{'M', 5, caracter_m},
{'N', 4, caracter_n},
{'O', 5, caracter_o},
{'P', 5, caracter_p},
{'Q', 9, caracter_q},
{'R', 7, caracter_r},
{'S', 6, caracter_s},
{'T', 4, caracter_t},
{'U', 4, caracter_u},
{'V', 3, caracter_v},
{'W', 5, caracter_w},
{'X', 5, caracter_x},
{'Y', 5, caracter_y},
{'Z', 4, caracter_z},
{'>', 5, caracter_derecha},
{'<', 5, caracter_izquierda},
{'^', 5, caracter_arriba},
{'v', 5, caracter_abajo},
{' ', 1, caracter_espacio}
};

matriz_t caracter_a_matriz(char caracter, size_t * n)
{
		size_t i;
		for(i=0; i<CANT_CARACTERES; i++)
		{
			if(caracter == caracteres_dic[i].caracter)
			{
				*n = caracteres_dic[i].tam;
				return caracteres_dic[i].matriz;
			}
		}
		return caracteres_dic[CANT_CARACTERES-1].matriz; //por omision devuelve un espacio
}

void caracter_graficar(matriz_t matriz, size_t n, float x, float y, float escala, SDL_Renderer * renderer)
{
		int w, h;
		SDL_GetRendererOutputSize(renderer, &w, &h);
		for(int i = 0; i < n-1; i++)
		{
			SDL_RenderDrawLine(
				renderer,
				matriz[i][0] * escala + x,
				-matriz[i][1] * escala - y + h,
				matriz[i+1][0] * escala + x,
				-matriz[i+1][1] * escala - y + h
			);
		}
}

void cadena_graficar(char * cadena, float x, float y, float escala, SDL_Renderer * renderer)
{
	size_t tam;
	for(size_t i = 0; cadena[i]; i++)
	{
		matriz_t matriz = caracter_a_matriz(cadena[i], &tam);
		caracter_graficar(matriz, tam, x, y, escala, renderer);
		x += CARACTER_ANCHO * escala;
	}
}


void contador_graficar_ceros(int contador, size_t n, float x, float y, float escala, SDL_Renderer * renderer)
{
	char aux[n];
	snprintf(aux, n+1, "%0*d", (int)n-abs(contador)/1000, abs(contador));
	cadena_graficar(aux, x, y, escala, renderer);

}

void contador_graficar_derecha(int contador, size_t n, float x, float y, float escala, SDL_Renderer * renderer)
{
	char aux[n];
	snprintf(aux, n+1, "%*d", (int)n, abs(contador));
	cadena_graficar(aux, x, y, escala, renderer);
}
