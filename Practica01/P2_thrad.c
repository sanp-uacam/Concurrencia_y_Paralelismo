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
    char buf[] = "oa chat soy io\n";

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

}