/*
 * Programa: Metodo Monte Carlo para calcular PI
 * Algoritmo de exclusion mutua: Lamport (Bakery Algorithm)
 * Basado en el programa de la practica 3 para calcular pi
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define TOTAL_DE_PUNTOS 100000LL
#define MAX_HILOS 16

/* Variable compartida protegida exclusivamente por Lamport */
long long puntosDentro = 0;

/* Variables del algoritmo de Lamport */
volatile int eligiendo[MAX_HILOS];
volatile int numero[MAX_HILOS];

int numeroHilos;

/* Entrada a la seccion critica: algoritmo de la panaderia de Lamport */
void entrarSeccionCritica(int id)
{
    int maximo = 0;

    eligiendo[id] = 1;

    for (int i = 0; i < numeroHilos; i++) {
        if (numero[i] > maximo) {
            maximo = numero[i];
        }
    }

    numero[id] = maximo + 1;
    eligiendo[id] = 0;

    for (int i = 0; i < numeroHilos; i++) {
        if (i == id) {
            continue;
        }

        while (eligiendo[i]) {
           
        }

        while (numero[i] != 0 &&
               (numero[i] < numero[id] ||
                (numero[i] == numero[id] && i < id))) {
            
        }
    }
}

/* Salida de la seccion critica */
void salirSeccionCritica(int id)
{
    numero[id] = 0;
}

void *calcularPuntos(void *arg)
{
    int id = *(int *)arg;
    long long puntosPorHilo = TOTAL_DE_PUNTOS / numeroHilos;
    long long puntosExtra = TOTAL_DE_PUNTOS % numeroHilos;

    /* rand_r evita compartir el estado global de rand() entre hilos. */
    unsigned int semilla = (unsigned int)(time(NULL) ^ (id * 2654435761u));

    /* Reparto exacto: los primeros hilos reciben un punto adicional. */
    if (id < puntosExtra) {
        puntosPorHilo++;
    }

    for (long long i = 0; i < puntosPorHilo; i++) {
        double x = ((double)rand_r(&semilla) / (double)RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&semilla) / (double)RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            entrarSeccionCritica(id);
            puntosDentro++;
            salirSeccionCritica(id);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t hilos[MAX_HILOS];
    int ids[MAX_HILOS];
    struct timespec inicio, fin;

    numeroHilos = 4;

    if (argc >= 2) {
        numeroHilos = atoi(argv[1]);
    }

    if (numeroHilos != 2 && numeroHilos != 4 &&
        numeroHilos != 8 && numeroHilos != 16) {
        fprintf(stderr, "Uso: %s [2|4|8|16]\n", argv[0]);
        return EXIT_FAILURE;
    }

    puntosDentro = 0;

    for (int i = 0; i < MAX_HILOS; i++) {
        eligiendo[i] = 0;
        numero[i] = 0;
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < numeroHilos; i++) {
        ids[i] = i;
        if (pthread_create(&hilos[i], NULL, calcularPuntos, &ids[i]) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < numeroHilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo = (double)(fin.tv_sec - inicio.tv_sec) +
                    (double)(fin.tv_nsec - inicio.tv_nsec) / 1000000000.0;
    double pi = 4.0 * (double)puntosDentro / (double)TOTAL_DE_PUNTOS;

    printf("Hilos: %d\n", numeroHilos);
    printf("Puntos: %lld\n", TOTAL_DE_PUNTOS);
    printf("Puntos dentro: %lld\n", puntosDentro);
    printf("PI aproximado: %.10f\n", pi);
    printf("Tiempo: %.6f segundos\n", tiempo);

    return EXIT_SUCCESS;
}
