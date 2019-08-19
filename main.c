#include <SDL2/SDL.h>
#include <time.h>

#include "config.h"
#include "lista.h"
#include "graficador.h"
#include "diccionario.h"
#include "caracteres.h"
#include "nave.h"
#include "asteroide.h"
#include "disparo.h"
#include "matematica.h"

#define DT (1.0 / JUEGO_FPS)
#define PUNTAJE_DIGITOS_MAX 8 //basado en el high score historico

// Crea una lista con n asteroides en posiciones aleatorias sobre los bordes de la pantalla
lista_t *inicializar_asteroides(int n);

// Destruye el asteroide a y crea dos asteroides mas pequenos dependiendo del radio
bool asteroide_split(asteroide_t *a, lista_iterador_t *iter_ast, lista_t *lista_ast, int *puntaje);

/*
Verifica la colision entre un asteroide y cada disparo de la lista lista_disp, y hace los cambios necesarios
en las variables del juego. Devuelve false en caso de error, devuelve true si se ejecuto correctamente.
*/
bool colision_asteroide_disparo(asteroide_t * a, lista_t * lista_ast, lista_t * lista_disp, lista_iterador_t * iter_ast, int * puntaje);

// Recorre la lista disparos, moviendo, dibujando y destruyendo, de ser necesario, los disparos
bool actualizar_disparos(lista_t *lista_disp);

//Libera la memora asociada a la nave y a las listas de asteroides y de disparos, y finaliza el graficador
void destruir_objetos(nave_t * nave, lista_t * lista_ast, lista_t * lista_disp);

