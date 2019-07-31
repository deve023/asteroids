#ifndef _MATEMATICA_H_
#define _MATEMATICA_H_


/*
Recibe las coordenadas pertenecientes a los puntos p0 = (x0,y0) y p1 = (x1,y1)
y un valor de la coordenada x.
Dada la ecuacion de la recta formada por los puntos p0 y p1, devuelve
el valor de la coordenada 'y' del punto de la recta cuya abscisa es x.
*/
float calcular_y_recta(float x, float x0, float y0, float x1, float y1);

/*
Recibe dos numeros min y max.
Devuelve un numero aleatorio perteneciente al intervalo [min,max].
*/
float randomf(float min, float max);

#endif // _MATEMATICA_H_
