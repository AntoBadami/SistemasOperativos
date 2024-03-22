#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigmanager(int sig_num){
	//reasigna la senial
	if(sig_num == SIGINT){
		printf("Progama interrumpido\n");
		exit(1);

	}else if(sig_num == SIGTSTP) {
		
		printf("Programa suspendido\n");
		fflush(stdout); 
	}
}


int main() {
        signal(SIGINT, sigmanager);
        signal(SIGTSTP,sigmanager);
        printf("Ingresa algo por teclado:\n");
        while(1);

        return 0;
}

