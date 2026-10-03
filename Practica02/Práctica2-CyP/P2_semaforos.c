
/*
 * Práctica 2: Hilos y semáforos
 * Autor: Gladys Candy Chumá Pérez
 * Fecha: 27/09/2026

 
 * Descripción: Simula la preparación y entrega de
 * 10 comidas utilizando un cocinero y dos meseros.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NR_LOOP 10

static void * cocinar(void* arg);
static void * servir(void* arg);

// Cantidad de platillos pendientes
static int counter = 0;

// Total de comidas servidas
static int servidas = 0;

// Semáforo para controlar las comidas disponibles
sem_t sem1;

// Protege los contadores compartidos
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void)
{
    pthread_t cocinero, mesero_1, mesero_2;

    // Identificadores para distinguir a los meseros
    int id1 = 1;
    int id2 = 2;

    sem_init(&sem1, 0, 0);

    printf("Iniciando preparacion de %d comidas...\n\n", NR_LOOP);

    // Crea el cocinero y los dos meseros
    pthread_create(&cocinero, NULL, cocinar, NULL);
    pthread_create(&mesero_1, NULL, servir, &id1);
    pthread_create(&mesero_2, NULL, servir, &id2);

    // Espera a que terminen los tres hilos
    pthread_join(cocinero, NULL);
    pthread_join(mesero_1, NULL);
    pthread_join(mesero_2, NULL);

    // Muestra los resultados finales
    printf("Comidas preparadas: %d\n", NR_LOOP);
    printf("Comidas servidas:   %d\n", servidas);
    printf("Platillos pendientes: %d\n", counter);

    sem_destroy(&sem1);
    pthread_mutex_destroy(&mutex);

    return 0;
}

// Prepara las 10 comidas
static void * cocinar(void* arg)
{
    for (int i = 0; i < NR_LOOP; i++)
    {
        pthread_mutex_lock(&mutex);

        counter++;

        printf("[COCINERO] Comida %d preparada | Pendientes: %d\n",
               i + 1, counter);

        pthread_mutex_unlock(&mutex);

        // Avisa que hay una comida disponible
        sem_post(&sem1);

        // Espera medio segundo
        usleep(500000);
    }

    return NULL;
}

// Cada mesero sirve 5 comidas
static void * servir(void* arg)
{
    int id = *((int *)arg);

    for (int i = 0; i < NR_LOOP / 2; i++)
    {
        // Espera hasta que haya comida disponible
        sem_wait(&sem1);

        pthread_mutex_lock(&mutex);

        counter--;
        servidas++;

        printf("[MESERO %d] Comida servida | Pendientes: %d\n",
               id, counter);

        pthread_mutex_unlock(&mutex);

        // Espera dos segundos antes de servir otra comida
        sleep(2);
    }

    return NULL;
}
