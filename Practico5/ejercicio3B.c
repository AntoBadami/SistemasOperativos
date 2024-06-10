#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 30

int main()
{
    int pid = getpid();
    printf("Process A PID%d\n",pid);
    
    char buffer[MAX];
    printf("esperando comunicacion\n");
    while (1)
    {
        
        printf(".\n");
        int fd = open("com", O_RDONLY, 0644);
        if(fd == -1)
        {
            printf("Esperando creacion de archivo\n");
        }
        ssize_t n = read(fd,buffer, MAX);
        if(n > 0)
        {
            printf("%s\n",buffer);
        }
        close(fd);
        sleep(5);
    }
    
    
    return 0;
}