/* ============================================================================
 * ALGORITMO DE PETERSON - Exclusion Mutua para 2 procesos/hilos
 * ============================================================================
 * Concurrencia y Paralelismo - Exposicion 01: Algoritmos de Exclusion Mutua
 * Equipo 2 - Algoritmo: Peterson (soporta N=2 procesos)
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <time.h>

volatile atomic_int flag[2];   /* flag[i]: el proceso i quiere entrar a la SC */
volatile atomic_int turno;     /* de quien es el turno de ceder el paso */

/* Recurso compartido que se protege con la seccion critica.
 * Si el algoritmo de exclusion mutua funciona, jamas debe corromperse. */
long contador_compartido = 0;

/* Numero de iteraciones que cada proceso ejecutara sobre la SC */
#define N_ITERACIONES 100000

/* Para poder registrar mensajes de la simulacion sin condiciones de
 * carrera en la propia impresion (esto NO forma parte del algoritmo,
 * solo es utilidad para mostrar la traza en pantalla). */
pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER;

void log_evento(int id_proceso, const char *evento) {
    pthread_mutex_lock(&mutex_log);
    printf("[Proceso %d] %s\n", id_proceso, evento);
    pthread_mutex_unlock(&mutex_log);
}

/* ---------------------------------------------------------------------
 * peterson_lock(id): entra a la seccion critica siguiendo el algoritmo
 * de Peterson. 'id' es el proceso que llama (0 o 1); 'otro' es el rival.
 * --------------------------------------------------------------------- */
void peterson_lock(int id) {
    int otro = 1 - id;

    /* 1) Anuncio mi intencion de entrar a la seccion critica */
    atomic_store(&flag[id], 1);

    /* 2) Cedo el turno al otro proceso (cortesia) */
    atomic_store(&turno, otro);

    /* 3) Espero mientras:
     *      - el otro proceso tambien quiera entrar (flag[otro] == true)
     *      - Y ademas el turno sea del otro proceso (turno == otro)
     *    Esta espera ocupada (busy-wait) es la barrera que garantiza
     *    la exclusion mutua. En cuanto una de las dos condiciones deje
     *    de cumplirse, puedo entrar. */
    while (atomic_load(&flag[otro]) && atomic_load(&turno) == otro) {
        /* espera activa (spin-wait) */
    }
}

/* ---------------------------------------------------------------------
 * peterson_unlock(id): sale de la seccion critica, bajando mi bandera
 * para indicar que ya no quiero (ni estoy) usando el recurso.
 * --------------------------------------------------------------------- */
void peterson_unlock(int id) {
    atomic_store(&flag[id], 0);
}

/* ---------------------------------------------------------------------
 * Funcion que ejecuta cada hilo (simula un "proceso").
 * Alterna entre "seccion no critica" (trabajo normal) y
 * "seccion critica" (acceso protegido al recurso compartido).
 * --------------------------------------------------------------------- */
void *rutina_proceso(void *arg) {
    int id = *(int *)arg;
    char msg[64];

    for (int i = 0; i < N_ITERACIONES; i++) {

        /* ---------- SECCION NO CRITICA (trabajo independiente) ---------- */
        /* (aqui cada proceso hace tareas que no requieren el recurso) */

        /* ---------- ENTRADA A LA SECCION CRITICA ---------- */
        peterson_lock(id);

        /* ---------- SECCION CRITICA ---------- */
        /* Solo un proceso puede estar aqui a la vez. Lo demostramos
         * incrementando una variable compartida SIN proteccion adicional
         * (sin mutex de pthread): si Peterson falla, el contador final
         * no coincidira con el esperado. */
        long temp = contador_compartido;
        temp = temp + 1;          /* operacion NO atomica a proposito */
        contador_compartido = temp;

        if (i == 0 || i == N_ITERACIONES - 1) {
            snprintf(msg, sizeof(msg), "entra a SC (iter %d) contador=%ld",
                     i, contador_compartido);
            log_evento(id, msg);
        }

        /* ---------- SALIDA DE LA SECCION CRITICA ---------- */
        peterson_unlock(id);

        /* ---------- RESTO (seccion no critica) ---------- */
    }

    return NULL;
}

int main(void) {
    pthread_t hilo0, hilo1;
    int id0 = 0, id1 = 1;

    /* Inicializacion de variables compartidas del algoritmo */
    atomic_store(&flag[0], 0);
    atomic_store(&flag[1], 0);
    atomic_store(&turno, 0);

    printf("=====================================================\n");
    printf(" ALGORITMO DE PETERSON - Exclusion Mutua (2 procesos)\n");
    printf("=====================================================\n");
    printf("Iteraciones por proceso: %d\n", N_ITERACIONES);
    printf("Valor esperado del contador al finalizar: %d\n\n",
           2 * N_ITERACIONES);

    struct timespec t_inicio, t_fin;
    clock_gettime(CLOCK_MONOTONIC, &t_inicio);

    /* Creamos los dos hilos que compiten por la seccion critica */
    pthread_create(&hilo0, NULL, rutina_proceso, &id0);
    pthread_create(&hilo1, NULL, rutina_proceso, &id1);

    /* Esperamos a que ambos terminen */
    pthread_join(hilo0, NULL);
    pthread_join(hilo1, NULL);

    clock_gettime(CLOCK_MONOTONIC, &t_fin);
    double segundos = (t_fin.tv_sec - t_inicio.tv_sec) +
                       (t_fin.tv_nsec - t_inicio.tv_nsec) / 1e9;

    printf("\n=====================================================\n");
    printf("Contador final obtenido : %ld\n", contador_compartido);
    printf("Contador esperado       : %d\n", 2 * N_ITERACIONES);
    printf("Resultado                : %s\n",
           (contador_compartido == 2 * N_ITERACIONES)
               ? "CORRECTO (exclusion mutua garantizada)"
               : "ERROR (hubo condicion de carrera)");
    printf("Tiempo de ejecucion      : %.4f s\n", segundos);
    printf("=====================================================\n");

    return 0;
}
