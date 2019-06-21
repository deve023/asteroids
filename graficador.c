#include "graficador.h"

typedef struct sprite {
    char nombre[10];
    uint16_t n;
    float **coords;
} sprite_t;

static sprite_t *sprite_crear() {
    sprite_t *s = malloc(sizeof(sprite_t));
    if(s == NULL)
        return NULL;

    return s;
}

static void sprite_destruir(sprite_t *s) {
    for(size_t i = 0; i < s->n; i++)
        free(s->coords[i]);
    free(s);
}

static int sprite_comparar_nombre(const sprite_t *s, const char *n) {
    return strcmp(s->nombre, n);
}

bool graficador_inicializar(const char *fn, SDL_Renderer *r) {
    FILE *fp;
    fp = fopen(fn, "rf");
    if(fp == NULL)
        return false;

    ls = lista_crear();
    if(ls == NULL) {
        fclose(fp);
        return false;
    }

    while(!feof(fp)) {
        sprite_t *s = sprite_crear();
        if(s == NULL){
            lista_destruir(ls, sprite_destruir);
            fclose(fp);
            return false;
        }

        fread(s->nombre, 10, sizeof(char), fp); //Guardo nombre[10]
        fread(&(s->n), 1, sizeof(uint16_t), fp); //Guardo n

        s->coords = malloc(sizeof(float *) * s->n);
        if(s->coords == NULL) {
            free(s);
            lista_destruir(ls, sprite_destruir);
            fclose(fp);
            return false;
        }

        for(size_t i = 0; i < s->n; i++){
            (s->coords)[i] = malloc(sizeof(float) * 2);
            if((s->coords)[i] == NULL) {
                sprite_destruir(s);
                lista_destruir(ls, sprite_destruir); //Si llega a entrar aca, va a liberar memoria de mas, pero esta bien, no?
                fclose(fp);
                return false;

            fread(s->coords, 2 * s->n, sizeof(float), fp);
            }
        }

        lista_insertar_final(ls, s);
    }
    fclose(fp);

    renderer = r;

    return true;
}
void graficador_finalizar() {
    lista_destruir(ls, sprite_destruir);
}

void graficador_ajustar_variables(float *x, float *y) {
    if(*x > VENTANA_ANCHO)
        *x = 0;
    else if(*x < 0)
        *x = VENTANA_ANCHO;

    if(*y > VENTANA_ALTO)
        *y = 0;
    else if(*y < 0)
        *y = VENTANA_ALTO;
}

bool graficador_dibujar(const char *nombre, float escala, float x, float y, float angulo) {
    sprite_t *s = lista_buscar(ls, nombre, sprite_comparar_nombre);
    if(s == NULL)
        return false;

    //vector_rotar(s->coords, s->n, angulo);
    for(size_t i = 0; i < (s->n) - 1; i++)
			SDL_RenderDrawLine(
				renderer,
				(s->coords)[i][0],
				-(s->coords)[i][1],
				(s->coords)[i+1][0],
				-(s->coords)[i+1][1]
			);
    //vector_rotar(s->coords, s->n, -angulo);

    return true;
}