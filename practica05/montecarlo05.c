#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// Variables globales
long long puntos_totales;
long long dentro_circulo;
int num_hilos;

sem_t semaforo;


// Funcion que ejecuta cada hilo
void *monte_carlo(void *arg) {

    long id = (long)arg;

    long long puntos_hilo = puntos_totales / num_hilos;

    long long locales = 0;

    unsigned int semilla =
        (unsigned int)time(NULL) + id;

    for (long long i = 0; i < puntos_hilo; i++) {

        double x =
            (double)rand_r(&semilla) / RAND_MAX;

        double y =
            (double)rand_r(&semilla) / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            locales++;
        }
    }

    sem_wait(&semaforo);

    dentro_circulo += locales;

    sem_post(&semaforo);

    return NULL;
}


// Funcion que realiza una prueba
double ejecutar_prueba(int hilos) {

    num_hilos = hilos;

    dentro_circulo = 0;

    pthread_t threads[num_hilos];

    struct timespec inicio;
    struct timespec fin;

    sem_init(&semaforo, 0, 1);

    // Iniciar tiempo
    clock_gettime(CLOCK_MONOTONIC, &inicio);


    // Crear hilos
    for (long i = 0; i < num_hilos; i++) {

        pthread_create(
            &threads[i],
            NULL,
            monte_carlo,
            (void *)i
        );
    }


    // Esperar a los hilos
    for (int i = 0; i < num_hilos; i++) {

        pthread_join(
            threads[i],
            NULL
        );
    }


    // Finalizar tiempo
    clock_gettime(CLOCK_MONOTONIC, &fin);


    double tiempo =
        (fin.tv_sec - inicio.tv_sec) +
        (fin.tv_nsec - inicio.tv_nsec)
        / 1000000000.0;


    double pi =
        4.0 * dentro_circulo /
        puntos_totales;


    printf("\nPrueba con %d hilo(s)\n", num_hilos);

    printf("Pi aproximado: %.8f\n", pi);

    printf("Tiempo: %.6f segundos\n", tiempo);


    sem_destroy(&semaforo);


    return tiempo;
}


// Programa principal
int main(int argc, char *argv[]) {

    // Solo necesitamos indicar los puntos
    if (argc != 2) {

        printf("Uso: %s <puntos>\n", argv[0]);

        return 1;
    }


    puntos_totales = atoll(argv[1]);


    if (puntos_totales <= 0) {

        printf("La cantidad de puntos debe ser mayor que 0.\n");

        return 1;
    }


    printf("\n");
    printf("====================================\n");
    printf("       MONTE CARLO PARA PI\n");
    printf("====================================\n");

    printf("Puntos utilizados: %lld\n",
           puntos_totales);


    // --------------------------------
    // EJECUCION CON 1 HILO
    // --------------------------------

    double T1 =
        ejecutar_prueba(1);


    // --------------------------------
    // EJECUCION CON 4 HILOS
    // --------------------------------

    double T4 =
        ejecutar_prueba(4);


    // --------------------------------
    // EJECUCION CON 8 HILOS
    // --------------------------------

    double T8 =
        ejecutar_prueba(8);


    // =================================
    // CALCULO DE SPEEDUP
    // =================================

    double S1 = 1.0;

    double S4 =
        T1 / T4;

    double S8 =
        T1 / T8;


    // =================================
    // CALCULO DE EFICIENCIA
    // =================================

    double E1 =
        S1 / 1;

    double E4 =
        S4 / 4;

    double E8 =
        S8 / 8;


    // =================================
    // CALCULO DE OVERHEAD
    // =================================

    double O1 =
        1 * T1 - T1;

    double O4 =
        4 * T4 - T1;

    double O8 =
        8 * T8 - T1;


    // =================================
    // TABLA FINAL
    // =================================

    printf("\n\n");

    printf("==============================================================\n");

    printf("                 RESULTADOS FINALES\n");

    printf("==============================================================\n");


    printf(
        "%-8s %-12s %-10s %-12s %-15s %-12s\n",
        "Hilos",
        "T(p)",
        "T(1)",
        "Speedup",
        "Eficiencia",
        "Overhead"
    );


    printf("--------------------------------------------------------------\n");


    printf(
        "%-8d %-12.4f %-10.4f %-12.2f %-14.2f%% %-12.4f\n",
        1,
        T1,
        T1,
        S1,
        E1 * 100,
        O1
    );


    printf(
        "%-8d %-12.4f %-10.4f %-12.2f %-14.2f%% %-12.4f\n",
        4,
        T4,
        T1,
        S4,
        E4 * 100,
        O4
    );


    printf(
        "%-8d %-12.4f %-10.4f %-12.2f %-14.2f%% %-12.4f\n",
        8,
        T8,
        T1,
        S8,
        E8 * 100,
        O8
    );


    printf("==============================================================\n");


    return 0;
}