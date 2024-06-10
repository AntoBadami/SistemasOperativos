#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 256

int main(int argc, char *argv[])
{
	printf("Nombre del archivo ingresado: %s\n",argv[1]);

	char buffer[MAX];
	
	int fd1 = open("test.txt", O_RDONLY);
	int fd2 = open("test2.bak", O_RDWR|O_CREAT, 0644);

	if(fd1 == -1 || fd2 == -1)
        {
                printf("Error al abrir el archivo\n");
                exit(0);
        }

	ssize_t n = read(fd1, buffer, MAX);
        while(n > 0)
        {
                printf("%s",buffer);
		write(fd2, buffer, n);

                n = read(fd1, buffer, MAX);
        }

	close(fd1);
	close(fd2);

	return 0;
}



