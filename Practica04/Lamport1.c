/*
Nombre: P3_MonteCarlo_Lamport
Autor: Uriel Alejandro Tun Muñoz
Fecha: 29/09/2026
Descripción: Estimación de Pi por Monte Carlo utilizando el algoritmo
             de la Panadería de Lamport para exclusión mutua.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

#define TOTAL_PUNTOS 100000000

int puntosDentro = 0;
int numHilos;

bool *choosing;
int *number;

int obtener_max_number() {
    int max = 0;
    for (int i = 0; i < numHilos; i++) {
        if (number[i] > max) {
            max = number[i];
        }
    }
    return max;
}

void lock_lamport(int i) {
    choosing[i] = true;
    number[i] = 1 + obtener_max_number();
    choosing[i] = false;

    for (int j = 0; j < numHilos; j++) {
        while (choosing[j]) {
        }

        while (number[j] != 0 && (number[j] < number[i] || (number[j] == number[i] && j < i))) {
        }
    }
}

void unlock_lamport(int i) {
    number[i] = 0;
}

double random_double(unsigned int *seed) {
    *seed = (*seed * 1103515245 + 12345) & 0x7fffffff;
    return ((double)*seed / 2147483647.0) * 2.0 - 1.0;
}

void* funcionHilo(void* arg) {
    int id = *(int*)arg;
    int limite = TOTAL_PUNTOS / numHilos;
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self();

    int puntosLocales = 0;
    for (int i = 0; i < limite; i++) {
        double x = random_double(&seed);
        double y = random_double(&seed);

        if (x * x + y * y <= 1.0) {
            puntosLocales++;
        }
    }

    lock_lamport(id);
    puntosDentro += puntosLocales;
    unlock_lamport(id);

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

    choosing = (bool*)calloc(numHilos, sizeof(bool));
    number = (int*)calloc(numHilos, sizeof(int));
    pthread_t hilos[numHilos];
    int ids[numHilos];

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < numHilos; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, funcionHilo, &ids[i]);
    }

    for (int i = 0; i < numHilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi = 4.0 * (double)puntosDentro / TOTAL_PUNTOS;

    printf("\n=== RESULTADOS (LAMPORT) ===\n");
    printf("Hilos: %d\n", numHilos);
    printf("Puntos Totales: %d\n", TOTAL_PUNTOS);
    printf("Puntos Dentro: %d\n", puntosDentro);
    printf("Valor de Pi: %f\n", pi);
    printf("Tiempo de ejecucion: %f segundos\n", tiempo);

    free(choosing);
    free(number);
    return 0;
}