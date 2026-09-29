/*
 * Programa: Cocinero y tres meseros
 * Autor: Joaquin Alberto de la Cruz Morales Fuentes
 * Fecha: 08/09/2026
 * Qué hace: Simula un cocinero que prepara 30 comidas y tres meseros
 *           que compiten entre sí para servir las comidas disponibles.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define NR_LOOP 20       // cantidad total de comidas que se preparan
#define NR_MESEROS 3     // cantidad de meseros

// No recibe datos y retorna NULL al terminar
static void *cocinar(void *arg);

// Recibe el identificador del mesero y retorna NULL al terminar
static void *servir(void *arg);

static int counter = 0;   // cantidad de comidas preparadas
static int servidas = 0;  // cantidad de comidas que ya fueron servidas

sem_t sem1; // semáforo que controla las comidas disponibles

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// protege la variable servidas para evitar que varios meseros la modifiquen al mismo tiempo


int main(void)
{
    pthread_t cocinero;              // identificador del hilo cocinero
    pthread_t meseros[NR_MESEROS];  // identificadores de los tres meseros
    int ids[NR_MESEROS];             // identificadores de los meseros

    // Inicializa el semaforo en 0 porque al principio no hay comida
    sem_init(&sem1, 0, 0);

    // Crea el hilo del cocinero
    pthread_create(&cocinero, NULL, cocinar, NULL);

    // Recorre los tres meseros para crear sus hilos
    for (int i = 0; i < NR_MESEROS; i++)
    {
        ids[i] = i + 1; // asigna los identificadores 1, 2 y 3

        // Crea el hilo del mesero y le pasa su identificador
        pthread_create(&meseros[i], NULL, servir, &ids[i]);
    }

    // Espera a que el cocinero termine
    pthread_join(cocinero, NULL);

    // Recorre los tres meseros para esperar a que terminen
    for (int i = 0; i < NR_MESEROS; i++)
    {
        pthread_join(meseros[i], NULL);
    }

    // Muestra la cantidad total de comidas preparadas
    printf("Contador %d\n", counter);

    // Libera los recursos utilizados por el semáforo
    sem_destroy(&sem1);

    // Libera el mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}


static void *cocinar(void *arg)
{
    // Recorre las comidas que se deben preparar
    for (int i = 0; i < NR_LOOP; i++)
    {
        // Indica que el cocinero preparó una comida
        printf("COCINERO: Comida preparada\n");

        // Incrementa el contador de comidas preparadas
        counter++;

        // Indica que hay una comida disponible para los meseros
        sem_post(&sem1);

        sleep(1);
    }

    /*
     * Envía tres señales adicionales para despertar a los
     * tres meseros cuando ya no queden comidas por servir.
     */
    for (int i = 0; i < NR_MESEROS; i++)
    {
        sem_post(&sem1);
    }

    return NULL;
}


static void *servir(void *arg)
{
    int id = *(int *)arg; // obtiene el identificador del mesero

    while (1)
    {
        /*
         * El mesero espera a que haya una comida disponible.
         * Los tres meseros compiten por este mismo semáforo.
         */
        sem_wait(&sem1);

        // Protege el acceso a la variable servidas
        pthread_mutex_lock(&mutex);

        // Comprueba si todavía quedan comidas por servir
        if (servidas < NR_LOOP)
        {
            // Este mesero consiguió una comida
            servidas++;

            // Libera el mutex
            pthread_mutex_unlock(&mutex);

            // Muestra que mesero consiguió la comida
            printf("MESERO %d: Comida servida\n", id);
        }
        else
        {
            /*
             * Ya se sirvieron las 30 comidas.
             * Esta señal solamente sirve para despertar al mesero
             * y permitir que termine su ejecución.
             */
            pthread_mutex_unlock(&mutex);

            break;
        }
    }

    return NULL;
}