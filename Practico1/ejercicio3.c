#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	int f,pid, ppid;
	
	f = fork();
	pid = getpid();
	ppid = getppid();
	
	if (f == 0) {
		//Proceso hijo 
		printf("Soy el proceso hijo. Mi PID es: %d. El PID de mi padre es: %d\n", pid, ppid);
	} else {
		//Proceso padre
		printf("Soy el proceso padre. Mi PID es: %d. El PID de mi padre es: %d\n", pid, ppid);
	}
	
	return 0;
}
