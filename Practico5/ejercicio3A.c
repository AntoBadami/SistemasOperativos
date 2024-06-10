//PROGRAM A
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#define MAX 30

int main(int argc, char const *argv[])
{
    int pid = getpid();
    printf("Process A PID%d\n",pid);
    //obtener hora
    while (1)
    {
        int fd = open("com", O_WRONLY|O_CREAT, 0644);
        if(fd == -1)
        {
            printf("Error en la creacion del archivo\n");
            exit(1);
        }
        time_t t = time(NULL);
        char *date = ctime(&t);

        write(fd,date,MAX);

        printf("Process A -> DATE commit\n");
        close(fd);
        sleep(5);
        
    }
    



    return 0;
}