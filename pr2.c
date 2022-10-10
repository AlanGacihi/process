#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define MAX_LINE_LENGTH 10000

int main(void)
{
        int fd[2], nbytes;
        pid_t childpid;
        char string[] = "Hello, world!\n";
        char buffer[80];
        char *filenames[] = {"dataset4", "dataset2", "dataset3"};
        char line[MAX_LINE_LENGTH];
        char readbuffer[80];
        char *token;
        float conv;
        int i = 0;
        float min, max, maximum, minimum;
        const char delim[2] = " ";


        pipe(fd);

        for (int i = 0; i < 1; i++) {

            if((childpid = fork()) == -1)
            {
                perror("fork");
                exit(1);
            }

            if(childpid == 0)
            {
                /* Open file */
                FILE *file = fopen(filenames[i], "r");

                fgets(line, MAX_LINE_LENGTH, file);

                /* Get first token */
                token  = strtok(line, delim);

                /* walk through other tokens */
                while(token != NULL)
                {
                    conv = atof(token);
                    if (i == 0) {
                        max = conv;
                        min = conv;
                    }
                    if (conv < min)
                        min = conv;
                    if (conv > max)
                        max = conv;
                    token = strtok(NULL, delim);
                    i++;
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
                sprintf(buffer, "%f %f %f %f\n", min + max, min - min, min, max);
                printf("%f %f\n", min, max);
                write(fd[1], buffer, (strlen(buffer)+1));
                exit(0);
            }
            else
            {
                /* Parent process closes up output side of pipe */
                close(fd[1]);

                /* Read in a string from the pipe */
                nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                printf("Received string: %s", readbuffer);
            }
        }

        return(0);
}