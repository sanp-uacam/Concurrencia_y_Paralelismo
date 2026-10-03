//METODO DE MONTE CARLO CON MUTEX

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TOTAL_PUNTOS 1000000
#define NUM_HILOS 4

int puntosDentro = 0;  

pthread_mutex_t lock;

typedef struct {
    int id;
} DatosHilo;

// Generador de números aleatorios propio (thread-safe, no depende de rand())
unsigned int siguiente_random(unsigned int *semilla)
{
    *semilla = (*semilla) * 1103515245 + 12345;
    return (*semilla / 65536) % 32768;
}

void* calcular(void* arg)
{
    DatosHilo* datos = (DatosHilo*) arg;
    unsigned int semilla = (unsigned int)time(NULL) ^ (datos->id * 7919);

    int puntosPorHilo = TOTAL_PUNTOS / NUM_HILOS;
    int puntosLocales = 0;

    for (int i = 0; i < puntosPorHilo; i++)
    {
        double x = ((double)siguiente_random(&semilla) / 32768.0) * 2.0 - 1.0;
        double y = ((double)siguiente_random(&semilla) / 32768.0) * 2.0 - 1.0;

        if (x * x + y * y <= 1.0)
        {
            puntosLocales++;
        }
    }

    pthread_mutex_lock(&lock);
    puntosDentro += puntosLocales;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main()
{
    pthread_t hilos[NUM_HILOS];
    DatosHilo datos[NUM_HILOS];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < NUM_HILOS; i++)
    {
        datos[i].id = i;
        pthread_create(&hilos[i], NULL, calcular, &datos[i]);
    }

    for (int i = 0; i < NUM_HILOS; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    double pi = 4.0 * puntosDentro / TOTAL_PUNTOS;

    printf("Total de puntos: %d\n", TOTAL_PUNTOS);
    printf("Puntos dentro del circulo: %d\n", puntosDentro);
    printf("Valor aproximado de PI: %.6f\n", pi);

    pthread_mutex_destroy(&lock);

    return 0;
}