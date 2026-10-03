
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int global_counter = 20;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Primer hilo: incrementa el contador
void *thread_routine(void *arg)
{
    int num_line = *((int *)arg);
    int fd;
    char buf[] = "New line\n";

    printf("Starting thread...\n");

    pthread_mutex_lock(&mutex);

    for (int i = 0; i < num_line; i++)
    {
        global_counter++;

        fd = open("/mnt/c/Users/glady/Onedrive/Desktop/CyP_2026/readme.txt", O_WRONLY | O_APPEND);

        if (fd == -1)
        {
            perror("Error al abrir readme.txt");
            break;
        }

        if (write(fd, buf, sizeof(buf) - 1) == -1)
            perror("Error al escribir");

        close(fd);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Segundo hilo: disminuye el contador
void *thread_routine_two(void *arg)
{
    int num_line = *((int *)arg);
    int fd;
    char buf[] = "Gladys...\n";

    printf("Starting thread...\n");

    pthread_mutex_lock(&mutex);

    for (int i = 0; i < num_line; i++)
    {
        global_counter--;

        fd = open("/mnt/c/Users/glady/Onedrive/Desktop/CyP_2026/readme.txt", O_WRONLY | O_APPEND);

        if (fd == -1)
        {
            perror("Error al abrir readme.txt");
            break;
        }

        if (write(fd, buf, sizeof(buf) - 1) == -1)
            perror("Error al escribir");

        close(fd);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void)
{
    pthread_t thread_one;
    pthread_t thread_two;

    int counter = 1000;

    pthread_create(&thread_one, NULL, thread_routine, &counter);
    pthread_create(&thread_two, NULL, thread_routine_two, &counter);

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    printf("Contador final: %d\n", global_counter);

    pthread_mutex_destroy(&mutex);

    return 0;
}