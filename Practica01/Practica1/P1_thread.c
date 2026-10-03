// Dann - Práctica #1
// Demostración de una condición de carrera (race condition) con dos hilos.
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Recurso compartido entre los dos hilos.
 * Al ser global, cualquier hilo puede leerlo y modificarlo.
 * Valor de arranque: 20. */
int global_counter = 20;

/* Mutex creado para serializar el acceso al contador.
 * Ojo: está declarado e inicializado, pero ninguna rutina lo usa
 * (no hay lock/unlock), así que en la práctica no protege nada. */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Hilo 1: incrementa el contador global.
 * Recibe por 'arg' la cantidad de iteraciones a realizar. */
void *thread_routine(void *arg)
{
    // Se recupera el entero que viaja dentro del puntero genérico
    int num_line = *((int*)arg);
    printf("Starting thread one..\n");

    for (int i = 0; i < num_line; i++)
    {
        // Incremento sin sincronización: lee, suma y escribe en pasos separados,
        // por lo que el otro hilo puede intercalarse y provocar pérdidas de actualizaciones
        global_counter++;
    }
    return NULL;
}

/* Hilo 2: decrementa el contador global.
 * Hace lo opuesto al hilo 1, con el mismo número de iteraciones. */
void *thread_routine_two(void *arg)
{
    // Igual que arriba: se extrae el entero del argumento
    int num_line = *((int*)arg);
    printf("Starting thread two..\n");

    for (int i = 0; i < num_line; i++)
    {
        // Decremento también sin protección, compitiendo con el hilo 1
        global_counter--;
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    // Identificadores de los dos hilos de trabajo
    pthread_t thread_one;
    pthread_t thread_two;

    // Se exige un argumento en la línea de comandos: el número de iteraciones
    if (argc < 2)
    {
        printf("Uso: ./execute numero\n");
        return -1;
    }

    // El argumento llega como texto; se convierte a entero
    int counter = atoi(argv[1]);

    // Se imprime el estado del contador antes de lanzar los hilos
    printf("Valor inicial: %d\n", global_counter);

    // Se crea el hilo 1 y se le pasa la dirección de 'counter' como parámetro
    if (pthread_create(&thread_one, NULL, thread_routine, &counter) != 0)
        return -1;

    // Se crea el hilo 2 con el mismo parámetro
    if (pthread_create(&thread_two, NULL, thread_routine_two, &counter) != 0)
        return -1;

    // main se bloquea hasta que cada hilo termine su ejecución
    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    // Con sincronización correcta el resultado sería 20 (sumas y restas se cancelan);
    // sin ella, con iteraciones altas, normalmente será otro valor
    printf("Valor final: %d (esperado: 20)\n", global_counter);

    // Se libera el mutex, aunque nunca llegó a utilizarse
    pthread_mutex_destroy(&mutex);
    return 0;