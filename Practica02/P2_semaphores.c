/*
 * ============================================================
 * Archivo   : P2_semaphores.c
 * Autor     : Emmanuel 75449
 * Fecha     : 2025
 * Descripción: Simulación del problema productor-consumidor
 *              usando semáforos POSIX. Un cocinero produce
 *              platillos y seis meseros los consumen.
 * ============================================================
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define NR_LOOP 12
#define NUM_MESEROS 6

static int counter = 0;
static pthread_mutex_t mtx_counter;
static sem_t sem_platillos;

static void *cocinar(void *arg);
static void *servir(void *arg);

int main(void)
{
    pthread_t cocinero;
    pthread_t meseros[NUM_MESEROS];

    sem_init(&sem_platillos, 0, 0);
    pthread_mutex_init(&mtx_counter, NULL);

    pthread_create(&cocinero, NULL, cocinar, NULL);

    for (int i = 0; i < NUM_MESEROS; i++) {
        pthread_create(&meseros[i], NULL, servir, (void *)(long)i);
    }

    pthread_join(cocinero, NULL);

    for (int i = 0; i < NUM_MESEROS; i++) {
        pthread_join(meseros[i], NULL);
    }

    sem_destroy(&sem_platillos);
    pthread_mutex_destroy(&mtx_counter);

    printf("\n=== FIN ===\n");
    printf("Platillos restantes sin servir: %d\n", counter);

    return 0;
}

static void *cocinar(void *arg)
{
    (void)arg;
    for (int i = 0; i < NR_LOOP; i++) {
        pthread_mutex_lock(&mtx_counter);
        counter++;
        printf("COCINERO: Comida preparada. Platillos en espera: %d\n", counter);
        pthread_mutex_unlock(&mtx_counter);

        sem_post(&sem_platillos);
    }
    return NULL;
}

static void *servir(void *arg)
{
    long id = (long)arg;
    int porciones = NR_LOOP / NUM_MESEROS;   // 12 / 6 = 2

    for (int i = 0; i < porciones; i++) {
        sem_wait(&sem_platillos);            // FUERA del mutex ✅

        pthread_mutex_lock(&mtx_counter);
        counter--;
        printf("MESERO %ld: Comida servida. Platillos en espera: %d\n", id + 1, counter);
        pthread_mutex_unlock(&mtx_counter);
    }
    return NULL;
}