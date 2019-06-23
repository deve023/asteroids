#include "lista.h"

struct lista {
	struct nodo *prim;
};

struct lista_iterador {
	lista_t *l;
	struct nodo *n;
	struct nodo *ant;
};

typedef struct nodo {
	struct nodo *sig;
	void *dato;
} nodo_t;

static nodo_t *nodo_crear(void *d) {
	nodo_t *n = malloc(sizeof(nodo_t));
	if(n == NULL)
		return NULL;

	n->sig = NULL;
	n->dato = d;

	return n;
}

lista_t *lista_crear() {
	lista_t *l = malloc(sizeof(lista_t));
	if(l == NULL)
		return NULL;

	l->prim = NULL;
	return l;
}


void lista_destruir(lista_t *l, void (*destruir_dato)(void *d)) {
	nodo_t *n = l->prim;
	while(n != NULL) {
		nodo_t *sig = n->sig;

		if(destruir_dato != NULL)
			destruir_dato(n->dato);

		free(n);

		n = sig;
	}

	free(l);
}

bool lista_es_vacia(const lista_t *l) {
	return l->prim == NULL;
}

bool lista_insertar_comienzo(lista_t *l, void *d) {
	nodo_t *n = nodo_crear(d);
	if(n == NULL)
		return false;

	n->sig = l->prim;
	l->prim = n;

	return true;
}

bool lista_insertar_final(lista_t *l, void *d) {
	nodo_t *n = nodo_crear(d);
	if(n == NULL)
		return false;

	// Si está vacía inserto al principio:
	if(l->prim == NULL) {
		l->prim = n;
		return true;
	}

	// La lista no estaba vacía:
	nodo_t *aux = l->prim;
	while(aux->sig != NULL)
		aux = aux->sig;

	aux->sig = n;

	return true;
}

void *lista_extraer_primero(lista_t *l) {
	if(lista_es_vacia(l))
		return NULL;

	nodo_t *n = l->prim;
	void *d = n->dato;

	l->prim = n->sig;
	free(n);

	return d;
}

void * lista_extraer_ultimo(lista_t * l)
{
	//Si esta vacia devuelvo NULL
	if(lista_es_vacia(l))
		return NULL;

	//Si tiene un solo elemento extraigo ese
	if(l->prim->sig == NULL)
	{
		void * d = l->prim->dato;
		free(l->prim);
		l->prim = NULL;
		return d;
	}

	//Si tiene mas de un elemento busco el anteultimo
	nodo_t * aux = l->prim;
	while(aux->sig->sig != NULL)
		aux = aux->sig;

	//Extraigo el ultimo elemento
	void * d = aux->sig->dato;
	free(aux->sig);
	aux->sig = NULL;

	return d;
}


void *lista_buscar(const lista_t *l, const void *d, int (*cmp)(const void *a, const void *b)) {

	nodo_t *n = l->prim;
	while(n != NULL) {
		if(cmp(n->dato, d) == 0)
			return n->dato;

		n = n->sig;
	}

	return NULL;
}

/*void *lista_borrar(lista_t *l, const void *d, int (*cmp)(const void *a, const void *b)) {

	if(lista_es_vacia(l))
		return NULL;

	// Si es el primero
	nodo_t *n = l->prim;
	if(cmp(n->dato, d) == 0)
		return lista_extraer_primero(l);

	// Si esta despues
	n = n->sig;
	nodo_t *ant;
	while(n != NULL) {
		if(cmp(n->dato, d) == 0)
			break;
		ant = n;
		n = n->sig;
	}

	if(n == NULL)
		return NULL;

	void *dato = n->dato;

	ant->sig = n->sig;
	free(n);

	return dato;
}*/

void *lista_borrar(lista_t *l, const void *d, int (*cmp)(const void *a, const void *b)) {
    // Retiramos ocurrencias al comienzo.
    while(l->prim && cmp(l->prim->dato, d) == 0) {
        struct nodo *aux = l->prim->sig;
        free(l->prim);
        l->prim = aux;
    }

    // Verificamos que quede al menos un elemento.
    if(l->prim == NULL)
        return (void*)d;

    // Retiramos de resto.
    struct nodo *ant = l->prim;
    while(ant->sig) {
        struct nodo *actual = ant->sig;
        if(cmp(actual->dato, d) == 0) {
            ant->sig = actual->sig;
            free(actual);
        }
        else
            ant = actual;
    }

    return (void*)d;
}

