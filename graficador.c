#include "graficador.h"

typedef struct sprite {
    char nombre[TAM_NOMBRE];
    uint16_t n;
    float **coords;
} sprite_t;

static SDL_Renderer *renderer;

static int width, height;

//Lista con los sprites
static lista_t *ls;

static sprite_t *sprite_crear()
{
    sprite_t *s = malloc(sizeof(sprite_t));
    if(s == NULL)
        return NULL;

    s->n = 0;
    s->coords = NULL;

    return s;
}

static void sprite_destruir(void *s)
{
    vector_destruir(((sprite_t*)s)->coords, ((sprite_t*)s)->n);
    free(s);
}

static int sprite_comparar_nombre(const void *s, const void *n)
{
    return strcmp(((sprite_t*)s)->nombre, (char*)n);
}

bool graficador_inicializar(const char *fn, SDL_Renderer *r)
{
    FILE *fp = fopen(fn, "rb");
    if(fp == NULL)
        return false;

    ls = lista_crear();
    if(ls == NULL)
    {
        fclose(fp);
        return false;
    }

    while(!feof(fp))
    {
        sprite_t *s = sprite_crear();
        if(s == NULL)
        {
            lista_destruir(ls, sprite_destruir);
            fclose(fp);
            return false;
        }

        fread(s->nombre, sizeof(char), 10, fp);//Guardo nombre[10]
        fread(&(s->n), sizeof(uint16_t), 1, fp);//Guardo n

        s->coords = malloc(sizeof(float *) * s->n);
        if(s->coords == NULL)
        {
            free(s);
            lista_destruir(ls, sprite_destruir);
            fclose(fp);
            return false;
        }

        for(size_t i = 0; i < s->n; i++)
        {
            (s->coords)[i] = malloc(sizeof(float) * 2);
            if((s->coords)[i] == NULL)
            {
                sprite_destruir(s);
                lista_destruir(ls, sprite_destruir);
                fclose(fp);
                return false;
            }

            fread(s->coords[i], sizeof(float), 2, fp);
        }

        lista_insertar_final(ls, s);
    }

    fclose(fp);

    renderer = r;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    return true;
}

void graficador_finalizar()
{
    lista_destruir(ls, sprite_destruir);
}

void graficador_ajustar_variables(float *x, float *y)
{
    if(*x > width)
        *x = fmod(*x,width);
    else if(*x == 0)
        *x = width;
    else if(*x < 0)
        *x = width + fmod(*x,width);

    if(*y > height)
        *y = fmod(*y,height);
    else if(*y == 0)
        *y = height;
    else if(*y < 0)
        *y = height + fmod(*y,height);
}

bool graficador_dibujar(const char *nombre, float escala, float x, float y, float angulo)
{
    sprite_t *s = lista_buscar(ls, nombre, sprite_comparar_nombre);
    if(s == NULL)
        return false;

    vector_rotar(s->coords, s->n, angulo);
    for(size_t i = 0; i < (s->n) - 1; i++)
    {
			SDL_RenderDrawLine(
				renderer,
				(s->coords)[i][0] * escala + x,
				-(s->coords)[i][1] * escala - y + h,
				(s->coords)[i+1][0] * escala + x,
				-(s->coords)[i+1][1] * escala - y + h
			);
    }
    vector_rotar(s->coords, s->n, -angulo);

    return true;
}
