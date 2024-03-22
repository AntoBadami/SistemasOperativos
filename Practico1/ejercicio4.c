#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	int pid, wstatus;
	
	pid = fork();
	
	if (pid == 0) {
		//Proceso hijo 
		printf("Proceso hijo. Mi PID es: %d, en ejecucion\n", getpid());
		
	} else {
		//Proceso padre
		printf("Proceso padre. Mi PID es: %d, en ejecucion\n", getpid());
		int childout=wait(&wstatus);
		
		if(childout == -1){
			printf("El proceso hijo no termino correctamente\n");
		}else if(childout == pid){
			printf("El proceso hijo termino correctamente\n");
		}
		printf("Proceso padre. Mi PID es: %d, finalizo\n", getpid());
		
	}
	
	return 0;
}

