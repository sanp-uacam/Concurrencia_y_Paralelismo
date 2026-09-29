/*
  Programa: Calculo de PI con Monte Carlo usando hilos y semaforo
  Autor: Joaquin Alberto de la Cruz Morales Fuentes
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_DE_PUNTOS 1000000

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
            // Seccion critica: protege puntosDentro
            sem_wait(&semaforo);
            puntosDentro++;
            sem_post(&semaforo);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Uso: %s <numero_de_hilos>\n", argv[0]);
        printf("Ejemplo: %s 8\n", argv[0]);
        return 1;
    }

    int numeroHilos = atoi(argv[1]);

    if (numeroHilos <= 0 || numeroHilos > TOTAL_DE_PUNTOS) {
        printf("El numero de hilos debe ser mayor que 0.\n");
        return 1;
    }

    pthread_t *hilos = malloc(numeroHilos * sizeof(pthread_t));
    DatosHilo *datos = malloc(numeroHilos * sizeof(DatosHilo));

    if (hilos == NULL || datos == NULL) {
        printf("Error al reservar memoria.\n");
        free(hilos);
        free(datos);
        return 1;
    }

    if (sem_init(&semaforo, 0, 1) != 0) {
        perror("Error al inicializar el semaforo");
        free(hilos);
        free(datos);
        return 1;
    }

    puntosDentro = 0;

    // Para que las pruebas sean reproducibles en cantidad de puntos,
    // cada hilo recibe una cantidad; el sobrante se asigna a los primeros hilos.
    long puntosPorHilo = TOTAL_DE_PUNTOS / numeroHilos;
    long sobrantes = TOTAL_DE_PUNTOS % numeroHilos;

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < numeroHilos; i++) {
        datos[i].puntos = puntosPorHilo + (i < sobrantes ? 1 : 0);
        datos[i].semilla = (unsigned int)time(NULL) ^ (unsigned int)(i * 1234567 + numeroHilos * 98765);

        if (pthread_create(&hilos[i], NULL, calcularPuntos, &datos[i]) != 0) {
            perror("Error al crear hilo");
            sem_destroy(&semaforo);
            free(hilos);
            free(datos);
            return 1;
        }
    }

    for (int i = 0; i < numeroHilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo = (fin.tv_sec - inicio.tv_sec)
                  + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi = 4.0 * (double)puntosDentro / TOTAL_DE_PUNTOS;

    printf("Hilos: %d\n", numeroHilos);
    printf("Puntos totales: %d\n", TOTAL_DE_PUNTOS);
    printf("Puntos dentro: %ld\n", puntosDentro);
    printf("PI aproximado: %.10f\n", pi);
    printf("Tiempo: %.6f segundos\n", tiempo);

    sem_destroy(&semaforo);
    free(hilos);
    free(datos);

    return 0;
}
