#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int global_counter = 20;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_routine(void *arg) {
     int fd;
    char buf[] = "New line \n";

    for (size_t i = 0; i < 1000; i++) {
        pthread_mutex_lock(&mutex);   // Bloquea el acceso para otros hilos
        global_counter++;             // Sección crítica
        pthread_mutex_unlock(&mutex);

        fd = open("/Users/vdj/Desktop/Cyp_2026/README.txt", O_WRONLY|O_APPEND);
        write(fd, buf, sizeof(buf)-1);
        close(fd); // Libera el acceso
    }
    return NULL;
}

void *thread_routine_two(void *arg) {
         int fd;
    char buf[] = "soy la mera verga en la programacion \n";

    for (size_t i = 0; i < 1000; i++) {
        pthread_mutex_lock(&mutex);   // Bloquea el acceso para otros hilos
        global_counter--;             // Sección crítica
        pthread_mutex_unlock(&mutex); // Libera el acceso

                fd = open("/Users/vdj/Desktop/Cyp_2026/README.txt", O_WRONLY|O_APPEND);
        write(fd, buf, sizeof(buf)-1);
        close(fd); // Libera el acceso
    }
    return NULL;
}

int main(int argc, char const *argv[]) {
    pthread_t thread_one;
    pthread_t thread_two;

    if (pthread_create(&thread_one, NULL, thread_routine, NULL) != 0)
        return -1;
    if (pthread_create(&thread_two, NULL, thread_routine_two, NULL) != 0)
        return -1;

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    printf("Valor final de global_counter: %d\n", global_counter);

    // Destrucción del mutex al finalizar
    pthread_mutex_destroy(&mutex);

    return 0;

    // Una sección crítica es cualquier parte del código donde se accede o modifica un recurso compartido (como una variable global,
    // lista o archivo) que varios hilos o procesos pueden usar al mismo tiempo.  El problema es que si dos hilos modifican ese recurso al mismo tiempo, 
    // las operaciones se pisan y los datos se corrompen (esto se llama condición de carrera). 
    // 
    //Por ejemplo, un incremento global_counter++ no se hace en un solo paso; la CPU primero lee el valor actual, lo suma y luego lo guarda. 
    // Si dos hilos leen el mismo valor inicial antes de guardarlo, el conteo final sale mal.  Para evitarlo, 
    // aplicamos exclusión mutua: usamos un mecanismo como un mutex para bloquear la sección crítica (pthread_mutex_lock). 
    // Así aseguramos que solo un hilo entre a la vez, 
    // modifique la variable y libere el paso (pthread_mutex_unlock) para los demás.  
}