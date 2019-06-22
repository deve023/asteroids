#include <SDL2/SDL.h>
#include <time.h>

#include "config.h"
#include "lista.h"
#include "graficador.h"
#include "diccionario.h"
#include "nave.h"
#include "asteroide.h"
#include "disparo.h"



#define DT (1.0 / JUEGO_FPS)



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

	graficador_inicializar("sprites.bin", renderer);
	nave_t *nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL);
	if(nave == NULL) {
		graficador_finalizar();
		return 1;
	}
	
	lista_t * lista_disp = lista_crear();
	lista_t * lista_ast = lista_crear();
	lista_t * lista_vidas = lista_crear();


	//creo 4 asteroides
	for(size_t i = 0; i < 4; i++)
	{
		lista_insertar_final(lista_ast, asteroide_crear(0, 0, 32)); //hay que hacerlo random
	}

	size_t vidas = VIDAS_CANT_INICIAL;
	for(size_t i = 0; i < vidas; i++)
	{
		lista_insertar_final(lista_vidas, nave_crear(100+15*i, VENTANA_ALTO-100, PI/2));
	}

	bool nave_murio = false;
	float nave_espera = 0;

	bool game_over = false;
	// END código del alumno

	unsigned int ticks = SDL_GetTicks();
	while(1) {
		if(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				break;
	    		if (event.type == SDL_KEYDOWN) {
				// BEGIN código del alumno
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						nave_impulso(nave);
						break;
					case SDLK_DOWN:
						break;
					case SDLK_RIGHT:
						nave_incrementar_angulo(nave, -NAVE_ROTACION_PASO);
						break;
					case SDLK_LEFT:
						nave_incrementar_angulo(nave, NAVE_ROTACION_PASO);
						break;
					case SDLK_SPACE:
						//creo un disparo
						lista_insertar_final(
							lista_disp, 
							disparo_crear(nave_get_x(nave), nave_get_y(nave), nave_get_angulo(nave))
							);
						if(game_over)
						{
							nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL);
							for(size_t i = 0; i < vidas; i++)
							{
								lista_insertar_final(lista_vidas, nave_crear(100+15*i, VENTANA_ALTO-100, PI/2));
							}
							lista_destruir(lista_ast, asteroide_destruir);
							lista_ast = lista_crear();
							
							game_over = false;
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

		

		if(game_over)
		{
			cadena_graficar("GAME OVER", 450, 400, 2, renderer);
		}
		
		


		//recorre la lista de asteroides moviendo y dibujando cada uno, y chequeando colision con la nave
        lista_iterador_t * iter_ast = lista_iterador_crear(lista_ast);
		for(
			iter_ast = lista_iterador_crear(lista_ast);
			!lista_iterador_termino(iter_ast);
			lista_iterador_siguiente(iter_ast)
		){
			asteroide_t * a = lista_iterador_actual(iter_ast);
			asteroide_mover(a, DT);
			if(asteroide_colision(a, nave_get_x(nave), nave_get_y(nave)))
			{
				lista_extraer_ultimo(lista_vidas);
				nave_destruir(nave);
				nave_murio = true;
				//return 0; //lol
			}
			if(lista_es_vacia(lista_vidas))
			{
				game_over = true;
			}
			if(nave_murio && !game_over && nave_espera >= 1)
			{
				nave_espera = 0;
				if(!asteroide_colision(a,NAVE_X_INICIAL,NAVE_Y_INICIAL))
				{
					nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL);
					nave_murio = false;
				}
			}
			asteroide_dibujar(a);
		}
		lista_iterador_destruir(iter_ast);


		//recorre la lista de disparos moviendo y dibujando cada uno, 
		//elimina a los que tienen mas de 0.7 segundos de vida
		lista_iterador_t * iter_disp = lista_iterador_crear(lista_disp);
		for(
			iter_disp = lista_iterador_crear(lista_disp);
			!lista_iterador_termino(iter_disp);
			lista_iterador_siguiente(iter_disp)
		){
			disparo_t * d = lista_iterador_actual(iter_disp);
			if(disparo_get_tiempo(d) >= 0.7)
				lista_iterador_eliminar(iter_disp);
			disparo_mover(d, DT);
			disparo_dibujar(d);
		}
		lista_iterador_destruir(iter_disp);

		lista_iterador_t * iter_vidas = lista_iterador_crear(lista_vidas);
		for(
			iter_vidas = lista_iterador_crear(lista_vidas);
			!lista_iterador_termino(iter_vidas);
			lista_iterador_siguiente(iter_vidas)
		){
			nave_t * v = lista_iterador_actual(iter_vidas);
			nave_dibujar(v);
		}
		lista_iterador_destruir(iter_vidas);

		printf("x: %f\n y: %f\n", nave_get_x(nave), nave_get_y(nave));

		if(!nave_murio)
		{
			nave_mover(nave, DT);
			nave_dibujar(nave);
		}

		if(nave_murio)
		{
			nave_espera += DT;
		}


		

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
	graficador_finalizar();
	// END código del alumno

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
