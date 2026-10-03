
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_DE_PUNTOS 100000000

long puntosDentro = 0;
sem_t semaforo;

typedef struct {
    long puntos;
    unsigned int semilla;
} DatosHilo;

void *calcularPuntos(void *arg)
{
    DatosHilo *datos = (DatosHilo *)arg;

    for (long i = 0; i < datos->puntos; i++) {

        double x = ((double)rand_r(&datos->semilla) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&datos->semilla) / RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {

            sem_wait(&semaforo);

            puntosDentro++;

            sem_post(&semaforo);
        }
    }

    return NULL;
}

double ejecutarPrueba(int numeroHilos)
{
    pthread_t *hilos;
    DatosHilo *datos;

    hilos = malloc(numeroHilos * sizeof(pthread_t));
    datos = malloc(numeroHilos * sizeof(DatosHilo));

    if (hilos == NULL || datos == NULL) {
        printf("Error de memoria\n");
        exit(1);
    }

    sem_init(&semaforo, 0, 1);

    puntosDentro = 0;

    long puntosPorHilo = TOTAL_DE_PUNTOS / numeroHilos;
    long sobrantes = TOTAL_DE_PUNTOS % numeroHilos;

    struct timespec inicio, fin;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < numeroHilos; i++) {

        datos[i].puntos =
            puntosPorHilo + (i < sobrantes ? 1 : 0);

        datos[i].semilla =
            (unsigned int)time(NULL) +
            i * 1234567 +
            numeroHilos * 98765;

        pthread_create(
            &hilos[i],
            NULL,
            calcularPuntos,
            &datos[i]
        );
    }

    for (int i = 0; i < numeroHilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo =
        (fin.tv_sec - inicio.tv_sec) +
        (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi =
        4.0 * (double)puntosDentro / TOTAL_DE_PUNTOS;

    /*
     * Mostrar informacion de esta ejecucion
     */
    printf("\n");
    printf("Hilos: %d\n", numeroHilos);
    printf("Puntos: %d\n", TOTAL_DE_PUNTOS);
    printf("Puntos dentro: %ld\n", puntosDentro);
    printf("PI aproximado: %.10f\n", pi);
    printf("Tiempo: %.6f segundos\n", tiempo);

    sem_destroy(&semaforo);

    free(hilos);
    free(datos);

    return tiempo;
}

int main()
{
    double tiempos[3];

    /*
     * Prueba con 1, 4 y 8 hilos
     */
    tiempos[0] = ejecutarPrueba(1);
    tiempos[1] = ejecutarPrueba(4);
    tiempos[2] = ejecutarPrueba(8);

    /*
     * Tiempo con 1 hilo
     */
    double T1 = tiempos[0];

    int hilos[3] = {1, 4, 8};

    double speedup[3];
    double eficiencia[3];
    double overhead[3];

    /*
     * Calcular metricas
     */
    for (int i = 0; i < 3; i++) {

        speedup[i] = T1 / tiempos[i];

        eficiencia[i] =
            speedup[i] / hilos[i];

        overhead[i] =
            hilos[i] * tiempos[i] - T1;
    }

    /*
     * Mostrar resultados
     */
    printf("\n");
    printf("Resultados\n");
    printf("\n");

    printf("Hilos\tTiempo\t\tSpeedup\t\tEficiencia\tOverhead\n");

    for (int i = 0; i < 3; i++) {

        printf("%d\t%.6f\t%.4f\t\t%.4f\t\t%.6f\n",
               hilos[i],
               tiempos[i],
               speedup[i],
               eficiencia[i],
               overhead[i]);
    }

    return 0;
}

