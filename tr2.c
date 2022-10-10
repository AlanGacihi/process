#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
 
// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
void *ThreadFun(void *vargp)
{
    char *filename;
    filename = (char *)vargp;
    printf("Printing GeeksQuiz from Thread %s\n", filename);
    pthread_exit(NULL);
}
  
int main()
{
    pthread_t thread_id;
    printf("Before Thread\n");
    pthread_create(&thread_id, NULL, ThreadFun, (void *)"Hello");
    //pthread_join(thread_id, NULL);
    printf("After Thread\n");
    exit(0);
}