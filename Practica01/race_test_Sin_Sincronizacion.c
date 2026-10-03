#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int global_counter = 20;

void *thread_routine_1(void *arg){

    int num_line = *((int*)arg);

    int fd;

    char buf [100];

    printf("Starting thread...");

    for(int i=0; i<num_line; i++){
        global_counter++;
        fd = open("/mnt/c/Users/leidy/Concurrencia_y_Paralelismo/Practica01/readme2.txt", O_WRONLY | O_APPEND);
        sprintf(buf, "Thread_One: Global counter = %d\n",global_counter);

        write(fd, buf, strlen(buf));

        close(fd);
    }
}
    
void *thread_routine_2(void *arg){

    int num_line = *((int*)arg);
    int fd;
    char buf[100];

    printf("Starting second thread...");
    
    for(int i=0; i<num_line; i++){
        global_counter--;
        fd = open("/mnt/c/Users/leidy/Concurrencia_y_Paralelismo/Practica01/readme2.txt", O_WRONLY | O_APPEND);
        sprintf(buf, "Thread_Two: Global counter = %d\n",global_counter);
        write(fd, buf, strlen(buf));
        close(fd);
    }
    
    return NULL;
}

int main(int argc, char const *argv[]){

    pthread_t thread_one;
    pthread_t thread_two;
    
    int counter = 0;
    counter = atoi(argv[1]);
    int fd;
    char buf[100];

    if(0!=pthread_create(&thread_one, NULL, thread_routine_1,&counter))
    return 1;
    if(0!=pthread_create(&thread_two, NULL, thread_routine_2,&counter))
    return 1;

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);
    
    return 0;
}