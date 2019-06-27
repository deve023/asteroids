#include <SDL2/SDL.h>
#include <time.h>

#include "config.h"
#include "lista.h"
#include "graficador.h"
#include "diccionario.h"
#include "nave.h"
#include "asteroide.h"
#include "disparo.h"
#include "matematica.h"

#define DT (1.0 / JUEGO_FPS)
#define PUNTAJE_DIGITOS_MAX 8 //basado en el high score historico

// Crea una lista con n asteroides.
lista_t *inicializar_asteroides(int n);

/*
Recibe un puntaje entero, una posicion (x,y), un factor de escala y un renderer.
Dibuja en la pantalla el puntaje trasladado a (x,y) y dimensionado por escala, en formato Asteroids.
*/
void puntaje_graficar_asteroids(int puntaje, float x, float y, float escala, SDL_Renderer * renderer);

int main() {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;

	SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
	SDL_SetWindowTitle(window, "Asteroids");

	int dormir = 0;

	// BEGIN código del alumno
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

	int asteroides_cant = ASTEROIDES_CANT_INICIAL; //partida a partida aumenta de a dos.
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

	int score = 0;

	int best_score = 0;

	// END código del alumno

	unsigned int ticks = SDL_GetTicks();
	while(1) 
	{
		if(SDL_PollEvent(&event)) 
		{
			if(event.type == SDL_QUIT)
				break;

	    	if(event.type == SDL_KEYDOWN) 
	    	{
				// BEGIN código del alumno
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
						//creamos un disparo
						if(!nave_murio) 
						{
							disparo_t * dn = disparo_crear(nave_get_x(nave), nave_get_y(nave), nave_get_angulo(nave));
							if(dn == NULL)
							{
								fputs("Error de asignacion de memoria.\n", stderr);
								graficador_finalizar();
								nave_destruir(nave);
								lista_destruir(lista_ast, asteroide_destruir);
								lista_destruir(lista_disp, disparo_destruir);
							}

							lista_insertar_final(lista_disp, dn);
						}

						if(vidas == 0) 
						{
							nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL);
							if(nave == NULL) 
							{
								fputs("Error de asignacion de memoria.\n", stderr);
								graficador_finalizar();
								lista_destruir(lista_disp, disparo_destruir);
								return 1;
							}
							
							nave_murio = false;
							vidas = VIDAS_CANT_INICIAL;
							
							lista_destruir(lista_ast, asteroide_destruir);
							asteroides_cant = ASTEROIDES_CANT_INICIAL; //reseteo la cantidad inicial de asteroides al comenzar nueva partida
							lista_ast = inicializar_asteroides(asteroides_cant);
							if(lista_ast == NULL) 
							{
								fputs("Error de asignacion de memoria.\n", stderr);
								graficador_finalizar();
								nave_destruir(nave);
								lista_destruir(lista_disp, disparo_destruir);
								return 1;
							}
							
							score = 0;
						}
						break;
				}
				// END código del alumno
			}
			continue;
		}
        	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        	SDL_RenderClear(renderer);
        	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);


		// BEGIN código del alumno

      	//esto pasa solo en game over
        if(vidas == 0)
		{
			cadena_graficar("GAME OVER", 400, 500, 4, renderer);

			//actualizo el mejor puntaje si es necesario
			if(score > best_score)
				best_score = score;
		}

		//esto pasa solo durante la partida
		if(vidas != 0)
		{
	        if(!nave_murio) //si la nave esta viva la muevo y la grafico
			{
				nave_mover(nave, DT);
				nave_dibujar(nave);
			}

	        if(nave_murio) //si la nave esta muerta sumo tiempo de espera
	        {
				nave_espera += DT;
	        }

	        //si llego el tiempo de que la nave aparezca:

	        if(nave_creacion_colision == true) //si hay colision reseteamos el contador para esperar 0.1 segundos mas
			{
				puts("*");
				nave_espera = 0.9;
				nave_creacion_colision = false;

			}
			if(nave_creacion_colision == false && nave_espera >=1) //si no hay colision creamos la nave
			{
				nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL);
				if(nave == NULL)
				{
					fputs("Error de asignacion de memoria.\n", stderr);
					graficador_finalizar();
					nave_destruir(nave);
					lista_destruir(lista_ast, asteroide_destruir);
					lista_destruir(lista_disp, disparo_destruir);
				}
				nave_espera = 0;
				nave_murio = false;
			}
		}


		//todo lo siguiente pasa durante la partida y game over

		//ITERADOR ASTEROIDES:
		//recorre la lista de asteroides moviendo y dibujando cada uno, y chequeando colision con la nave y disparos
        lista_iterador_t * iter_ast = lista_iterador_crear(lista_ast);
        if(iter_ast == NULL)
		{
			fputs("Error de asignacion de memoria.\n", stderr);
			graficador_finalizar();
			nave_destruir(nave);
			lista_destruir(lista_ast, asteroide_destruir);
			lista_destruir(lista_disp, disparo_destruir);
		}
		for(; !lista_iterador_termino(iter_ast); lista_iterador_siguiente(iter_ast))
		{
			asteroide_t * a = lista_iterador_actual(iter_ast); //obtengo asteroide actual
			asteroide_mover(a, DT); //muevo el asteroide actual
			asteroide_dibujar(a); //dibujo el asteroide actual

			//si la nave esta viva veo si se choca con un asteroide
			if(!nave_murio && asteroide_colision(a, nave_get_x(nave), nave_get_y(nave)))
			{
				vidas --;
				nave_destruir(nave);
				nave_murio = true;
			}
			//si la nave esta muerta y es tiempo de aparecer, veo si hay colision en el lugar de spawn
			if(nave_murio && vidas!=0 && nave_espera >= 1)
			{
				nave_espera = 0;
				if(asteroide_colision(a, NAVE_X_INICIAL, NAVE_Y_INICIAL))
				{
					nave_creacion_colision = true;
				}
			}

			//veo si el asteroide colisiona con algun disparo
			lista_iterador_t *iter_disp = lista_iterador_crear(lista_disp);
			if(iter_disp == NULL)
			{
				fputs("Error de asignacion de memoria.\n", stderr);
				graficador_finalizar();
				nave_destruir(nave);
				lista_destruir(lista_ast, asteroide_destruir);
				lista_destruir(lista_disp, disparo_destruir);
				lista_iterador_destruir(iter_ast);
			}
			for(; !lista_iterador_termino(iter_disp); lista_iterador_siguiente(iter_disp))
			{
				disparo_t *d = lista_iterador_actual(iter_disp);
				if(asteroide_colision(a, disparo_get_x(d), disparo_get_y(d)))
				{
					float x = asteroide_get_x(a);
					float y = asteroide_get_y(a);

					if(asteroide_get_radio(a) == 8) //destruimos el asteroide actual
					{
						asteroide_destruir(lista_iterador_eliminar(iter_ast));

						score += 100;
					}

					else if(asteroide_get_radio(a) == 16) //destruimos el actual y creamos dos asteroides de radio 8
					{
						asteroide_destruir(lista_iterador_eliminar(iter_ast));

						asteroide_t * an = asteroide_crear(x,y,8);
						if(an == NULL)
						{
							fputs("Error de asignacion de memoria.\n", stderr);
							graficador_finalizar();
							nave_destruir(nave);
							lista_destruir(lista_ast, asteroide_destruir);
							lista_destruir(lista_disp, disparo_destruir);
						}
						lista_insertar_final(lista_ast, an);

						an = asteroide_crear(x,y,8);
						if(an == NULL)
						{
							fputs("Error de asignacion de memoria.\n", stderr);
							graficador_finalizar();
							nave_destruir(nave);
							lista_destruir(lista_ast, asteroide_destruir);
							lista_destruir(lista_disp, disparo_destruir);
						}
						lista_insertar_final(lista_ast, an);

						score += 50;
					}

					else //destruimos el actual y creamos dos asteroides de radio 16
					{
						asteroide_destruir(lista_iterador_eliminar(iter_ast));

						asteroide_t * an = asteroide_crear(x,y,16);
						if(an == NULL)
						{
							fputs("Error de asignacion de memoria.\n", stderr);
							graficador_finalizar();
							nave_destruir(nave);
							lista_destruir(lista_ast, asteroide_destruir);
							lista_destruir(lista_disp, disparo_destruir);
						}
						lista_insertar_final(lista_ast, an);

						an = asteroide_crear(x,y,16);
						if(an == NULL)
						{
							fputs("Error de asignacion de memoria.\n", stderr);
							graficador_finalizar();
							nave_destruir(nave);
							lista_destruir(lista_ast, asteroide_destruir);
							lista_destruir(lista_disp, disparo_destruir);
						}
						lista_insertar_final(lista_ast, an);

						score += 20;
					}

					disparo_destruir(lista_iterador_eliminar(iter_disp));

					break;
				}
			}
			lista_iterador_destruir(iter_disp);
		}
		lista_iterador_destruir(iter_ast);

		if(lista_es_vacia(lista_ast)) //veo si murieron todos los asteroides
		{
			free(lista_ast);
			asteroides_cant += 2;
			lista_ast = inicializar_asteroides(asteroides_cant); //creo lista de asteroides, con 10 asteroides.
			if(lista_ast == NULL) {
				fputs("Error de asignacion de memoria.\n", stderr);
				graficador_finalizar();
				nave_destruir(nave);
				lista_destruir(lista_disp, disparo_destruir);
				return 1;
			}
		}

		//ITERADOR DISPAROS
		//recorre la lista de disparos moviendo y dibujando cada uno,
		//elimina a los que tienen mas de 0.7 segundos de vida
		lista_iterador_t *iter_disp = lista_iterador_crear(lista_disp);
		if(iter_disp == NULL)
		{
			fputs("Error de asignacion de memoria.\n", stderr);
			graficador_finalizar();
			nave_destruir(nave);
			lista_destruir(lista_ast, asteroide_destruir);
			lista_destruir(lista_disp, disparo_destruir);
		}
		for(; !lista_iterador_termino(iter_disp); lista_iterador_siguiente(iter_disp))
		{
			disparo_t * d = lista_iterador_actual(iter_disp);
			disparo_mover(d, DT);
			disparo_dibujar(d);
			if(disparo_get_tiempo(d) >= DISPARO_TIEMPO_VIDA)
				disparo_destruir(lista_iterador_eliminar(iter_disp));
		}
		lista_iterador_destruir(iter_disp);

		//graficamos las vidas en la parte superior izquierda de la pantalla
		for(size_t i = 0; i < vidas; i++)
			graficador_dibujar(NAVE_SPRITE, NAVE_ESCALA, 100+15*i, VENTANA_ALTO-100, PI/2);

		//graficamos el puntaje en la parte superior izquierda de la pantalla
		puntaje_graficar_asteroids(score, 120, VENTANA_ALTO-75, 3, renderer);

		//graficamos el mejor puntaje en el centro superior de la pantalla
		puntaje_graficar_asteroids(best_score, VENTANA_ANCHO/2, VENTANA_ALTO-75, 2, renderer);


		// END código del alumno

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

	// BEGIN código del alumno
	nave_destruir(nave);
	lista_destruir(lista_ast, asteroide_destruir);
	lista_destruir(lista_disp, disparo_destruir);
	graficador_finalizar();
	// END código del alumno

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}

lista_t *inicializar_asteroides(int n) {
	lista_t *lista_ast = lista_crear();
	if(lista_ast == NULL)
		return NULL;

	float x, y; // Posicion incial de cada asteroide

	//creamos n asteroides
	for(size_t i = 0; i < n; i++) {
		int s = rand() % 2;
		if(s == 0) {
			x = 0;
			y = randomf(0, VENTANA_ALTO);
		}
		else {
			x = randomf(0, VENTANA_ANCHO);
			y = 0;
		}

		if(!lista_insertar_final(lista_ast, asteroide_crear(x, y, 32))) {
			lista_destruir(lista_ast, asteroide_destruir);
			return NULL;
		}
	}

	return lista_ast;
}

void puntaje_graficar_asteroids(int puntaje, float x, float y, float escala, SDL_Renderer * renderer)
{
		if(puntaje <= 9)
			contador_graficar_ceros(puntaje, 2, x, y, escala, renderer);
		else
			contador_graficar_derecha(puntaje, PUNTAJE_DIGITOS_MAX, x-(PUNTAJE_DIGITOS_MAX-2)*CARACTER_ANCHO*escala, y, escala, renderer);
}
