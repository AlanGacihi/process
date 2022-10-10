#include <iostream>
#include <cstdlib>
#include <pthread.h>


void *readfile(void *f) {
   char *filename;
   filename = (char *)f;
   printf("%s\n", filename);
   pthread_exit(NULL);
}

int main () {
    int NUM_THREADS = argc - 1;
    pthread_t threads[NUM_THREADS];
    int rc;
    int i;
    char *filenames[] = {"dataset1", "dataset2", "dataset3"};

    for( i = 0; i < NUM_THREADS; i++ ) {
        rc = pthread_create(&threads[i], NULL, readfile, (void *)filenames[i]);
        if (rc) {
            printf("Error:unable to create thread, %d\n", rc);
            exit(-1);
        }
    }
    pthread_exit(NULL);
}