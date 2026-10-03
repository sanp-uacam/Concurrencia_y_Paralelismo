#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int global_counter = 20;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_routine(void *arg)
{
    int num_line = *((int *)arg);
    int fd;
    char buf[] = "New line\n";

    printf("Starting thread...\n");

    for (int i = 0; i < num_line; i++)
    {
        global_counter++;
            fd = open("/mnt/c/Users/glady/Onedrive/Desktop/CyP_2026/readme.txt", O_WRONLY | O_APPEND);
             write(fd, buf, sizeof(buf) - 1);
            close(fd);
    }

    return NULL;
}

void *thread_routine_two(void *arg)
{
    int num_line = *((int *)arg);
    int fd;
    char buf[] = "Gladys...\n";

    printf("Starting thread...\n");

    for (int i = 0; i < num_line; i++)
    {
        global_counter--;
            fd = open("/mnt/c/Users/glady/Onedrive/Desktop/CyP_2026/readme.txt", O_WRONLY | O_APPEND);
            write(fd, buf, sizeof(buf) - 1);
            close(fd);
    }

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

    return 0;
}