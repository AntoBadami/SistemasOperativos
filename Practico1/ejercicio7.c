#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

/* Variables globales */
int pid;

void sigmanager(int sig_num) {
		
	printf("señal: %d enviada al hijo\n", sig_num);
	kill(pid, sig_num);
	fflush(stdout);
}

void sonsigmanager(int sig_num) {

        if(sig_num == SIGINT)
	{
                printf("Hijo recibio señal %d, proceso terminado..\n", sig_num);
                exit(0);

        }
	else  
	{
        	printf("Hijo recibio señal %d\n", sig_num);
        }

}


int main() {
	int status;
	pid = fork();
	
	if(pid == 0)
	{
		/* hijo */
		signal(SIGINT, sonsigmanager);
		signal(SIGTSTP,sonsigmanager);
	}
	else
	{
		/* padre */
		signal(SIGINT, sigmanager);
		signal(SIGTSTP,sigmanager);
		if(wait(&status) == pid)
		{
			return 0;
		}
	}
	/* loop infinito */
	while(1){
		printf(".\n");
		sleep(1);
	}
	return 0;
}


