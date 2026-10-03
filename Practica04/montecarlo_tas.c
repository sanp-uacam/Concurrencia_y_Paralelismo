// Monte Carlo con Test-and-Set
// Autor: Royfran Rodrigo Santini Pacheco
// Fecha: 28/09/2026

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

#define TOTAL_PUNTOS 100000000L // 100,000,000 de puntos requeridos

long puntosDentro = 0;
volatile int lock_flag = 0;
int num_hilos = 4; // Valor por defecto

// Implementación de un generador aleatorio reentrante/thread-safe para entornos sin rand_r
static inline int my_rand_r(unsigned int *seed) {
    *seed = *seed * 1103515245 + 12345;
    return (unsigned int)(*seed / 65536) % 32768;
}

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
    // Uso de intptr_t para evitar advertencias de tamaño de puntero en 64 bits
    intptr_t id_hilo = (intptr_t)arg;
    long puntos_por_hilo = TOTAL_PUNTOS / num_hilos;
    long puntos_locales = 0;

    // Semilla individual por hilo
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self() ^ (unsigned int)id_hilo;

    for (long i = 0; i < puntos_por_hilo; i++) {
        double x = ((double)my_rand_r(&seed) / 32767.0) * 2.0 - 1.0;
        double y = ((double)my_rand_r(&seed) / 32767.0) * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            puntos_locales++;
        }
    }

    // --- SECCIÓN CRÍTICA PROTEGIDA CON TEST-AND-SET ---
    lock_test_and_set(&lock_flag);
    puntosDentro += puntos_locales;
    unlock_test_and_set(&lock_flag);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        num_hilos = atoi(argv[1]);
    }

    pthread_t *hilos = malloc(sizeof(pthread_t) * num_hilos);

    for (intptr_t i = 0; i < num_hilos; i++) {
        // Cast a (void*)(intptr_t) para compatibilidad exacta en 64-bit Windows
        if (pthread_create(&hilos[i], NULL, calcular_pi, (void*)i) != 0) {
            perror("Error al crear hilo");
            free(hilos);
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