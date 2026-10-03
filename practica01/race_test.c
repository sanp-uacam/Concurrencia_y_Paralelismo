#include <stdio.h>
#include <pthread.h>

#define NUM_HILOS 2
#define ITERACIONES 1000000

long global_counter = 0;

void *incrementar(void *arg) {
    for (long i = 0; i < ITERACIONES; i++) {
        global_counter++;   // sección crítica sin protección
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

    printf("Valor esperado : %ld\n", (long)NUM_HILOS * ITERACIONES);
    printf("Valor obtenido : %ld\n", global_counter);

    return 0;
}