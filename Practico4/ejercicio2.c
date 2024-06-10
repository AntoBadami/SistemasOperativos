#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
        char *buffer = NULL;
	
	//comprueba existencia
	int fd = open("archivo2.txt", O_RDONLY);
	if(fd != -1)
        {
		printf("El archivo existe\n");
		exit(0);
        }
	close(fd);
	
	while(1)
        {
                printf(".\n");
                sleep(1);
		
		fd = open("archivo2.txt", O_RDONLY);

                ssize_t n = read(fd, buffer, 1);
		if(n > 0)
		{
			if(buffer == '0')
        	        {
				printf("0: fin de ejecucion\n");
                	        close(fd);
                        	exit(0);
                	}else if(buffer == '1')
			{
				printf("1\n");
			}else
			{
				printf("otro digito\n");
			}
		}
		
		close(fd);
        }

        return 0;
}

