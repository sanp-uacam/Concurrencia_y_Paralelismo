/*
 * Programa: Sincronización entre cocinero y meseros
 * Autor: Erbet Gomez Bohorquez-2026/09/08
 *
 * ¿Qué hace?:
 * Un cocinero prepara una cantidad determinada de platos y cuatro
 * meseros los entregan. Un semáforo controla que ningún mesero
 * intente entregar un plato que todavía no haya sido preparado.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define CANTIDAD_PLATOS 40       // Número total de platos preparados
#define RACIONES_MESERO 10       // Cantidad de platos que entrega cada mesero

/* Funciones ejecutadas por los diferentes hilos.
 * Reciben un puntero genérico y devuelven un puntero genérico.
 */
static void *prepararPlatos(void *dato);
static void *atenderMesa1(void *dato);
static void *atenderMesa2(void *dato);
static void *atenderMesa3(void *dato);
static void *atenderMesa4(void *dato);

/* Variable compartida que permite conocer cuántos platos
 * han sido preparados por el cocinero.
 */
static int totalPreparados = 0;

/* El semáforo representa la cantidad de platos disponibles
 * para que los meseros puedan tomarlos.
 */
sem_t platosDisponibles;

int main(void)
{
    pthread_t cocinero;
    pthread_t mesero1, mesero2, mesero3, mesero4;

    /* Se inicia el semáforo en cero porque al comienzo
     * ningún plato está disponible para los meseros.
     */
    sem_init(&platosDisponibles, 0, 0);

    /* Se crea un hilo para el cocinero y cuatro para los meseros. */
    pthread_create(&cocinero, NULL, prepararPlatos, NULL);
    pthread_create(&mesero1, NULL, atenderMesa1, NULL);
    pthread_create(&mesero2, NULL, atenderMesa2, NULL);
    pthread_create(&mesero3, NULL, atenderMesa3, NULL);
    pthread_create(&mesero4, NULL, atenderMesa4, NULL);

    /* El programa principal espera a que todos los hilos
     * terminen sus respectivas tareas.
     */
    pthread_join(cocinero, NULL);
    pthread_join(mesero1, NULL);
    pthread_join(mesero2, NULL);
    pthread_join(mesero3, NULL);
    pthread_join(mesero4, NULL);

    printf("\nTotal de platos preparados: %d\n", totalPreparados);

    /* Se libera el recurso utilizado para la sincronización. */
    sem_destroy(&platosDisponibles);

    return 0;
}

/*
 * Función: prepararPlatos
 * Recibe: un puntero genérico que no se utiliza.
 * Devuelve: NULL.
 *
 * Propósito:
 * Simula el trabajo del cocinero. Cada plato preparado incrementa
 * el contador y libera un permiso del semáforo para indicar que
 * existe una nueva unidad disponible para los meseros.
 */
static void *prepararPlatos(void *dato)
{
    for (int plato = 1; plato <= CANTIDAD_PLATOS; plato++)
    {
        totalPreparados++;

        printf("[Cocina] Plato %d listo\n", totalPreparados);
        fflush(stdout);

        /* Cada llamada agrega un permiso al semáforo.
         * Ese permiso podrá ser utilizado por cualquiera
         * de los cuatro meseros.
         */
        sem_post(&platosDisponibles);
    }

    return NULL;
}

/*
 * Función: atenderMesa1
 * Recibe: un puntero genérico que no se utiliza.
 * Devuelve: NULL.
 *
 * Propósito:
 * Espera un plato disponible antes de realizar cada entrega.
 */
static void *atenderMesa1(void *dato)
{
    for (int entrega = 1; entrega <= RACIONES_MESERO; entrega++)
    {
        sem_wait(&platosDisponibles);

        printf("   Mesero 1 entrego el plato %d de su turno\n", entrega);
        fflush(stdout);

        /* Simula el tiempo necesario para llevar el plato
         * desde la cocina hasta la mesa.
         */
        sleep(1);
    }

    return NULL;
}

/*
 * Función: atenderMesa2
 * Recibe: un puntero genérico que no se utiliza.
 * Devuelve: NULL.
 */
static void *atenderMesa2(void *dato)
{
    for (int entrega = 1; entrega <= RACIONES_MESERO; entrega++)
    {
        sem_wait(&platosDisponibles);

        printf("   Mesero 2 entrego el plato %d de su turno\n", entrega);
        fflush(stdout);

        sleep(1);
    }

    return NULL;
}

/*
 * Función: atenderMesa3
 * Recibe: un puntero genérico que no se utiliza.
 * Devuelve: NULL.
 */
static void *atenderMesa3(void *dato)
{
    for (int entrega = 1; entrega <= RACIONES_MESERO; entrega++)
    {
        sem_wait(&platosDisponibles);

        printf("   Mesero 3 entrego el plato %d de su turno\n", entrega);
        fflush(stdout);

        sleep(1);
    }

    return NULL;
}

/*
 * Función: atenderMesa4
 * Recibe: un puntero genérico que no se utiliza.
 * Devuelve: NULL.
 */
static void *atenderMesa4(void *dato)
{
    for (int entrega = 1; entrega <= RACIONES_MESERO; entrega++)
    {
        sem_wait(&platosDisponibles);

        printf("   Mesero 4 entrego el plato %d de su turno\n", entrega);
        fflush(stdout);

        sleep(1);
    }

    return NULL;
}
