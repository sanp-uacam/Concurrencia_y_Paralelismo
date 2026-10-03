#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define TOTAL_PUNTOS 100000000

int numHilos;
int puntosDentro = 0;
sem_t semaforo;

double ahora(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void *monteCarlo(void *arg)
{
    for (int i = 0; i < (TOTAL_PUNTOS / numHilos); i++)
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

double ejecutar(int n)
{
    numHilos = n;
    puntosDentro = 0;

    pthread_t hilos[n];

    double inicio = ahora();

    for (int i = 0; i < n; i++)
        pthread_create(&hilos[i], NULL, monteCarlo, NULL);

    for (int i = 0; i < n; i++)
        pthread_join(hilos[i], NULL);

    double tiempo = ahora() - inicio;

    printf("Hilos: %d\n", n);
    printf("Puntos dentro: %d\n", puntosDentro);
    printf("Aproximacion de Pi: %f\n", 4.0 * puntosDentro / TOTAL_PUNTOS);
    printf("Tiempo: %f segundos\n\n", tiempo);

    return tiempo;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <num_hilos>\n", argv[0]);
        return 1;
    }

    int p = atoi(argv[1]);

    sem_init(&semaforo, 0, 1);

    printf("Puntos totales: %d\n\n", TOTAL_PUNTOS);

    double T1 = ejecutar(1);

    double Tp = ejecutar(p);

    double speedup    = T1 / Tp;
    double eficiencia = speedup / p;
    double overhead   = p * Tp - T1;

    printf("Medidas de Rendimiento en paralelo\n");
    printf("Tiempo con 1 hilo:  %f segundos\n", T1);
    printf("Tiempo con %d hilos: %f segundos\n", p, Tp);
    printf("Speedup: %f\n", speedup);
    printf("Eficiencia: %f%%\n", eficiencia * 100);
    printf("Overhead: %f segundos\n", overhead);
    

    sem_destroy(&semaforo);

    return 0;
}