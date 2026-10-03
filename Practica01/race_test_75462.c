#include <stdio.h>
#include <pthread.h>


/*
 * ============================================================================
 * CONCURRENCIA: Condiciones de Carrera y Sección Crítica
 * ============================================================================
 *
 * 1. CONDICIÓN DE CARRERA (RACE CONDITION)
 * Ocurre cuando varios hilos acceden y modifican el mismo recurso compartido
 * (en este caso, 'global_counter') al mismo tiempo. Como no hay un orden
 * garantizado de cuál hilo lee o escribe primero, el resultado final varía
 * e inserviblemente en cada ejecución.
 *
 * 2. SECCIÓN CRÍTICA
 * Es el bloque de código que accede a la variable compartida. Para evitar
 * corrupción de datos, esta zona debe ejecutarse bajo exclusión mutua
 * (solo un hilo a la vez).*/


// CONDICIÓN DE CARRERA: Ocurre cuando dos o más hilos modifican esta variable al mismo tiempo
// y el resultado final depende de cuál hilo termina primero (es impredecible).
int global_counter = 20;//Varible que comparten los hilos
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Hilo 1: En ensamblador (gcc -S) ++ usa: movl (leer), addl (sumar 1), movl (guardar)
void *thread_routine(void *arg) {
    for (size_t i = 0; i < 100000; i++) {
        // SECCIÓN CRÍTICA: Bloque de código que modifica el recurso compartido.
        // Debe ejecutarse con exclusión mutua (mutex) para evitar corrupción de datos.
        // pthread_mutex_lock(&mutex);
        global_counter++;
        // pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Hilo 2: En ensamblador (gcc -S) -- usa: movl (leer), subl (restar 1), movl (guardar)
void *thread_routine_two(void *arg) {
    for (size_t i = 0; i < 100000; i++) {
        // SECCIÓN CRÍTICA: Si el SO pausa este hilo en medio del movl/subl/movl,
        // el otro hilo leerá un dato desactualizado.
        // pthread_mutex_lock(&mutex);
        global_counter--;
        // pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
//Aqui inician los hilos
    pthread_create(&t1, NULL, thread_routine, NULL);
    pthread_create(&t2, NULL, thread_routine_two, NULL);

//Aqui esperan a que los hilos terminen
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Resultado esperado: 20\n");
    printf("Resultado real:     %d\n", global_counter);

    return 0;
}
