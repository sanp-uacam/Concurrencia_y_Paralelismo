/* Práctica 04/Montecarlo - Gladys Candy Chumá Pérez - 09/09/2026 
   Equipo 5: 1.Gladys Candy Chumá Pérez
            2.Celso Alberto Gutierrez Estrella
            3.Laura Lucia Jaramillo Yah
            4.Christopher Martinez Huicab
            5.José Castillo Dzib
            6.Jordi H Gomez Gongora
*/  

#define _POSIX_C_SOURCE 200809L

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_PUNTOS 100000000
#define NUM_HILOS 16 // Se hace el cambio manual a 2, 4, 8 o 16

// Contador atómico compartido por los hilos.
static atomic_int puntosDentro = 0;

// Función que ejecutan los hilos.
static void *generarPuntos(void *arg);

// Incrementa el contador usando Compare and Swap.
static void incrementarConCAS(void)
{
    int esperado = atomic_load(&puntosDentro);

    // Si otro hilo cambió el contador, vuelve a intentarlo.
    while (!atomic_compare_exchange_weak(
        &puntosDentro,
        &esperado,
        esperado + 1
    ))
    {
    }
}

// Genera puntos aleatorios y comprueba si están dentro del círculo.
static void *generarPuntos(void *arg)
{
    int numeroHilo = *((int *)arg);

    // Cada hilo tiene una semilla diferente.
    unsigned int semilla =
        (unsigned int)time(NULL) + numeroHilo;

    // Cada hilo genera una parte de los puntos.
    int cantidad = TOTAL_PUNTOS / NUM_HILOS;

    // Reparte los puntos sobrantes.
    if (numeroHilo <= TOTAL_PUNTOS % NUM_HILOS)
    {
        cantidad++;
    }

    for (int i = 0; i < cantidad; i++)
    {
        // Genera las coordenadas x e y entre -1 y 1.
        double x =
            ((double)rand_r(&semilla) / RAND_MAX) * 2.0 - 1.0;

        double y =
            ((double)rand_r(&semilla) / RAND_MAX) * 2.0 - 1.0;

        // Comprueba si el punto cayó dentro del círculo.
        if (x * x + y * y <= 1.0)
        {
            incrementarConCAS();
        }
    }

    return NULL;
}

int main(void)
{
    // Guarda los hilos y sus números.
    pthread_t hilos[NUM_HILOS];
    int numeros[NUM_HILOS];

    struct timespec inicio, fin;

    // Comienza la medición antes de crear los hilos.
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Crea los hilos.
    for (int i = 0; i < NUM_HILOS; i++)
    {
        numeros[i] = i + 1;

        pthread_create(
            &hilos[i],
            NULL,
            generarPuntos,
            &numeros[i]
        );
    }

    // Espera a que todos los hilos terminen.
    for (int i = 0; i < NUM_HILOS; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    // Termina la medición después de finalizar los hilos.
    clock_gettime(CLOCK_MONOTONIC, &fin);

    // Convierte el tiempo medido a segundos.
    double tiempo =
        (fin.tv_sec - inicio.tv_sec) +
        (fin.tv_nsec - inicio.tv_nsec) / 1000000000.0;

    // Obtiene el resultado final del contador atómico.
    int resultado = atomic_load(&puntosDentro);

    // Calcula la aproximación de Pi.
    double pi =
        4.0 * resultado / TOTAL_PUNTOS;

    printf("Numero de hilos: %d\n", NUM_HILOS);
    printf("Total de puntos: %d\n", TOTAL_PUNTOS);
    printf("Puntos dentro del circulo: %d\n", resultado);
    printf("Valor aproximado de Pi: %f\n", pi);
    printf("Tiempo de ejecucion: %f segundos\n", tiempo);

    return 0;
}