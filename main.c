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

	nave_t *nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL); //creo nave 
	if(nave == NULL) {
		graficador_finalizar();
		return 1;
	}
	
	lista_t * lista_disp = lista_crear(); //creo lista de disparos (validar)
	lista_t * lista_ast = lista_crear(); //creo lista de asteroides (validar)


	//creo 4 asteroides
	for(size_t i = 0; i < 10; i++)
	{
		lista_insertar_final(lista_ast, asteroide_crear(0, 0, 32)); //hay que hacerlo random
	}

	//variables del juego:

	int vidas = VIDAS_CANT_INICIAL;
	
	bool nave_murio = false;

	float nave_espera = 0; //tiempo que espero para crear la nave despues de morir

	bool nave_creacion_colision = false; //me dice si en el lugar de aparicion de la nave hay asteroides

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
						if(vidas == 0)
						{
							nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL);
							nave_murio = false;
							vidas = VIDAS_CANT_INICIAL;
							lista_destruir(lista_ast, asteroide_destruir);
							lista_ast = lista_crear();
							for(size_t i = 0; i < 10; i++)
							{
								lista_insertar_final(lista_ast, asteroide_crear(0, 0, 32)); //hay que hacerlo random
							}
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
			cadena_graficar("GAME OVER", 400, 300, 4, renderer);
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

	        if(nave_creacion_colision == true) //si hay colision reseteo el contador para esperar 0.1 segundos mas
			{
				puts("*");
				nave_espera = 0.9;
				nave_creacion_colision = false;

			}
			if(nave_creacion_colision == false && nave_espera >=1) //si no hay colision creo la nave
			{
				nave = nave_crear(NAVE_X_INICIAL, NAVE_Y_INICIAL, NAVE_ANGULO_INICIAL);
				nave_espera = 0;
				nave_murio = false;
			}
		}


		//todo lo siguiente pasa durante la partida y game over

		//ITERADOR ASTEROIDES:
		//recorre la lista de asteroides moviendo y dibujando cada uno, y chequeando colision con la nave
        lista_iterador_t * iter_ast = lista_iterador_crear(lista_ast);
		for(
			iter_ast = lista_iterador_crear(lista_ast);
			!lista_iterador_termino(iter_ast);
			lista_iterador_siguiente(iter_ast)
		){
			asteroide_t * a = lista_iterador_actual(iter_ast);//obtengo asteroide actual
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
				if(asteroide_colision(a,NAVE_X_INICIAL,NAVE_Y_INICIAL))
				{
					nave_creacion_colision = true;
				}
			}	
		}
		lista_iterador_destruir(iter_ast);


		//ITERADOR DISPAROS
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


		//grafica las vidas en la parte superior izquierda de la pantalla
		for(size_t i = 0; i < vidas; i++)
		{
			graficador_dibujar(NAVE_SPRITE, NAVE_ESCALA, 100+15*i, VENTANA_ALTO-100, PI/2);
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
