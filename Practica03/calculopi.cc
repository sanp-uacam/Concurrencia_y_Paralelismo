//pracitca monte carlo
//autor: Bryan Ariel Figueroa Quintal
//Fecha: 09/09/2026

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

#define TOTAL_PUNTOS 10000000L
#define NUM_HILOS 16

long puntos_dentro_total = 0;
long puntos_fuera_local = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* calcular_pi(void* arg) {
    long id_hilo = (long)arg;
    long puntos_por_hilo = TOTAL_PUNTOS / NUM_HILOS;
    long puntos_dentro_local = 0;


    // Semilla unica por hilo usando el tiempo y el ID del hilo
    // Se usa uintptr_t para evitar advertencias de truncamiento de punteros de 64 a 32 bits
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self() ^ (unsigned int)id_hilo;

    for (long i = 0; i < puntos_por_hilo; i++) {
        // Generar numeros flotantes entre -1.0 y 1.0 usando rand_r (thread-safe)
        double x = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            puntos_dentro_local++;
        }
    }

    // Sincronizacion unica por hilo al finalizar el bucle
    pthread_mutex_lock(&lock);
    puntos_dentro_total += puntos_dentro_local;
    puntos_fuera_local = puntos_dentro_total - TOTAL_PUNTOS;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    pthread_t hilos[NUM_HILOS];

    // Creacion de los hilos pasando su ID
    for (long i = 0; i < NUM_HILOS; i++) {
        if (pthread_create(&hilos[i], NULL, calcular_pi, (void*)i) != 0) {
            perror("Error al crear el hilo");
            return 1;
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    // Estimacion final de PI
    double pi_estimado = 4.0 * (double)puntos_dentro_total / TOTAL_PUNTOS;

    printf("Puntos dentro del circulo: %ld / %ld\n", puntos_dentro_total, TOTAL_PUNTOS);
    printf("puntos Fuera del circulo  %ld / %ld\n:",puntos_fuera_local, TOTAL_PUNTOS );
    printf("Estimacion de PI: %.6f\n", pi_estimado);


    return 0;
}
//**Conclusión**

//El mejor rendimiento se obtuvo al utilizar 16 hilos,
//alcanzando el tiempo de ejecución real más bajo de 0.575 segundos (frente a 0.618s con 8 hilos y 0.802s con 2 hilos).