void lista_recorrer(const lista_t *l, bool (*visitar)(void *dato, void *extra), void *extra) {

	nodo_t *n = l->prim;
	while(n != NULL) {
		if(!visitar(n->dato, extra))
			return;
		n = n->sig;
	}
}

void lista_mapear(lista_t *l, void *(*f)(void *dato, void *extra), void *extra) {

	nodo_t *n = l->prim;
	while(n != NULL) {
		n->dato = f(n->dato, extra);
		n = n->sig;
	}
}

lista_t *lista_filtrar(lista_t *l, bool (*f)(void *dato, void *extra), void *extra){

	lista_t * nl = lista_crear();
	if(nl == NULL)
		return NULL;

	nodo_t *aux = l->prim;

	//Miro los primeros elementos

	while(aux != NULL && f(aux->dato, extra)) {
		void * d = lista_extraer_primero(l);
		lista_insertar_final(nl, d);
		aux = l->prim;
	}

	//Chequeo si ya se termino la lista
	if(aux == NULL)
		return nl;

	//Avanzo y miro el resto de la lista
	nodo_t *ant = aux;
	aux = aux->sig;

    while(aux != NULL) {

        if(f(aux->dato, extra)) {

			lista_insertar_final(nl, aux->dato);

			ant->sig = aux->sig;
			free(aux);

			aux = ant->sig;
		}
		else {
			ant = aux;
			aux = aux->sig;
		}
	}

	return nl;
}

bool lista_extender(lista_t *a, const lista_t *b) {

	nodo_t *aux = b->prim;
	while(aux != NULL) {
		if(!lista_insertar_final(a, aux->dato))
			return false;

		aux = aux->sig;
	}

	return true;
}

void **lista_a_vector(const lista_t *l, size_t *n) {
	void **v = NULL;
	size_t tam = 0;

	nodo_t *nodo = l->prim;
	while(nodo != NULL) {
		void **aux_v = realloc(v, (++tam) * sizeof(void*));
		if(aux_v == NULL){
			free(v);
			return NULL;
		}
		v = aux_v;

		v[tam - 1] = nodo->dato;
		nodo = nodo->sig;
	}

	*n = tam;
	return v;
}

lista_iterador_t *lista_iterador_crear(lista_t *l) {
	lista_iterador_t *li = malloc(sizeof(lista_iterador_t));
	if(li == NULL)
		return NULL;

	li->l = l;
	li->n = l->prim;
	li->ant = NULL;

	return li;
}

void lista_iterador_destruir(lista_iterador_t *li) {
	free(li);
}

void *lista_iterador_actual(const lista_iterador_t *li) {

	if(li == NULL)
		return NULL;

	return li->n->dato;
}

bool lista_iterador_siguiente(lista_iterador_t *li) {

	if(li->n == NULL)
		return false;

	/*if(li->n->sig == NULL) {
		li->n = NULL;
		return false;
	}*/ //que problema hay con que sea NULL?

	li->ant = li->n;
	li->n = li->n->sig;

	return true;
}

bool lista_iterador_termino(const lista_iterador_t *li) {

	return li->n == NULL;
}

void *lista_iterador_eliminar(lista_iterador_t *li) {

	if(li->n == NULL)
		return NULL;

	if(li->ant == NULL) {
		li->n = li->n->sig;
		return lista_extraer_primero(li->l);
	}

	void *d = li->n->dato;

	li->ant->sig = li->n->sig;
	struct nodo *aux = li->n->sig;
	free(li->n);
	li->n = aux;

	return d;
}

bool lista_iterador_insertar(lista_iterador_t *li, void *dato) {

	if(lista_es_vacia(li->l)) {
		lista_insertar_comienzo(li->l, dato);
		return true;
	}


	if(li->n == NULL)
		return false;

	nodo_t *nn = nodo_crear(dato);
	if(nn == NULL)
		return false;

	nodo_t *aux = li->n->sig;
	li->n->sig = nn;
	nn->sig = aux;

	return true;
}
