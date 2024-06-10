#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAX 30

int main()
{
    int pd[2];
    pipe(pd);

  //obtener hora
        time_t t = time(NULL);
        char *date = ctime(&t);  //creamos procesos hijos:
    int pid1 = fork(),pid2;

    if(pid1 == 0)
    {
        //firstchild

        //obtener fecha
        time_t t = time(NULL);
        char *date = ctime(&t);

        //escribimos
        write(pd[1],date,MAX);

        exit(EXIT_SUCCESS);

    }
    else
    {
        pid2 =fork();

        if(pid2 == 0)
        {

            char buffer[MAX];
            read(pd[0],buffer,MAX);
            printf("Lectura: %s\n", buffer );
            exit(EXIT_SUCCESS);

        }
    }

    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    printf("Procesos hijos terminados\n");

    return 0;
}