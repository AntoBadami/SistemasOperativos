
#include <fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX 30
int main()
{
    // creamos los archivos
    int fd1 = open("a-inout", O_RDWR|O_CREAT, 0644);
    int fd2 = open("b-inout", O_RDWR|O_CREAT, 0644);

    if(fd1 == -1 || fd2 == -1)
    {
        printf("Error en la creacion de los archivos\n");
        exit(1);
    }
    //cerramos
    close(fd1);
    close(fd2);

    //creamos procesos hijos
    printf("F:%d\n",getpid());
    int pid1 = fork(), pid2;
    if(pid1 == 0)
    {
        //First child
        printf("C1 PID:%d\n",getpid());
        //apertura de archivos a para lectura, b escritura

        int a_inout = open("a-inout", O_RDONLY);
        int b_inout = open ("b-inout",O_WRONLY);



        //escribimos el archivo
        sleep(5);
        //obtener hora
        time_t t = time(NULL);
        char *date = ctime(&t);

        write(b_inout,date,MAX);
        close(a_inout);
        close(b_inout);

        printf("Hijo 1 terminado\n");
        exit(EXIT_SUCCESS);
        
    }
    else
    {
        pid2 = fork();
        if(pid2 == 0)
        {
            //Second child
            printf("C2 PID:%d\n",getpid());
            
            int a_inout = open("a-inout", O_WRONLY);
            int b_inout = open ("b-inout",O_RDONLY);
            
            
            char buffer[MAX];
            ssize_t n = read(b_inout, buffer, MAX); //n bits efectivos leidos
            printf("Esperando escritura\n");
            while (n <= 0)
            {
                printf(".\n");
                usleep(300000);
                n = read(b_inout,buffer,MAX);
            }

            printf("C2: b-inout read = %s\n",buffer);
            
            close(a_inout);
            close(b_inout);
            printf("Hijo 2 terminado\n");
            exit(EXIT_SUCCESS);

        }
    }
    // Esperar a que ambos hijos terminen
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    
    // Eliminar archivos
    remove("a-inout");
    remove("b-inout");

}
