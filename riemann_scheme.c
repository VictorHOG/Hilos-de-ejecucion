/**
* FILE: riemann_scheme.c
* DESCRIPTION:
* 	Implementacion de la suma de Riemann para calcular el area bajo la
*	curva.
* AUTHOR: John Sanabria - john.sanabria@correounivalle.edu.co
* MODIFICADO POR:Victor Hugo Ortega Gomez - 1532342
* LAST REVISED: 06/05/17
* REFERENCES: http://mathinsight.org/calculating_area_under_curve_riemann_sums
******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define MAXRECT 1000000
#define MAXTHREAD 10
#define BASE 0.0
#define LIMIT 1.0

double length = LIMIT - BASE; // La longitud en el eje 'x'
double numRectxThread = (double)(MAXRECT/MAXTHREAD); // Cuantos rectangulos por hilo
double base_length = 0; // Variable que tiene el ancho de cada rectangulo
double sumTotal = 0; // Variable que tendra la suma total
double partialSums[MAXTHREAD]; // Variable que tiene la suma parcial de cada hilo

// Funcion a la que se le calculara la integral, x^2
double function(double x) {
	return x * x;
}

// Funcion que divide el intervalo [BASE, LIMIT] en MAXTHREAD subintervalos y calculas las
// sumas parciales en cada subintervalo, por medio de los hilos de ejecucion. 
void* calcular (void *arg) {

	int id = 0;
	double loslimit = 0.0;
	double limitCondition = 0.0;
	double val = 0.0;

	id = *((int*) arg); // Identificador del hilo, determina que porción del
			    // rectangulo le toca procesar a este hilo.
	partialSums[id] = 0; // Inicializacion de la posicion del vector que le corresponde a este hilo.
	loslimit = id * numRectxThread; // Posicion del eje 'x' donde se empieza a calcular el area
					// bajo la curva de los rectangulos.
	limitCondition = (id + 1) * numRectxThread; // Posicion del eje 'x' donde se termina el calculo
						    // del area bajo la curva de los rectangulos.
	base_length = length/MAXRECT;


	// Ciclo que recorre el segmento de 'x' que le  corresponde a este hilo
	// y donde calcula el area de los rectangulos que se encuentran en este segmento.
	for (double i = loslimit; i < limitCondition ; i++) {

		// Extremo derecho del rectangulo i
		val = BASE + i * base_length;
		// Sumando el area de cada rectangulo.
		// El area de cada rectangulo se calcula con:
       		// 'function(val) * base_length'
		partialSums[id] = partialSums[id] + function(val) * base_length;
	}

	printf("sum: %lf\n", partialSums[id]);
	pthread_exit(NULL);
}

// Funcion principal.
int main(int argc, char** argv) {

	pthread_t threads[MAXTHREAD]; // Vector usado para almacenar los identificadores de los hilos creados.
	int taskids[MAXTHREAD]; // Vector usado para almacenar los valores de los identificadores de los hilos
				// que serviran para determinar que bloque de rectangulos se va a procesar.

	base_length = length/MAXRECT; // esta variable contiene el ancho de
				      // cada base de cada rectangulo.

	printf("base length: %lf numRectxThread: %lf\n",base_length, numRectxThread);

	// Creacion de los hilos que calcularan el area bajo la curva
	for (int i = 0; i < MAXTHREAD; i++) {

		taskids[i] = i;
		printf("Main: creando el hilo %d \n", i);
		pthread_create(&threads[i], NULL, calcular, &taskids[i] );
	}

	// Ciclo donde se espera el resultado de cada hilo
	for (int i = 0; i < MAXTHREAD; i++) {

		// Espera por la terminacion del hilo de indice i
		pthread_join(threads[i], NULL);
		printf( "Main: hilo %d ha terminado. \n", i );
		sumTotal += partialSums[i];
	}

	printf( "Main: Todos los hilos terminaron satisfactoriamente. \n" );
	printf("Suma total %lf\n",sumTotal);
	pthread_exit(NULL);
}
