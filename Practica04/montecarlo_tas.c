// Monte carlo con Set and Test
// Autor: Alberto Benjamin Poot Veloz
// Fecha: 14/09/2026
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

#define TOTAL_PUNTOS 100000000L // 100,000,000 de puntos requeridos

long puntosDentro = 0;
volatile int lock_flag = 0;
int num_hilos = 4; // Valor por defecto

// Implementación del cerrojo Test-and-Set (Spinlock)
void lock_test_and_set(volatile int *lock) {
    while (__atomic_test_and_set(lock, __ATOMIC_ACQUIRE)) {
        // Espera activa (Spin)
    }
}

void unlock_test_and_set(volatile int *lock) {
    __atomic_clear(lock, __ATOMIC_RELEASE);
}

void* calcular_pi(void* arg) {
    long id_hilo = (long)arg;
    long puntos_por_hilo = TOTAL_PUNTOS / num_hilos;

    // Semilla individual para rand_r por hilo
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self() ^ (unsigned int)id_hilo;

    for (long i = 0; i < puntos_por_hilo; i++) {
        double x = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            // --- SECCIÓN CRÍTICA PROTEGIDA CON TEST-AND-SET ---
            lock_test_and_set(&lock_flag);
            puntosDentro++;
            unlock_test_and_set(&lock_flag);
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        num_hilos = atoi(argv[1]);
    }

    pthread_t *hilos = malloc(sizeof(pthread_t) * num_hilos);

    for (long i = 0; i < num_hilos; i++) {
        if (pthread_create(&hilos[i], NULL, calcular_pi, (void*)i) != 0) {
            perror("Error al crear hilo");
            return 1;
        }
    }

    for (int i = 0; i < num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    double pi_estimado = 4.0 * (double)puntosDentro / TOTAL_PUNTOS;

    printf("Hilos: %d | Puntos: %ld | pi: %.6f | PuntosDentro: %ld\n", 
           num_hilos, TOTAL_PUNTOS, pi_estimado, puntosDentro);

    free(hilos);
    return 0;
}