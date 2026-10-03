#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
//tarea, declarar una variable global 

void *thread_routine(void *arg){
    int num_line = *((int*)arg);
    int fd;
    char buf[] = "New line \n";
    char buf2[] = "Aaron Chi Uc \n";

    printf("starting thread..");

    for (int i = 0; i < num_line; i++)
    {
        fd = open("/mnt/c/Users/aaron/CyP_2026/readme.txt", O_WRONLY | O_APPEND);
        write(fd, buf, sizeof(buf)-1);
        write(fd, buf2, sizeof(buf2)-1);
        close(fd);
    }

    return NULL;
}

int main (int argc, char const *argv[])
{
    pthread_t thread_one;
    int counter = 3;
    counter = atoi(argv[1]);

    if (0!=pthread_create(&thread_one, NULL, thread_routine, &counter))
        return -1;

    pthread_join(thread_one, NULL);

    return 0;
}