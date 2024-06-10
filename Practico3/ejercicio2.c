#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 256

int main(int argc, char *argv[]){

	char buffer[MAX];
	int fd = open("data2.txt", O_RDONLY);
	
	if(fd == -1)
	{
		printf("Error al abrir el archivo\n");
		exit(0);
	}
	
	ssize_t n = read(fd, buffer, MAX);
	while(n > 0)
	{
		printf("%s",buffer);
		n = read(fd, buffer, MAX);
	}
	close(fd);

        return 0;
}

