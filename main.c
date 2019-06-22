#include <SDL2/SDL.h>

#include "config.h"
#include "lista.h"
#include "graficador.h"
#include "nave.h"
#include "asteroide.h"
#include "time.h"

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
	nave_t *nave = nave_crear();
	if(nave == NULL) {
		graficador_finalizar();
		return 1;
	}

	asteroide_t * asteroide = asteroide_crear(300, 400, 32);

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
				}
				// END código del alumno


			}
			continue;
		}
        	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        	SDL_RenderClear(renderer);
        	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);


		// BEGIN código del alumno
		nave_mover(nave, DT);
		
		asteroide_mover(asteroide, DT);
		if(asteroide_colision(asteroide, nave_get_x(nave), nave_get_y(nave)))
			return 0;

		nave_dibujar(nave);
		asteroide_dibujar(asteroide);
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
