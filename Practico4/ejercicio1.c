#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{	
	while(1)
	{
		printf(".\n");
		sleep(1);
	
		int fd = open("archivo.txt", O_RDONLY);
		if(fd != -1)
		{
			printf("El archivo existe\n");
			close(fd);
			break;
		}
	}

	return 0;
}
