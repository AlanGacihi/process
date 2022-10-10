#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 10000

float MAXIMUM, MINIMUM;
char **filenames;

void *readfile(void *threadid)
{
   long taskid;
   char line[MAX_LINE_LENGTH];
   char *token;
   char delim[2] = " ";
   float conv, max, min;
   int j = 0;

   taskid = (long) threadid;

   /* Open file */
    FILE *file = fopen(filenames[taskid + 1], "r");

    fgets(line, MAX_LINE_LENGTH, file);

    /* Get first token */
    token  = strtok(line, delim);

    /* walk through other tokens */
    while(token != NULL)
    {
        conv = atof(token);
        if (j == 0) {
            max = conv;
            min = conv;
        }
        if (conv < min)
            min = conv;
        if (conv > max)
            max = conv;
        token = strtok(NULL, delim);
        j++;
    }

    if (taskid == 0) {
        MAXIMUM = max;
        MINIMUM = min;
    }

    if (min < MINIMUM)
        MINIMUM = min;

    if (max > MAXIMUM)
        MAXIMUM = max;

    /* Close file */
    if (fclose(file))
    {
        perror(filenames[taskid + 1]);
        exit(EXIT_FAILURE);
    }

   printf("%s SUM=%f DIF=%f MIN=%f MAX=%f\n", filenames[taskid + 1], min + max, min - max, min, max);
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
        rc = pthread_create(&threads[t], NULL, readfile, (void *) taskids[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    printf("MINIMUM=%f MAXIMUM=%f\n", MINIMUM, MAXIMUM);

    pthread_exit(NULL);
}