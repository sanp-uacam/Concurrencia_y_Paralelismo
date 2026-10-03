#include <stdio.h>
#include <pthread.h>

#define NUM_HILOS 2
#define ITERACIONES 1000000

long global_counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *incrementar(void *arg) {
    for (long i = 0; i < ITERACIONES; i++) {
        pthread_mutex_lock(&mutex);     // Entrada a la sección crítica
        global_counter++;               // Sección crítica protegida
        pthread_mutex_unlock(&mutex);   // Salida de la sección crítica
    }
    return NULL;
}

int main(void) {
    pthread_t hilos[NUM_HILOS];

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_create(&hilos[i], NULL, incrementar, NULL);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Valor esperado : %ld\n", (long)NUM_HILOS * ITERACIONES);
    printf("Valor obtenido : %ld\n", global_counter);

    return 0;
}