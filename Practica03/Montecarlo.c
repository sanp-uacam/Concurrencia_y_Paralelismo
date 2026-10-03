// Estimacion de PI con el metodo de Monte Carlo usando hilos y semaforo
// Autor: Leonardo Ivan Gongora Us
// Fecha: 2026/09/14

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define TOTAL_PUNTOS 1000000L

static long puntosDentro = 0;
static sem_t lock;
static long numHilos;
static long puntosPorHilo;

static void *generarPuntos(void *arg);

int main(int argc, char *argv[])
{
    numHilos = (argc > 1) ? atol(argv[1]) : 4;

    if (numHilos <= 0)
    {
        fprintf(stderr, "El numero de hilos debe ser mayor a 0\n");
        return 1;
    }

    puntosPorHilo = TOTAL_PUNTOS / numHilos;

    srand((unsigned int)time(NULL));

    pthread_t *hilos = malloc(numHilos * sizeof(pthread_t));
    if (hilos == NULL)
    {
        fprintf(stderr, "No se pudo reservar memoria para los hilos\n");
        return 1;
    }

    sem_init(&lock, 0, 1);

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 0; i < numHilos; i++)
    {
        pthread_create(&hilos[i], NULL, generarPuntos, (void *)(intptr_t)i);
    }

    for (long i = 0; i < numHilos; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempoSegundos = (fin.tv_sec - inicio.tv_sec) +
                             (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi = 4.0 * (double)puntosDentro / (double)TOTAL_PUNTOS;

    printf("Hilos: %ld\n", numHilos);
    printf("Total de puntos: %ld\n", TOTAL_PUNTOS);
    printf("Puntos dentro del circulo: %ld\n", puntosDentro);
    printf("Aproximacion de PI: %.6f\n", pi);
    printf("Tiempo de ejecucion: %.6f segundos\n", tiempoSegundos);

    sem_destroy(&lock);
    free(hilos);

    return 0;
}

static void *generarPuntos(void *arg)
{
    long idHilo = (intptr_t)arg;
    (void)idHilo;

    for (long i = 0; i < puntosPorHilo; i++)
    {
        double x, y;

        sem_wait(&lock);

        x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y <= 1.0)
        {
            puntosDentro++;
        }

        sem_post(&lock);
    }

    return NULL;
}

//Resultados con el Metodo Montecarlo con Mutex

//| Hilos |      Puntos | π calculado |   Tiempo    |
//| ----: | ----------: | ----------: |   -----:    |
//|     2 | 1,000,000 |  3.142608     | 7.403561 s  |
//|     4 | 1,000,000 |  3.145920    |  7.443623 s  | 
//|    16 | 1,000,000 |  3.147840    |  9.694889  s |

//Conclusión
//Con 2 hilos se obtuvo el mejor rendimiento (7.40 s). El tiempo se mantiene casi igual con 8 hilos (7.44 s) 
//y empeora claramente con 16 (9.69 s). Como cada iteración genera los números aleatorios y actualiza puntosDentro
// dentro de la misma sección crítica (sem_wait/sem_post), en la práctica los hilos nunca trabajan 
//en paralelo de verdad se turnan uno por uno para entrar al candado. Con más hilos solo se agrega overhead de 
//creación de hilos y de cambios de contexto entre ellos compitiendo por el mismo semáforo. Por eso el tiempo sube en vez de bajar.

// ¿Aumenta la cantidad de hilos siempre mejora el rendimiento? Explique sus resultados.
//Aumentar los hilos no siempre mejora el rendimiento: al proteger casi todo el trabajo dentro del candado (generar puntos y contar),
//los hilos terminan ejecutándose casi en serie, turnándose por el semáforo en vez de trabajar en paralelo. 
//Con más hilos solo crece el overhead (crearlos, sincronizarlos, contención por el candado) sin ganancia real de paralelismo,
//por eso el tiempo se mantiene igual o empeora al pasar de 2 a 16 hilos.
