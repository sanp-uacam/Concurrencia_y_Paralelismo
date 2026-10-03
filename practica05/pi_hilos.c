#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>

long totalPuntos = 1000000;
int numHilos;
long puntosDentro = 0;
sem_t semaforo;

typedef struct {
    int id;
    long puntosAProcesar;
} ArgsHilo;

void* calcularPi(void* arg) {
    ArgsHilo* args = (ArgsHilo*)arg;
    int id = args->id;
    long n = args->puntosAProcesar;

    // Semilla distinta por hilo
    unsigned int seed = (unsigned int)(time(NULL) ^ (id * 2654435761u));
    long localesDentro = 0;

    for (long i = 0; i < n; i++) {
        double x = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;

        if (x*x + y*y <= 1.0) {
            localesDentro++;
        }
    }

    // Sección crítica: actualizar variable compartida
    sem_wait(&semaforo);
    puntosDentro += localesDentro;
    sem_post(&semaforo);

    free(args);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <numHilos>\n", argv[0]);
        return 1;
    }

    numHilos = atoi(argv[1]);
    if (numHilos <= 0) {
        fprintf(stderr, "numHilos debe ser > 0\n");
        return 1;
    }

    sem_init(&semaforo, 0, 1);

    pthread_t* hilos = malloc(sizeof(pthread_t) * numHilos);
    long base = totalPuntos / numHilos;
    long residuo = totalPuntos % numHilos;

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < numHilos; i++) {
        ArgsHilo* args = malloc(sizeof(ArgsHilo));
        args->id = i;
        args->puntosAProcesar = base + (i < residuo ? 1 : 0);
        pthread_create(&hilos[i], NULL, calcularPi, args);
    }

    for (int i = 0; i < numHilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo = (fin.tv_sec - inicio.tv_sec) +
                    (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi = 4.0 * puntosDentro / totalPuntos;

    printf("Hilos: %d\n", numHilos);
    printf("Puntos dentro: %ld / %ld\n", puntosDentro, totalPuntos);
    printf("Pi aproximado: %.6f\n", pi);
    printf("Error: %.6f\n", fabs(pi - M_PI));
    printf("Tiempo: %.6f segundos\n", tiempo);

    sem_destroy(&semaforo);
    free(hilos);
    return 0;
}