// Práctica 03/Montecarlo - Laura Lucia Jaramillo Yah - 09/09/2026

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_PUNTOS 1000000
#define NUM_HILOS 16 // Se hace el cambio manual a 8 y 16 para las pruebas

// Guarda los puntos que cayeron dentro del círculo
static int puntosDentro = 0;

// Protege el contador compartido
sem_t semaforo;

// Cada hilo genera una parte de los puntos
static void *generarPuntos(void *arg)
{
    int numeroHilo = *((int *)arg);

    // Cada hilo utiliza una semilla diferente
    unsigned int semilla = (unsigned int)time(NULL) + numeroHilo;

    // Reparte el total de puntos entre los hilos
    int cantidad = TOTAL_PUNTOS / NUM_HILOS;

    // Distribuye los puntos sobrantes
    if (numeroHilo <= TOTAL_PUNTOS % NUM_HILOS)
    {
        cantidad++;
    }

    for (int i = 0; i < cantidad; i++)
    {
        // Genera coordenadas entre -1 y 1
        double x = ((double)rand_r(&semilla) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&semilla) / RAND_MAX) * 2.0 - 1.0;

        // Comprueba si el punto está dentro del círculo
        if (x * x + y * y <= 1.0)
        {
            // Solo un hilo puede modificar el contador
            sem_wait(&semaforo);
            puntosDentro++;
            sem_post(&semaforo);
        }
    }

    return NULL;
}

int main(void)
{
    pthread_t hilos[NUM_HILOS];
    int numeros[NUM_HILOS];

    struct timespec inicio, fin;

    // Inicializa el semáforo con valor 1
    sem_init(&semaforo, 0, 1);

    // Inicia la medición del tiempo
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Crea los hilos
    for (int i = 0; i < NUM_HILOS; i++)
    {
        numeros[i] = i + 1;
        pthread_create(&hilos[i], NULL, generarPuntos, &numeros[i]);
    }

    // Espera a que todos los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    // Finaliza la medición del tiempo
    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo = (fin.tv_sec - inicio.tv_sec)
                  + (fin.tv_nsec - inicio.tv_nsec) / 1000000000.0;

    // Calcula el valor aproximado de Pi
    double pi = 4.0 * puntosDentro / TOTAL_PUNTOS;

    printf("Numero de hilos: %d\n", NUM_HILOS);
    printf("Total de puntos: %d\n", TOTAL_PUNTOS);
    printf("Puntos dentro del circulo: %d\n", puntosDentro);
    printf("Valor aproximado de Pi: %.6f\n", pi);
    printf("Tiempo de ejecucion: %.6f segundos\n", tiempo);

    // Libera los recursos del semáforo
    sem_destroy(&semaforo);

    return 0;
}
