#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	int pid, wstatus;
	
	pid = fork();
	
	if (pid == 0) {
		//Proceso hijo 
		printf("Proceso hijo. Mi PID es: %d, en ejecucion\n", getpid());
		sleep(10);
		
	} else {
		//Proceso padre
		printf("Proceso padre. Mi PID es: %d, en ejecucion\n", getpid());
		wait(&wstatus);
		
		if(WIFEXITED(wstatus)){
			printf("El proceso hijo termino normalmente, estado: %d\n", WEXITSTATUS(wstatus));
		}else if(WIFSIGNALED(wstatus)){
			printf("El proceso hijo matado por la senial %d\n", WTERMSIG(wstatus));
		}
		printf("Proceso padre. Mi PID es: %d, finalizo\n", getpid());
		
	}
	
	return 0;
}
