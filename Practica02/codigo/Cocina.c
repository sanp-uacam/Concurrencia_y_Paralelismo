// Práctica de semáforos - Autor: Laura Lucia Jaramillo Yah - 08/09/2026
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define TOTAL_COMIDAS 10

// Guarda el número del mesero y las comidas que debe entregar.
typedef struct
{
    int numero;
    int entregas;
} DatosMesero;

// Funciones que ejecutarán los hilos.
static void *cocinar(void *arg);
static void *servir(void *arg);

// Lleva la cantidad de comidas preparadas que todavía no se entregan.
static int comidas_sin_entregar = 0;

// Indica cuántas comidas están disponibles para los meseros.
sem_t comidas_disponibles;

// Evita que varios hilos modifiquen el contador al mismo tiempo.
pthread_mutex_t mutex_contador = PTHREAD_MUTEX_INITIALIZER;

int main(void)
{
    // Hilos utilizados para el cocinero y los dos meseros.
    pthread_t cocinero;
    pthread_t mesero1;
    pthread_t mesero2;

    // Cada mesero recibe un número y una cantidad de entregas.
    DatosMesero datos1 = {1, TOTAL_COMIDAS / 2};
    DatosMesero datos2 = {2, TOTAL_COMIDAS - datos1.entregas};

    // Inicia en cero porque todavía no existe comida preparada.
    sem_init(&comidas_disponibles, 0, 0);

    // Inicia el trabajo del cocinero y de los dos meseros.
    pthread_create(&cocinero, NULL, cocinar, NULL);
    pthread_create(&mesero1, NULL, servir, &datos1);
    pthread_create(&mesero2, NULL, servir, &datos2);

    // Espera hasta que todas las comidas sean preparadas y entregadas.
    pthread_join(cocinero, NULL);
    pthread_join(mesero1, NULL);
    pthread_join(mesero2, NULL);

    printf("PROCESO TERMINADO\n");
    printf("Comidas sin entregar: %d\n", comidas_sin_entregar);

    // Libera los recursos de sincronización al terminar.
    sem_destroy(&comidas_disponibles);
    pthread_mutex_destroy(&mutex_contador);

    return 0;
}

// Prepara las comidas y avisa a los meseros cuando hay una disponible.
static void *cocinar(void *arg)
{
    for (int i = 0; i < TOTAL_COMIDAS; i++)
    {
        // Protege el contador mientras el cocinero lo modifica.
        pthread_mutex_lock(&mutex_contador);

        comidas_sin_entregar++;

        printf("COCINERO: Comida preparada\n");
        printf("Comidas sin entregar: %d\n\n", comidas_sin_entregar);

        pthread_mutex_unlock(&mutex_contador);

        // Permite que uno de los meseros tome la comida preparada.
        sem_post(&comidas_disponibles);

        // El cocinero prepara una comida por segundo.
        sleep(1);
    }

    return NULL;
}

// Entrega las comidas asignadas al mesero recibido como argumento.
static void *servir(void *arg)
{
    DatosMesero *datos = (DatosMesero *)arg;

    for (int i = 0; i < datos->entregas; i++)
    {
        // Espera si todavía no existe una comida disponible.
        sem_wait(&comidas_disponibles);

        // Protege el contador mientras el mesero lo modifica.
        pthread_mutex_lock(&mutex_contador);

        comidas_sin_entregar--;

        printf("MESERO %d: Comida entregada\n", datos->numero);
        printf("Comidas sin entregar: %d\n\n", comidas_sin_entregar);

        pthread_mutex_unlock(&mutex_contador);

        // Los meseros trabajan más lento para que las comidas se acumulen.
        sleep(5);
    }

    return NULL;
}