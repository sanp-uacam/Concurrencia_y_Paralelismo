// Practica 02 - Semaforos (Productor-Consumidor: Cocinero/Mesero)
// Autor: Sergio A Noh Puch
// Fecha: 2026/09/06
// Que hace: simula un cocinero que prepara platillos y un mesero que los
// sirve. El cocinero (hilo productor) avisa mediante un semaforo cada vez
// que un platillo esta listo; el mesero (hilo consumidor) espera esa senal
// antes de servir. Al terminar ambos hilos, se imprime cuantos platillos
// fueron servidos en total.

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NR_LOOP 10 // cantidad de platillos que se van a preparar/servir

// Prototipos de las funciones que ejecutaran los hilos
static void * cocinar(void* arg); // hilo productor: prepara platillos
static void * servir(void* arg);  // hilo consumidor: sirve platillos

static int counter = 0; // cuenta cuantos platillos han sido servidos en total
sem_t sem1;             // semaforo que sincroniza a cocinero y mesero:
                        // arranca en 0 porque al inicio no hay comida lista

int main(void)
{
  pthread_t cocinero, mesero; // identificadores de los dos hilos

  // Se inicializa el semaforo: 0 = compartido solo entre hilos de este
  // proceso, 0 = valor inicial (no hay comida preparada todavia)
  sem_init(&sem1, 0, 0);

  // Se crean los hilos indicando la funcion que cada uno va a ejecutar
  pthread_create(&cocinero, NULL, cocinar, NULL);
  pthread_create(&mesero, NULL, servir, NULL);

  // El hilo principal espera a que ambos hilos terminen antes de continuar
  pthread_join(cocinero, NULL);
  pthread_join(mesero, NULL);

  printf("Contador %d \n", counter); // total de platillos servidos
  return 0;
}

// cocinar: no recibe datos utiles (arg no se usa) y no devuelve nada.
// Simula la preparacion de NR_LOOP platillos, avisando al mesero cada vez
// que uno queda listo.
static void * cocinar(void* arg)
{
  for (int i = 0; i < NR_LOOP; i++) // repite la preparacion NR_LOOP veces
  {
    printf("COCINERO: Comida preparada \n");
    sem_post(&sem1); // incrementa el semaforo: avisa que hay un platillo listo
    sleep(1);        // simula el tiempo que tarda en preparar el siguiente
  }
  return NULL;
}

// servir: no recibe datos utiles (arg no se usa) y no devuelve nada.
// Simula al mesero sirviendo NR_LOOP platillos, esperando siempre a que el
// cocinero avise que hay uno disponible.
static void * servir(void* arg)
{
  for (int i = 0; i < NR_LOOP; i++) // repite el servicio NR_LOOP veces
  {
    sem_wait(&sem1); // decrementa el semaforo; si es 0, el mesero espera aqui
    printf("MESERO: Comida servida \n");
    counter++; // se cuenta un platillo servido (solo este hilo lo modifica,
               // por lo que no necesita proteccion adicional)
  }
  return NULL;
}