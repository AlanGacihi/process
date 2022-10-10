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
        char string[] = "Hello, world!\n";
        char buffer[80];
        char *filenames[] = {"dataset1", "dataset2", "dataset3"};
        char line[MAX_LINE_LENGTH];
        char readbuffer[80];
        char *token;
        float conv;
        float all[3][2];
        float min, max, maximum, minimum;
        const char delim[2] = " ";

        for (int i = 0; i < 3; i++) {

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
                FILE *file = fopen(filenames[i], "r");

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
                
                /* Wait for child process to finish*/
                wait(NULL);
                
                /* Parent process closes up output side of pipe */
                close(fd[1]);

                /* Read in a string from the pipe */
                nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                printf("Received string: %s\n", readbuffer);

                token = strtok(readbuffer, delim);
                while(token != NULL)
                {
                    conv = atof(token);
                    if (k == 3)
                        all[i][0] = conv;
                    if (k == 4)
                        all[i][1] = conv
                    token = strtok(NULL, delim);
                    k++;
                }
                printf("%f %f\n", all[i][0], all[i][1]);
            }
        }

        return(0);
}