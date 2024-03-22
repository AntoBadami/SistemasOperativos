#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	int pid;
	
	pid = fork();
	
	if (f == 0) {
		//Proceso hijo 
		printf("Soy el proceso hijo\n");
	} else {
		//Proceso padre
		printf("Soy el proceso padre\n");
	}
	
	return 0;
}
