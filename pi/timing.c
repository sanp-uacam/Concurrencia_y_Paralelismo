#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TOTAL_PUNTOS 1000000
#define NUM_HILOS 2

pthread_mutex_t lock;
long puntosDentro = 0;

void* calcularPi(void* arg) {
    long iteraciones = TOTAL_PUNTOS / NUM_HILOS;
    long contadorLocal = 0;

    unsigned int seed = time(NULL) ^ (unsigned long)pthread_self();

    for (long i = 0; i < iteraciones; i++) {
        double x = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;

        if (x*x + y*y <= 1.0) {
            contadorLocal++;
        }
    }

    pthread_mutex_lock(&lock);
    puntosDentro += contadorLocal;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(void) {
    clock_t inicio = clock();

    pthread_t hilo1, hilo2;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&hilo1, NULL, calcularPi, NULL);
    pthread_create(&hilo2, NULL, calcularPi, NULL);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    pthread_mutex_destroy(&lock);

    double pi = 4.0 * puntosDentro / TOTAL_PUNTOS;
    printf("Pi estimado: %f\n", pi);

    clock_t fin = clock();
    double tiempo_transcurrido = (double)(fin - inicio) / CLOCKS_PER_SEC;

    printf("Puntos dentro: %ld\n", puntosDentro);
    printf("Tiempo de CPU: %f segundos\n", tiempo_transcurrido);

    return 0;
}