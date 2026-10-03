/*
Nombre: P2_MonteCarlo_Pi
Autor: Uriel Alejandro Tun Muñoz
Fecha: 29/09/2026
Descripción: Estimación de Pi por Monte Carlo distribuyendo la carga
             en hilos y sincronizando la variable compartida con semáforos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TOTAL_PUNTOS 1000000

int puntosDentro = 0;
int numHilos;
sem_t lock;

// Generador aleatorio simple y seguro por hilo (LCG)
double random_double(unsigned int *seed) {
    *seed = (*seed * 1103515245 + 12345) & 0x7fffffff;
    return ((double)*seed / 2147483647.0) * 2.0 - 1.0;
}

void* funcionHilo(void* arg) {
    int limite = TOTAL_PUNTOS / numHilos;
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self();

    for (int i = 0; i < limite; i++) {
        double x = random_double(&seed);
        double y = random_double(&seed);

        if (x * x + y * y <= 1.0) {
            sem_wait(&lock);
            puntosDentro++;
            sem_post(&lock);
        }
    }
    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <numHilos>\n", argv[0]);
        return 1;
    }

    numHilos = atoi(argv[1]);
    if (numHilos <= 0) {
        printf("El número de hilos debe ser mayor a 0.\n");
        return 1;
    }

    pthread_t hilos[numHilos];

    sem_init(&lock, 0, 1);

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < numHilos; i++) {
        pthread_create(&hilos[i], NULL, funcionHilo, NULL);
    }

    for (int i = 0; i < numHilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi = 4.0 * (double)puntosDentro / TOTAL_PUNTOS;

    printf("\n=== RESULTADOS ===\n");
    printf("Hilos: %d\n", numHilos);
    printf("Puntos Totales: %d\n", TOTAL_PUNTOS);
    printf("Puntos Dentro: %d\n", puntosDentro);
    printf("Valor de Pi: %f\n", pi);
    printf("Tiempo de ejecucion: %f segundos\n", tiempo);

    sem_destroy(&lock);
    return 0;
}