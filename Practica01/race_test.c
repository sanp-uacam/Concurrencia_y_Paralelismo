#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int global_counter = 20; // Variable compartida entre hilos
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Protege la sección crítica

void *thread_routine(void *arg){
    for (size_t i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&lock);
        global_counter++; // sección crítica
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *thread_routine_two(void *arg){
    for (size_t i = 0; i < 1000; i++)
    {
        pthread_mutex_lock(&lock);
        global_counter--; // sección crítica
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread_routine, NULL);
    pthread_create(&t2, NULL, thread_routine_two, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Valor final del contador: %d\n", global_counter);

    pthread_mutex_destroy(&lock);
    return 0;
}

//¿Qué es una condición de carrera?

//Una condición de carrera ocurre cuando dos o más hilos acceden 
//y modifican al mismo tiempo una variable compartida, sin ningún mecanismo que 
//coordine el orden de esos accesos. Como el resultado final depende de en qué 
//orden exacto el procesador entrelaza las instrucciones de cada hilo, y ese 
//orden no está garantizado, el programa se vuelve impredecible.

//¿Por qué puede producir resultados diferentes al ejecutar varias veces el programa?

//Porque la operación `global_counter++` (o `--`) no es una sola instrucción 
//atómica, sino tres pasos separados: leer el valor de memoria, sumarle/restarle 
//1 en un registro, y volver a escribirlo en memoria. El sistema operativo puede 
//pausar un hilo justo entre estos pasos y darle tiempo de CPU al otro hilo. Si 
//el segundo hilo lee el valor *antes* de que el primero termine de escribir su 
//resultado, ambos terminan trabajando sobre el mismo valor "viejo" y una de las 
//dos actualizaciones se pierde. Como el sistema operativo decide de forma no 
//determinista cuándo interrumpe a cada hilo, cada ejecución puede tener un 
//entrelazado distinto, y por eso el valor final cambia de una corrida a otra.

//¿Qué es una sección crítica y qué parte del código corresponde a ella?**

//Una sección crítica es el fragmento de código que accede a un recurso 
//compartido (en este caso, `global_counter`) y que debe ejecutarse de manera 
//atómica, es decir, sin que ningún otro hilo pueda interferir a la mitad. En 
//este programa, la sección crítica es exactamente la línea `global_counter++;` 
//dentro de `thread_routine` y la línea `global_counter--;` dentro de 
//`thread_routine_two`. Por eso, en la versión corregida, esas líneas quedan 
//encerradas entre `pthread_mutex_lock(&lock)` y `pthread_mutex_unlock(&lock)`, 
//garantizando que solo un hilo a la vez pueda ejecutarlas.

//`movl global_counter(%rip), %eax`: instrucción de **lectura**. Copia el 
//valor actual de la variable `global_counter` (que está en memoria) hacia el 
//registro `%eax` del procesador, para poder operar sobre él.

//`addl $1, %eax`**: instrucción de suma. Le suma 1 al valor que ya está 
//cargado en el registro `%eax`. Corresponde a la operación `global_counter++`.

//`subl $1, %eax`**: instrucción de resta. Le resta 1 al valor cargado 
//en `%eax`. Corresponde a la operación `global_counter--`.

//`movl %eax, global_counter(%rip)`**: instrucción de escritura. Toma 
//el valor ya modificado que está en el registro `%eax` y lo guarda de vuelta 
//en la variable `global_counter` en memoria.