/*
Recibe un puntaje entero, una posicion (x,y), un factor de escala y un renderer.
Dibuja en la pantalla el puntaje trasladado a (x,y) y dimensionado por escala, en formato Asteroids.
*/
void puntaje_graficar_asteroids(int puntaje, float x, float y, float escala, SDL_Renderer * renderer);

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;

	SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
	SDL_SetWindowTitle(window, "Asteroids");

	int dormir = 0;

	srand(time(NULL));

	if(!graficador_inicializar("sprites.bin", renderer))
	{
		fputs("Error de inicializacion de graficador.\n", stderr);

		return 1;
	}

	nave_t *nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL); //creamos nave
	if(nave == NULL) {
		fputs("Error de asignacion de memoria.\n", stderr);
		graficador_finalizar();

		return 1;
	}

	lista_t * lista_disp = lista_crear(); //creamos lista de disparos
	if(lista_disp == NULL) {
		fputs("Error de asignacion de memoria.\n", stderr);
		graficador_finalizar();
		nave_destruir(nave);

		return 1;
	}

	int asteroides_cant = ASTEROIDES_CANT_INICIAL;
	lista_t * lista_ast = inicializar_asteroides(asteroides_cant); //creamos lista de asteroides.
	if(lista_ast == NULL) {
		fputs("Error de asignacion de memoria.\n", stderr);
		graficador_finalizar();
		nave_destruir(nave);
		lista_destruir(lista_disp, disparo_destruir);

		return 1;
	}

	//variables del juego:

	int vidas = VIDAS_CANT_INICIAL;

	bool nave_murio = false;

	bool nave_creacion_colision = false; //dice si en el lugar de aparicion de la nave hay asteroides

	float nave_espera = 0; //tiempo que esperamos para crear la nave despues de morir

	int puntaje = 0;

	int mejor_puntaje = 0;

	unsigned int ticks = SDL_GetTicks();

	while(1)
	{
		if(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				break;

	    	if(event.type == SDL_KEYDOWN)
	    	{
				switch(event.key.keysym.sym)
				{
					case SDLK_UP:
						if(!nave_murio)
							nave_impulso(nave);
						break;

					case SDLK_RIGHT:
						if(!nave_murio)
							nave_incrementar_angulo(nave, -NAVE_ROTACION_PASO);
						break;

					case SDLK_LEFT:
						if(!nave_murio)
							nave_incrementar_angulo(nave, NAVE_ROTACION_PASO);
						break;

					case SDLK_SPACE:
						if(!nave_murio)
						{
							disparo_t * dn = disparo_crear(nave_get_x(nave), nave_get_y(nave), nave_get_angulo(nave));
							if(dn == NULL)
							{
								fputs("Error de asignacion de memoria.\n", stderr);
								destruir_objetos(nave, lista_ast, lista_disp);
								return 1;
							}

							lista_insertar_final(lista_disp, dn);
						}

						if(vidas == 0)
						{
							nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL);
							if(nave == NULL)
							{
								fputs("Error de asignacion de memoria.\n", stderr);
								destruir_objetos(nave, lista_ast, lista_disp);
								return 1;
							}

							nave_murio = false;
							vidas = VIDAS_CANT_INICIAL;
							puntaje = 0;

							lista_destruir(lista_ast, asteroide_destruir);
							asteroides_cant = ASTEROIDES_CANT_INICIAL; //reseteo la cantidad de asteroides al comenzar nueva partida
							lista_ast = inicializar_asteroides(asteroides_cant);
							if(lista_ast == NULL)
							{
								fputs("Error de asignacion de memoria.\n", stderr);
								destruir_objetos(nave, lista_ast, lista_disp);
								return 1;
							}
						}
						break;
				}
			}
			continue;
		}

    	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    	SDL_RenderClear(renderer);
    	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);


        if(vidas == 0) //esto pasa solo en game over
		{
			cadena_graficar("GAME OVER", 400, 500, 4, renderer);

			//actualizo el mejor puntaje si es necesario
			if(puntaje > mejor_puntaje)
				mejor_puntaje = puntaje;
		}


		if(vidas != 0) //esto pasa solo durante la partida
		{
	        if(!nave_murio) //si la nave esta viva la movemos y graficamos
			{
				nave_mover(nave, DT);
				nave_dibujar(nave);
			}

	        if(nave_murio) //si la nave esta muerta sumamos tiempo de espera
				nave_espera += DT;


	        //si llego el tiempo de que la nave aparezca:

	        if(nave_creacion_colision == true) //si hay colision reseteamos el contador para esperar 0.1 segundos mas
			{
				nave_espera = 0.9;
				nave_creacion_colision = false;
			}
			if(nave_creacion_colision == false && nave_espera >=1) //si no hay colision creamos la nave
			{
				nave_espera = 0;
				nave_murio = false;

				nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL);
				if(nave == NULL)
				{
					fputs("Error de asignacion de memoria.\n", stderr);
					destruir_objetos(nave, lista_ast, lista_disp);
					return 1;
				}
			}
		}


		//ITERADOR ASTEROIDES:
		//recorre la lista de asteroides moviendo y dibujando cada uno, y chequeando colision con la nave y disparos
        lista_iterador_t * iter_ast = lista_iterador_crear(lista_ast);
        if(iter_ast == NULL)
		{
			fputs("Error de asignacion de memoria.\n", stderr);
			destruir_objetos(nave, lista_ast, lista_disp);
			return 1;
		}

		for(; !lista_iterador_termino(iter_ast); lista_iterador_siguiente(iter_ast))
		{
			asteroide_t * a = lista_iterador_actual(iter_ast); //obtenemos asteroide actual
			asteroide_mover(a, DT); //movemos asteroide actual
			asteroide_dibujar(a); //dibujamos asteroide actual

			//si la nave esta viva, chequeamos si colisiona con el asteroide
			if(!nave_murio && asteroide_colision(a, nave_get_x(nave), nave_get_y(nave)))
			{
				vidas --;
				nave_destruir(nave);
				nave = NULL;
				nave_murio = true;

				//destruimos el asteroide y creamos los asteroides correspondientes
				if(!asteroide_split(a, iter_ast, lista_ast, &puntaje))
				{
					fputs("Error de asignacion de memoria.\n", stderr);
					destruir_objetos(nave, lista_ast, lista_disp);
					lista_iterador_destruir(iter_ast);
					return 1;
				}

				continue;
			}

			//si la nave esta muerta y es tiempo de aparecer, chequeamos colision en el lugar de spawn
			if(nave_murio && vidas!=0 && nave_espera >= 1)
			{
				nave_espera = 0;
				if(asteroide_colision(a, NAVE_X_INICIAL, NAVE_Y_INICIAL))
				{
					nave_creacion_colision = true;
				}
			}

			//chequeamos si el asteroide colisiona con algun disparo
			if(!colision_asteroide_disparo(a, lista_ast, lista_disp, iter_ast, &puntaje))
			{
				fputs("Error de asignacion de memoria.\n", stderr);
				destruir_objetos(nave, lista_ast, lista_disp);
				lista_iterador_destruir(iter_ast);
				return 1;
			}
		}
		lista_iterador_destruir(iter_ast);

		//chequeamos si murieron todos los asteroides
		if(lista_es_vacia(lista_ast))
		{
			free(lista_ast);
			asteroides_cant += ASTEROIDES_INCREMENTO;
			lista_ast = inicializar_asteroides(asteroides_cant);
			if(lista_ast == NULL)
			{
				fputs("Error de asignacion de memoria.\n", stderr);
				destruir_objetos(nave, lista_ast, lista_disp);
				return 1;
			}
		}

		//actualizamos los disparos
		if(!actualizar_disparos(lista_disp))
		{
			fputs("Error de asignacion de memoria.\n", stderr);
			destruir_objetos(nave, lista_ast, lista_disp);
			lista_iterador_destruir(iter_ast);
			return 1;
		}


		//graficamos las vidas en la parte superior izquierda de la pantalla
		for(size_t i = 0; i < vidas; i++)
			graficador_dibujar(NAVE_SPRITE, NAVE_ESCALA, 100+15*i, VENTANA_ALTO-100, PI/2);

		//graficamos el puntaje en la parte superior izquierda de la pantalla
		puntaje_graficar_asteroids(puntaje, 120, VENTANA_ALTO-75, 3, renderer);

		//graficamos el mejor puntaje en el centro superior de la pantalla
		puntaje_graficar_asteroids(mejor_puntaje, VENTANA_ANCHO/2, VENTANA_ALTO-75, 2, renderer);


        SDL_RenderPresent(renderer);
		ticks = SDL_GetTicks() - ticks;
		if(dormir) {
			SDL_Delay(dormir);
			dormir = 0;
		}
		else if(ticks < 1000 / JUEGO_FPS)
			SDL_Delay(1000 / JUEGO_FPS - ticks);
		ticks = SDL_GetTicks();
	}

	destruir_objetos(nave, lista_ast, lista_disp);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}

