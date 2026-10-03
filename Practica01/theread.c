#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

//tarea, declarar una variable global

// Primer hilo
void *thread_routine(void *arg){
    int num_line = *((int*)arg);
    int fd;
    char buf[] = "New line \n";
    char buf2[] = "ALDAIR EDDIEL CANUL CABRERA \n";

    printf("starting one thread.. \n");

    for (int i = 0; i < num_line; i++)
    {
        fd = open("/mnt/c/concurrencia/Hola.txt", O_WRONLY | O_APPEND);
        if (fd != -1) {
            write(fd, buf, sizeof(buf)-1);
            write(fd, buf2, sizeof(buf2)-1);
            close(fd);
        }
    }

    return NULL;
}

// Segundo hilo
void *thread_routine_two(void *arg){
    int num_line = *((int*)arg);
    int fd;
    char buf[] = "este es el segundo hilo\n";

    printf("starting two thread.. \n");

    for (int i = 0; i < num_line; i++)
    {
        fd = open("/mnt/c/concurrencia/Hola.txt", O_WRONLY | O_APPEND);
        if (fd != -1) {
            write(fd, buf, sizeof(buf)-1);
            close(fd);
        }
    }

    return NULL;
}

int main (int argc, char const *argv[])
{
    pthread_t thread_one, thread_two;
    int counter = 3; 
    if (argc > 1) {
        counter = atoi(argv[1]);
    }

    if (0 != pthread_create(&thread_one, NULL, thread_routine, &counter)) {
        perror("Error al crear el hilo 1");
        return -1;
    }

    if (0 != pthread_create(&thread_two, NULL, thread_routine_two, &counter)) {
        perror("Error al crear el hilo 2");
        return -1;
    }

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);


    return 0;
}
