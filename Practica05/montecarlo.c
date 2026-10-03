/*
    Medicion de Speedup, Eficiencia y Overhead en Monte Carlo - Autor: Victor Adrian Romero Minaya - 2026/09/30
    El programa aproxima el valor de PI con el metodo de Monte Carlo, generando
    puntos aleatorios (x, y) dentro de un cuadrado de lado 2 (de -1 a 1) y
    contando cuantos caen dentro del circulo unitario inscrito (x*x + y*y <= 1).
    El trabajo se reparte en partes iguales entre varios hilos (pthreads) y un
    SEMAFORO protege el acceso a la variable compartida puntosDentro.
    Para la practica se mide el tiempo de ejecucion con clock_gettime
    (CLOCK_MONOTONIC) usando 1, 4 y 8 hilos con la misma cantidad de puntos,
    y con esos tiempos T(p) se calculan:
        Speedup:    S(p) = T(1) / T(p)
        Eficiencia: E(p) = S(p) / p
        Overhead:   O(p) = p * T(p) - T(1)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int totalPuntos = 100000000;
int numHilos = 2;
int puntosDentro = 0;

sem_t sem;

void *monteCarlo(void *arg)
{
    unsigned int semilla = *(unsigned int *)arg;
    int puntosPorHilo = totalPuntos / numHilos;
    int puntosLocal = 0;
    double x, y;

    for (int i = 0; i < puntosPorHilo; i++)
    {
        x = ((double)rand_r(&semilla) / RAND_MAX) * 2 - 1;
        y = ((double)rand_r(&semilla) / RAND_MAX) * 2 - 1;

        if ((x * x) + (y * y) <= 1)
        {
            puntosLocal++;
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
    if (argc > 2)
    {
        totalPuntos = atoi(argv[2]);
    }

    pthread_t hilos[numHilos];
    unsigned int semillas[numHilos];

    sem_init(&sem, 0, 1);

    for (int i = 0; i < numHilos; i++)
    {
        semillas[i] = (unsigned int)time(NULL) + 7919u * i;
    }

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < numHilos; i++)
    {
        pthread_create(&hilos[i], NULL, monteCarlo, &semillas[i]);
    }

    for (int i = 0; i < numHilos; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

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
