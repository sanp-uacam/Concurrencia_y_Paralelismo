//Practica de semaforos - Autor: Leidy M Morayta Pérez - 08/09/2026.

//Importacion de librerias para utilizar los hilos, los semaforos , impresion en consola y los tiempo de esepra de los hilos.
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NR_LOOP 12 //Numero de vueltas (platos a preparar y servir)
#define NR_MESEROS 2 //Numero de meseros, que se utiliza para dividir el numero de platos entre los meseros, para que no haya un error


//Declaracion de los metodos cocinar y servir, que seran ejecutados por los hilos creados en el metodo main
static void * cocinar(void* arg);
static void * servir(void* arg);


static int counter = 0; //declaracion del contador de platos a cocinar y servir en 0 
sem_t sem1;// declaracion del semaforo que se utilizara para controlar el acceso al contador de platos

//metodo main donde declaramos los hilos y el semaforo, ademas de crear los hilos y esperar a que terminen su ejecucion
int main(void)
{
 //declaracion de los hilos cocinero y mesero, ademas de inicializar el semaforo en 0, 
 //para que el mesero no pueda servir hasta que el cocinero haya preparado un plato
  pthread_t cocinero, mesero, mesero2;
  sem_init(&sem1, 0, 0); 

//creacion de los hilos cocinero y mesero, que ejecutaran los metodos cocinar y servir respectivamente
  pthread_create (&cocinero, NULL, *cocinar, NULL);
  pthread_create (&mesero, NULL, *servir, NULL);
  pthread_create (&mesero2, NULL, *servir, NULL);

//espera a que los hilos terminen su ejecucion antes de finalizar el programa.
  pthread_join(cocinero, NULL);
  pthread_join(mesero, NULL);
  pthread_join(mesero2, NULL);

  printf("Contador %d \n", counter);

  return 0;
}

//metodo cocinar
static void * cocinar(void* arg) {
    
//bucle for que se ejecuta NR_LOOP veces, incrementando el contador de platos y
// liberando el semaforo para que el mesero pueda servir
  for (int i = 0; i < NR_LOOP; i++)
  {
    //sem_wait(&sem1);
    counter++;
    printf("COCINERO: Comida preparada. Pendientes:  %d \n", counter);
    sem_post(&sem1);//aqui se libera el semaforo
    sleep(1); //declara el tiempo de espera entre ejecucion del metodo cocinar en 1 mas rapido que el mesero
  }
  
}

// metodo servir  
static void * servir(void* arg) {

    //bucle for que se ejecuta NR_LOOP/NR_MESEROS veces, decrementando el contador de platos 
    //esperando a que el cocinero prepare un plato antes de servir
    for (int i = 0; i < NR_LOOP/NR_MESEROS; i++)
  {
    sem_wait(&sem1); //aqui se espera a que el semaforo sea liberado por el cocinero
    counter--;
    printf("MESERO: Comida servida. Pendientes:  %d \n", counter);
    sleep(4);  //declara el tiempo de espera entre ejecucion del metodo servir en 4 mas lentos que el cocinero
  }
  
}
