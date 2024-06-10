
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX 10

int main()
{
  
   int pid=fork();
   
   if (pid == -1) 
   {
        perror("fork");
        exit(EXIT_FAILURE);
   } 
   else if (pid == 0) 
   { // Proceso hijo (A)
        int fd;
        char a[MAX] = "0";

	// Abre el archivo y escribe el valor "0" una vez
        fd = open("note.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        write(fd, a, MAX);
        close(fd);
   }
   else
   { // Proceso padre (B)
   
   int fd;
        char aux[MAX];
        int buff;

        // Abre el archivo y lee el valor
        fd = open("note.txt", O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        read(fd, aux, MAX);
        buff = atoi(aux);
        close(fd);

        // Imprime un mensaje segun el valor leido
        if (buff == 0) {
            printf("ENCONTRE UN 0\n");
        }
		else
		{
			printf("NO ENCONTRE UN 0\n");
		}
   }

   return 0;
   
}
