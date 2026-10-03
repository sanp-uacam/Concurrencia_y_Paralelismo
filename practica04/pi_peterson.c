/*
 * ============================================================
 * Archivo   : pi_peterson.c
 * Autor     : Emmanuel 75449
 * Fecha     : 2025
 * Descripción: Cálculo de π con Monte Carlo usando hilos POSIX.
 *              La sección crítica (puntosDentro++) está protegida
 *              con el algoritmo de Peterson generalizado (filtro).
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define TOTAL_PUNTOS 100000000L
#define MAX_HILOS 16

/* --- Variables para el algoritmo de Peterson generalizado --- */
static int nivel[MAX_HILOS];   /* Nivel actual de cada hilo */
static int turno[MAX_HILOS];   /* Turno en cada nivel */

/* --- Variables compartidas del cálculo --- */
static long puntosDentro = 0;
static int numHilos;

/* --- Argumentos que recibe cada hilo --- */
typedef struct {
    int id;
    long puntosAProcesar;
} ArgsHilo;

/* ------------------------------------------------------------
 * Entrar a la sección crítica con Peterson generalizado.
 * Cada hilo sube nivel por nivel. En cada nivel, cede el turno
 * al otro y espera si es necesario.
 * ------------------------------------------------------------ */
static void entrarSeccionCritica(int id)
{
    for (int nivel_actual = 1; nivel_actual < numHilos; nivel_actual++) {
        nivel[id] = nivel_actual;
        turno[nivel_actual] = id;

        /* Esperar mientras otro hilo del mismo nivel tenga el turno */
        for (int j = 0; j < numHilos; j++) {
            if (j == id) continue;

            /* Espera ocupada: mientras el otro esté en nivel >= actual
               y sea su turno, espero */
            while (nivel[j] >= nivel_actual && turno[nivel_actual] == id) {
                /* spin */
            }
        }
    }
}

/* ------------------------------------------------------------
 * Salir de la sección crítica (reinicia el nivel del hilo).
 * ------------------------------------------------------------ */
static void salirSeccionCritica(int id)
{
    nivel[id] = 0;
}

/* ------------------------------------------------------------
 * Función que ejecuta cada hilo.
 * ------------------------------------------------------------ */
static void *calcularPi(void *arg)
{
    ArgsHilo *args = (ArgsHilo *)arg;
    int id = args->id;
    long n = args->puntosAProcesar;

    unsigned int seed = (unsigned int)(time(NULL) ^ (id * 2654435761u));
    long localesDentro = 0;

    for (long i = 0; i < n; i++) {
        double x = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;

        if (x*x + y*y <= 1.0) {
            localesDentro++;
        }
    }

    /* Sección crítica protegida con Peterson generalizado */
    entrarSeccionCritica(id);
    puntosDentro += localesDentro;
    salirSeccionCritica(id);

    free(args);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <numHilos>\n", argv[0]);
        return 1;
    }

    numHilos = atoi(argv[1]);
    if (numHilos < 1 || numHilos > MAX_HILOS) {
        fprintf(stderr, "numHilos debe estar entre 1 y %d\n", MAX_HILOS);
        return 1;
    }

    /* Inicializar variables de Peterson */
    for (int i = 0; i < numHilos; i++) {
        nivel[i] = 0;
    }
    for (int i = 0; i < numHilos; i++) {
        turno[i] = 0;
    }

    pthread_t *hilos = malloc(sizeof(pthread_t) * numHilos);
    long base = TOTAL_PUNTOS / numHilos;
    long residuo = TOTAL_PUNTOS % numHilos;

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < numHilos; i++) {
        ArgsHilo *args = malloc(sizeof(ArgsHilo));
        args->id = i;
        args->puntosAProcesar = base + (i < residuo ? 1 : 0);
        pthread_create(&hilos[i], NULL, calcularPi, args);
    }

    for (int i = 0; i < numHilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo = (fin.tv_sec - inicio.tv_sec) +
                    (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi = 4.0 * puntosDentro / TOTAL_PUNTOS;

    printf("Hilos          : %d\n", numHilos);
    printf("Puntos dentro  : %ld / %ld\n", puntosDentro, TOTAL_PUNTOS);
    printf("Pi aproximado  : %.6f\n", pi);
    printf("Error          : %.6f\n", fabs(pi - M_PI));
    printf("Tiempo         : %.6f segundos\n", tiempo);

    free(hilos);
    return 0;
}