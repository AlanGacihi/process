#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

float MAXIMUM, MINIMUM;
char **filenames;

void *readfile(void *threadid)
{
   long taskid;

   taskid = (long) threadid;
   printf("File %s\n", filenames[taskid + 1]);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int NUM_THREADS = argc -1;
    pthread_t threads[NUM_THREADS];
    long taskids[NUM_THREADS];
    int rc, t;

    filenames = argv;
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