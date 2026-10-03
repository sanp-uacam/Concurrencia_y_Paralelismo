#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>


#define TOTAL_PUNTOS 100000000
#define NUM_HILOS 2

int puntosDentro = 0;

sem_t semaforo;

void *monteCarlo(void *arg)
{
    for (int i = 0; i < (TOTAL_PUNTOS / NUM_HILOS); i++)
    {
        double x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y <= 1.0)
        {
            sem_wait(&semaforo);

            puntosDentro++;

            sem_post(&semaforo);
        }
    }

    return NULL;
}

int main(void){
clock_t inicio = clock();

pthread_t hilo1, hilo2;

    sem_init(&semaforo, 0, 1);

    pthread_create(&hilo1, NULL, monteCarlo, NULL);
    pthread_create(&hilo2, NULL, monteCarlo, NULL);


    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    double pi = 4.0 * puntosDentro / TOTAL_PUNTOS;

    printf("Puntos totales: %d\n", TOTAL_PUNTOS);
    printf("Puntos dentro: %d\n", puntosDentro);
    printf("Aproximacion de Pi: %f\n", pi);


    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;

    printf("Tiempo de CPU: %f segundos\n",tiempo);

    sem_destroy(&semaforo);

    return 0;
}