lista_t *inicializar_asteroides(int n)
{
	lista_t *lista_ast = lista_crear();
	if(lista_ast == NULL)
		return NULL;

	float x, y; // Posicion incial de cada asteroide

	//creamos n asteroides
	for(size_t i = 0; i < n; i++)
	{
		int s = rand() % 2;
		if(s == 0)
		{
			x = 0;
			y = randomf(0, VENTANA_ALTO);
		}
		else
		{
			x = randomf(0, VENTANA_ANCHO);
			y = 0;
		}

		if(!lista_insertar_final(lista_ast, asteroide_crear(x, y, ASTEROIDE_RADIO_GRANDE)))
		{
			lista_destruir(lista_ast, asteroide_destruir);
			return NULL;
		}
	}

	return lista_ast;
}

bool asteroide_split(asteroide_t *a, lista_iterador_t *iter_ast, lista_t *lista_ast, int *puntaje)
{
	float x = asteroide_get_x(a);
	float y = asteroide_get_y(a);
	float radio = asteroide_get_radio(a);

	asteroide_destruir(lista_iterador_eliminar(iter_ast));

	if(radio == ASTEROIDE_RADIO_CHICO)
	{
		*puntaje += PUNTAJE_ASTEROIDE_CHICO;
	}

	else if(radio == ASTEROIDE_RADIO_MEDIANO) //creamos dos asteroides de radio 8
	{
		asteroide_t * an = asteroide_crear(x,y,ASTEROIDE_RADIO_CHICO);
		if(an == NULL)
			return false;

		lista_insertar_final(lista_ast, an);

		an = asteroide_crear(x,y,ASTEROIDE_RADIO_CHICO);
		if(an == NULL)
			return false;

		lista_insertar_final(lista_ast, an);

		*puntaje += PUNTAJE_ASTEROIDE_MEDIANO;
	}

	else //creamos dos asteroides de radio 16
	{
		asteroide_t * an = asteroide_crear(x,y,ASTEROIDE_RADIO_MEDIANO);
		if(an == NULL)
			return false;

		lista_insertar_final(lista_ast, an);

		an = asteroide_crear(x,y,ASTEROIDE_RADIO_MEDIANO);
		if(an == NULL)
			return false;

		lista_insertar_final(lista_ast, an);

		*puntaje += PUNTAJE_ASTEROIDE_GRANDE;
	}
	return true;
}

bool actualizar_disparos(lista_t *lista_disp)
{
	lista_iterador_t *iter_disp = lista_iterador_crear(lista_disp);
	if(iter_disp == NULL)
		return false;

	for(; !lista_iterador_termino(iter_disp); lista_iterador_siguiente(iter_disp))
	{
		disparo_t * d = lista_iterador_actual(iter_disp);

		disparo_mover(d, DT);
		disparo_dibujar(d);
		if(disparo_get_tiempo(d) >= DISPARO_TIEMPO_VIDA)
			disparo_destruir(lista_iterador_eliminar(iter_disp));
	}
	lista_iterador_destruir(iter_disp);

	return true;
}

void puntaje_graficar_asteroids(int puntaje, float x, float y, float escala, SDL_Renderer * renderer)
{
		if(puntaje <= 9)
			contador_graficar_ceros(puntaje, 2, x, y, escala, renderer);
		else
			contador_graficar_derecha(puntaje, PUNTAJE_DIGITOS_MAX, x-(PUNTAJE_DIGITOS_MAX-2)*CARACTER_ANCHO*escala, y, escala, renderer);
}

bool colision_asteroide_disparo(asteroide_t * a, lista_t * lista_ast, lista_t * lista_disp, lista_iterador_t * iter_ast, int * puntaje)
{
	lista_iterador_t *iter_disp = lista_iterador_crear(lista_disp);
	if(iter_disp == NULL)
	{
		return false;
	}
	for(; !lista_iterador_termino(iter_disp); lista_iterador_siguiente(iter_disp))
	{
		disparo_t *d = lista_iterador_actual(iter_disp);
		if(asteroide_colision(a, disparo_get_x(d), disparo_get_y(d)))
		{
			if(!asteroide_split(a, iter_ast, lista_ast, puntaje))
			{
				lista_iterador_destruir(iter_disp);
				return false;
			}

			disparo_destruir(lista_iterador_eliminar(iter_disp));

			break;
		}
	}
	lista_iterador_destruir(iter_disp);

	return true;
}

void destruir_objetos(nave_t * nave, lista_t * lista_ast, lista_t * lista_disp)
{
	nave_destruir(nave);
	nave = NULL;

	lista_destruir(lista_ast, asteroide_destruir);
	lista_ast = NULL;

	lista_destruir(lista_disp, disparo_destruir);
	lista_disp = NULL;

	graficador_finalizar();
}
