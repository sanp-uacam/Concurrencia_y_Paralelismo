/*Concurrencia y Paralelismo: Practica 02 : Semaforos
autor: Aldair Eddiel Canul Cabrera
fecha: 2026/09/08
Descripción: Controla que los meseros solo sirvan la comida después de que el cocinero la prepare.
*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NR_LOOP 10 //Aqui son las veces en que se va a repetir el proceso por mesero

//Estas son las funciones de los hilos que son cocinar y servir
static void * cocinar(void* arg);
static void * servir(void* arg);

static int counter = 0; //Este es el contador global de las tareas
sem_t sem1; //Aqui esta el semaforo que avisa cuando la comida esta lista

// main: función principal del programa. Recibe void y devuelve un entero (0)
int main(void)
{
  pthread_t cocinero, mesero1, mesero2; //Variables para controlar los hilos
  sem_init(&sem1, 0, 0); //Aqui se inicializa el semaforo en 0 para que el mesero espere el platillo "Tiempo compartido"
  int id_mesero1 = 1; // Identificador del primer mesero
  int id_mesero2 = 2; // Identificador del segundo mesero

  //Aqui empiezan a funcionar los hilos del mesero y del cocinero
  pthread_create(&cocinero, NULL, cocinar, NULL);
  pthread_create(&mesero1, NULL, servir, &id_mesero1);
  pthread_create(&mesero2, NULL, servir, &id_mesero2);

  //Espera a que ambos hilos terminen su trabajo
  pthread_join(cocinero, NULL);
  pthread_join(mesero1, NULL);
  pthread_join(mesero2, NULL);

  //Imprime en consola el valor del contador final
  printf("Contador %d \n", counter);
  printf("Fin del programa...\n");

  sem_destroy(&sem1); // Libera los recursos del semáforo al terminar

  return 0;
}

// cocinar: recibe un puntero genérico (arg) y devuelve NULL al terminar el hilo
static void * cocinar(void* arg) {
  for (int i = 0; i < NR_LOOP * 2; i++) //Bucle para cocinar (se multiplica por 2 para abastecer a \ambos meseros)
  {
    counter++;
    printf("COCINERO: Comida preparada \n");
    sem_post(&sem1); // Le avisa al mesero que ya hay plato listo
    sleep(1); // Espera 1 segundo antes de cocinar el siguiente
  }
  return NULL;
}

// servir: recibe el ID del mesero en (arg) y devuelve NULL al terminar el hilo
static void * servir(void* arg) {
  int id = *(int*)arg; // Convierte el puntero genérico a entero con el número de mesero
  for (int i = 0; i < NR_LOOP; i++) //Bucle para servir los platillo que se preparen
  {
    sem_wait(&sem1); // Espera a que el cocinero dé la señal antes de servir
    printf("MESERO %d: Comida servida \n", id);
  }
  return NULL;
}