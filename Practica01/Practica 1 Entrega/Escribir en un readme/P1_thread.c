#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void *thread_routine(void *arg){
    int num_line = *((int *)arg);
    int fd;
    char buf[] = "New line \n";

    printf("starting thread..");

    for (int i = 0; i < num_line; i++){
        fd = open("/readme.txt", 
            O_WRONLY | O_APPEND);
        write(fd, buf, sizeof(buf)-1);
        close(fd);
    }
}

int main (int argc, char const * args[]){

    int counter = 0;
    pthread_t thread_one;

    if (0 != pthread_create (&thread_one, NULL, thread_routine, &counter)){
        
    }

    return 0;

}