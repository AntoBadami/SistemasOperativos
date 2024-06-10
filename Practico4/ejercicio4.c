#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_MESSAGE_SIZE 100

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } 
	else if (pid == 0) 
	{ // Proceso hijo (B)
        // Espera a que exista el archivo
        while (access("comunicador.txt", F_OK) == -1) {
            sleep(1);
        }

        // Lee el mensaje del archivo
		char message[MAX_MESSAGE_SIZE];
		int fd = open("comunicador.txt", O_RDONLY);
		read(fd, message, MAX_MESSAGE_SIZE);
		close(fd);

        // Sobreescribe el archivo con un mensaje de respuesta
		fd = open("comunicador.txt", O_WRONLY);
		write(fd, "Respuesta de B", strlen("Respuesta de B"));
		close(fd);
    } 
	else 
	{ // Proceso padre (A)
        // Crea el archivo y escribe un mensaje inicial
		int fd = open("comunicador.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		write(fd, "Mensaje de A", strlen("Mensaje de A"));
		close(fd);

        // Lee periodicamente el archivo y espera a que el mensaje cambie
        char message[MAX_MESSAGE_SIZE];
        while (1) {
            sleep(1);

            // Lee el mensaje del archivo
			fd = open("comunicador.txt", O_RDONLY);
			read(fd, message, MAX_MESSAGE_SIZE);
			close(fd);

            // Comprueba si el mensaje ha cambiado
            if (strcmp(message, "Respuesta de B") == 0) {
                printf("El mensaje ha cambiado a \"%s\". Proceso A termina.\n", message);
                break;
            }
        }

        // Espera a que el proceso hijo (B) termine antes de eliminar el archivo
        wait(NULL);
        remove("comunicador.txt");
    }

    return 0;
}

