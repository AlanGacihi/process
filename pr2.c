/*
Family Name:
Given Name(s):
Student Number:
EECS Login ID (the one you use to access the red server):
YorkU email address (the one that appears in eClass):
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LINE_LENGTH 10000

int main(void)
{
        int fd[2], nbytes;
        pid_t childpid;
        char buffer[80];
        char *filenames[] = {"dataset1", "dataset2", "dataset3"};
        char line[MAX_LINE_LENGTH];
        char readbuffer[80];
        char *token;
        float conv;
        float all[3][2];
        float min, max, maximum, minimum;
        const char delim[2] = " ";

        for (int i = 1; i < argc; i++) {

            pipe(fd);

            if((childpid = fork()) == -1)
            {
                perror("fork");
                exit(1);
            }

            if(childpid == 0)
            {
                int j = 0;
                /* Open file */
                FILE *file = fopen(argv[i], "r");

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

                /* Close file */
                if (fclose(file))
                {
                    return EXIT_FAILURE;
                    perror(filenames[i]);
                }

                /* Child process closes up input side of pipe */
                close(fd[0]);

                /* Send data to the main pipe */
                sprintf(buffer, "%f %f %f %f", min + max, min - max, min, max);
                write(fd[1], buffer, (strlen(buffer)+1));
                exit(0);
            }
            else
            {
                int k = 0;
                float m, n;
                              
                /* Parent process closes up output side of pipe */
                close(fd[1]);

                /* Read in a string from the pipe */
                nbytes = read(fd[0], readbuffer, sizeof(readbuffer));

                token = strtok(readbuffer, delim);
                while(token != NULL)
                {
                    conv = atof(token);
                    if (k == 2)
                        all[i][0] = conv;
                        m = conv;
                    if (k == 3)
                        all[i][1] = conv;
                        n = conv;
                    token = strtok(NULL, delim);
                    k++;
                }

                printf("%s SUM=%f DIF=%f MIN=%f MAX=%f\n", argv[i], all[i][0] + all[i][1], all[i][0] - all[i][1], all[i][0], all[i][1]);
            }
        }

        /* Wait for child processes to finish */
        for (int i = 0; i < 3; i++) {
            pid_t pid = wait(NULL);
        }

        /* Calculate overall maximum and minimum */
        for (int i = 0; i < 3 ; i++) {
            if (i == 0) {
                minimum = all[i][0];
                maximum = all[i][1];
            }
            if (all[i][0] < minimum)
                minimum = all[i][0];
            if (all[i][1] > maximum)
                maximum = all[i][1];
        }

        printf("MINIMUM=%f MAXIMUM=%f\n", minimum, maximum);
        
        return(0);
}