#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	8

float MAXIMUM, MINIMUM;
char *filenames[];

void *readfile(void *threadid)
{
   long taskid;

   sleep(1);
   taskid = (long) threadid;
   printf("File %s\n", taskid, filenames[taskid]);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int NUMTHREADS = argc -1;
    pthread_t threads[NUM_THREADS];
    long taskids[NUM_THREADS];
    int rc, t;

    for(t = 0; t < NUM_THREADS; t++) {
        taskids[t] = t;
        printf("Creating thread %d\n", t);
        rc = pthread_create(&threads[t], NULL, readfile, (void *) taskids[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    pthread_exit(NULL);
}