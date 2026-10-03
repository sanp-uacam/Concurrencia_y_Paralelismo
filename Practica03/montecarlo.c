/*
    Calculo de PI con el metodo de Monte Carlo – Autor: Victor Adrian Romero Minaya – 2026/09/07
    El programa aproxima el valor de PI generando puntos aleatorios (x, y)
    dentro de un cuadrado de lado 2 (de -1 a 1), y contando cuantos de esos
    puntos caen dentro del circulo unitario inscrito (x*x + y*y <= 1).
    El trabajo se reparte entre varios hilos, y se utiliza un SEMAFORO para
    proteger el acceso a la variable compartida puntosDentro.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int totalPuntos = 1000000;
int numHilos = 2;
int puntosDentro = 0;

sem_t sem;

void *monteCarlo(void *arg)
{
    int puntosPorHilo = totalPuntos / numHilos;
    int puntosLocal = 0;
    double x, y;

    for (int i = 0; i < puntosPorHilo; i++)
    {
        x = ((double)rand() / RAND_MAX) * 2 - 1;
        y = ((double)rand() / RAND_MAX) * 2 - 1;

        if ((x * x) + (y * y) <= 1)
        {
            puntosLocal++; // el punto cae dentro del circulo
        }
    }

    sem_wait(&sem);
    puntosDentro += puntosLocal;
    sem_post(&sem);

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        numHilos = atoi(argv[1]);
    }

    pthread_t hilos[numHilos];
    srand(time(NULL));
    
    sem_init(&sem, 0, 1);

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio); // tiempo justo antes de crear los hilos

    for (int i = 0; i < numHilos; i++)
    {
        pthread_create(&hilos[i], NULL, monteCarlo, NULL);
    }

    for (int i = 0; i < numHilos; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin); // tiempo justo despues de que todos los hilos terminaron

    double tiempo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;
    double pi = 4.0 * puntosDentro / totalPuntos;

    printf("-----------------------------\n");
    printf("Metodo Monte Carlo con semaforos\n");
    printf("-----------------------------\n");
    printf("Numero de hilos: %d\n", numHilos);
    printf("Puntos totales: %d\n", totalPuntos);
    printf("Puntos dentro del circulo: %d\n", puntosDentro);
    printf("Valor aproximado de PI: %lf\n", pi);
    printf("Tiempo de ejecucion: %f segundos\n", tiempo);

    sem_destroy(&sem);

    return 0;
}
