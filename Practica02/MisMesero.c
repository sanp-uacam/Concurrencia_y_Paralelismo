// Cocinero y Meseros- Autor: Leonardo Ivan Gongora Us
// Fecha: 2026/09/05

// Descripcion: Un hilo "cocinero" produce platillos y los
// anuncia con sem_post(). Tres hilos "mesero" consumen esos
// platillos con sem_wait(), de forma que cada plato preparado
// sea servido exactamente una vez, sin usar variables compartidas

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>   
#include <unistd.h>

#define NR_LOOP 12   // total de platillos a preparar (determina cuanto trabajo hay para repartir entre los meseros)

// Se declaran antes de main porque cada una correra dentro de un hilo distinto
static void *cocinar(void *arg); // rol productor
static void *servir(void *arg);  // rol consumidor

//El semaforo mismo lleva la cuenta de cuantos platos estan listos y aun no se han servido
sem_t sem1;

int main(void)
{
    pthread_t cocinero, mesero1, mesero2, mesero3;

    // Arranca en 0, nadie puede servir hasta que el cocinero anuncie un platillo
    sem_init(&sem1, 0, 0);

    pthread_create(&cocinero, NULL, cocinar, NULL);

    // Se identifica a cada mesero con un numero distinto para poder
    // distinguir en la salida quien sirvio cada platillo
    pthread_create(&mesero1, NULL, servir, (void *)1);
    pthread_create(&mesero2, NULL, servir, (void *)2);
    pthread_create(&mesero3, NULL, servir, (void *)3);

    // main espera a que los 4 hilos terminen para no cerrar el programa
    // (y destruir el semaforo) mientras todavia estan trabajando
    pthread_join(cocinero, NULL);
    pthread_join(mesero1, NULL);
    pthread_join(mesero2, NULL);
    pthread_join(mesero3, NULL);

    sem_destroy(&sem1);

    return 0;
}

// cocinar: produce NR_LOOP platillos y avisa su disponibilidad via el con el semaforo, 
// sin necesidad de saber cuantos meseros hay ni quien los atendera.
// Recibe: nada. Devuelve: NULL.
static void *cocinar(void *arg)
{
    for (int i = 0; i < NR_LOOP; i++)
    {
        printf("COCINERO: Comida preparada #%d\n", i + 1);

        // sem_post anuncia el platillo: cualquier mesero bloqueado en
        // sem_wait puede despertar a partir de aqui
        sem_post(&sem1);

        usleep(500000); // simula el tiempo que toma preparar un platillo
    }

    return NULL;
}

// servir: consume platillos anunciados por el cocinero. Cada mesero atiende
// solo una fraccion del total para repartir la carga entre los 3.
// Recibe: su id de mesero, pasado desde main como void*.
// Devuelve: NULL.
static void *servir(void *arg)
{
    int id = (intptr_t)arg; // recupera el id que main envio disfrazado de puntero

    // NR_LOOP/3 asegura que los 3 meseros en conjunto sirvan exactamente
    // los NR_LOOP platillos que el cocinero prepara, sin que sobre ni falte
    for (int i = 0; i < NR_LOOP / 3; i++)
    {
        // Si aun no hay platillo disponible, el hilo se bloquea aqui en vez
        // de consultar en un ciclo activo que desperdiciaria CPU
        sem_wait(&sem1);

        printf("MESERO %d: Comida servida\n", id);

        sleep(1); // simula el tiempo de llevar el platillo a la mesa
    }

    return NULL;
}