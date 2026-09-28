#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int totalPuntos = 1000000;  
int numHilos = 6;           
int puntosDentro = 0; 
int puntosFuera = 0;  
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Generador de números aleatorios thread-safe portátil [-1.0, 1.0]
double aleatorio_hilo(unsigned int *semilla) {
    *semilla = *semilla * 1103515245 + 12345;
    unsigned int val = (*semilla / 65536) % 32768;
    return ((double)val / 32767.0) * 2.0 - 1.0;
}

void* calcularArroz(void* arg) {
    int id = *(int*)arg;
    int puntosPorHilo = totalPuntos / numHilos;

    // Semilla única por hilo basada en el tiempo y su identificador
    unsigned int semilla = (unsigned int)time(NULL) ^ (id * 32145);

    for (int i = 0; i < puntosPorHilo; i++) {
        double x = aleatorio_hilo(&semilla);
        double y = aleatorio_hilo(&semilla);

        if (x * x + y * y <= 1.0) {
            pthread_mutex_lock(&mutex);  
            puntosDentro++;             
            pthread_mutex_unlock(&mutex); 
        } else {
            pthread_mutex_lock(&mutex);
            puntosFuera++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main(void) {
    clock_t inicio = clock();
    pthread_t hilos[6];
    int ids[6];

    for (int i = 0; i < 6; i++) {
        ids[i] = i + 1;
        pthread_create(&hilos[i], NULL, calcularArroz, &ids[i]);
    }

    for (int i = 0; i < 6; i++) {
        pthread_join(hilos[i], NULL);
    }

    double pi = (double)puntosDentro / totalPuntos * 4;

    printf("Puntos dentro: %d\n", puntosDentro);
    printf("Puntos fuera: %d\n", puntosFuera);
    printf("Total: %d\n", puntosDentro + puntosFuera);
    printf("Numero aproximado a PI: %f\n", pi);

    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de CPU: %f segundos\n", tiempo);

    return 0;
}