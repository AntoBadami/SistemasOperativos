#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define SELECT_METHOD "-a"
#define FIFO "FIFO"
#define LRU "LRU"
#define OPT "OPT"
#define RESULT_FILE "results.csv"

typedef struct 
{
    int marcos;
    char referencia[31];
} Entrada;

int barPosition(char const *[], int, int);
int getInputData(const char*, Entrada**);
void simulateFIFO(int, char*);
void simulateLRU(int, char*);
void simulateOPT(int, char*);
void saveResults(const char*, int, int);

int main(int argc, char const *argv[])
{
    Entrada *inputData;
    int simulaciones = getInputData(argv[argc - 1], &inputData);

    //recuperamos parametros de entrada
    int barpos = barPosition(argv,argc,0);
    if(barpos == -1)
    {
        printf("No se selecciono ningun algoritmo\n");
        exit(-1);
    }

    if (strcmp(argv[barpos], SELECT_METHOD) == 0)
    {
        //eleccion de algoritmo
        for(int i = 0; i < simulaciones; i++) {
			printf("\nSimulacion %d con %d marcos:\n", i+1, inputData[i].marcos);
			
			if(strcmp(argv[barpos + 1], FIFO) == 0 ) 
			{
				simulateFIFO(inputData[i].marcos, inputData[i].referencia);
			}
			else if(strcmp(argv[barpos + 1], LRU) == 0)
			{
				simulateLRU(inputData[i].marcos, inputData[i].referencia);
			}
			else if(strcmp(argv[barpos + 1], OPT) == 0)
			{
				simulateOPT(inputData[i].marcos, inputData[i].referencia);
			}
			else // Algoritmo no reconocido
			{
				printf("Algoritmo no reconocido\n");
			}
		}
    }
    
    free(inputData);

    return 0;
}

//recupera la informacion del archivo de entrada
int getInputData(const char *filename, Entrada **inputData)
{
	FILE *archivo = fopen(filename,"r");
	if(archivo == NULL)
	{
		printf("Fallo en abrir el archivo \n");
		exit(1);
	}
	
	// Iniciamos vector
	*inputData = (Entrada *)malloc(sizeof(Entrada));
	if(*inputData == NULL)
	{
		printf("Fallo en asignacion de memoria\n");
		exit(2);
	}
	
	int i = 0;
	while (fscanf(archivo, "%d %30s", &(*inputData)[i].marcos, (*inputData)[i].referencia) == 2)
	{
		i++;
		*inputData = (Entrada *)realloc(*inputData, (i+1) * sizeof(Entrada));
		if(*inputData == NULL)
		{
			printf("Fallo en asignacion de memoria\n");
			exit(2);
		}
	}
	fclose(archivo);
	
	return i;
}

int barPosition( char const *argv[], int argc, int inicial)
{   
    for(int i = inicial+1;i < argc; i++)
    {
        if(*argv[i] =='-')
        {
            return i;
        }

    }    
    return -1;   
}


void simulateFIFO(int marcos, char *referencia) {
	int *frames = (int*)malloc(marcos * sizeof(int));
	for (int i = 0; i < marcos; i++) frames[i] = -1;
	
	int pageFaults = 0, nextFrame = 0;
	
	for (int i = 0; i < 30; i++) {
		int page = referencia[i] - '0';
		int found = 0;
		
		for (int j = 0; j < marcos; j++) {
			if (frames[j] == page) {
				found = 1;
				break;
			}
		}
		
		if (!found) {
			frames[nextFrame] = page;
			nextFrame = (nextFrame + 1) % marcos;
			pageFaults++;
		}
		
		// Estado actual de los marcos
		for (int j = 0; j < marcos; j++) {
			if (frames[j] != -1) printf("%d", frames[j]);
			else printf("-");
		}
		if (!found) printf(" F"); // Indicar fallo de pagina
		printf("\n");
	}
	
	printf("Total de fallos de página: %d\n", pageFaults);
	saveResults(FIFO, marcos, pageFaults);
	free(frames);
}


void simulateLRU(int marcos, char *referencia) {
	int *frames = (int*)malloc(marcos * sizeof(int));
	int *ages = (int*)malloc(marcos * sizeof(int));
	for (int i = 0; i < marcos; i++) frames[i] = -1;
	
	int pageFaults = 0;
	
	for (int i = 0; i < 30; i++) {
		int page = referencia[i] - '0';
		int found = 0, lruIndex = 0;
		
		for (int j = 0; j < marcos; j++) {
			if (frames[j] == page) {
				found = 1;
				ages[j] = i;
				break;
			}
		}
		
		if (!found) {
			int oldest = i;
			for (int j = 0; j < marcos; j++) {
				if (frames[j] == -1) {
					lruIndex = j;
					break;
				}
				if (ages[j] < oldest) {
					oldest = ages[j];
					lruIndex = j;
				}
			}
			frames[lruIndex] = page;
			ages[lruIndex] = i;
			pageFaults++;
		}
		
		// Imprimir estado marcos
		for (int j = 0; j < marcos; j++) {
			if (frames[j] != -1) printf("%d", frames[j]);
			else printf("-");
		}
		if (!found) printf(" F"); //Indicar fallo de pagina
		printf("\n");
	}
	
	printf("Total de fallos de página: %d\n", pageFaults);
	saveResults(LRU, marcos, pageFaults);
	free(frames);
	free(ages);
}


void simulateOPT(int marcos, char *referencia) {
	int *frames = (int*)malloc(marcos * sizeof(int));
	for (int i = 0; i < marcos; i++) frames[i] = -1;
	
	int pageFaults = 0;
	
	for (int i = 0; i < 30; i++) {
		int page = referencia[i] - '0';
		int found = 0;
		
		for (int j = 0; j < marcos; j++) {
			if (frames[j] == page) {
				found = 1;
				break;
			}
		}
		
		if (!found) {
			int replaceIndex = -1, farthest = i;
			
			for (int j = 0; j < marcos; j++) {
				if (frames[j] == -1) {
					replaceIndex = j;
					break;
				}
				
				int k;
				for (k = i + 1; k < 30; k++) {
					if (frames[j] == referencia[k] - '0') break;
				}
				if (k > farthest) {
					farthest = k;
					replaceIndex = j;
				}
			}
			frames[replaceIndex] = page;
			pageFaults++;
		}
		
		//Imprimir estado de marcos
		for (int j = 0; j < marcos; j++) {
			if (frames[j] != -1) printf("%d", frames[j]);
			else printf("-");
		}
		if (!found) printf(" F"); // Fallo de pagina
		printf("\n");
	}
	
	printf("Total de fallos de página: %d\n", pageFaults);
	saveResults(OPT, marcos, pageFaults);
	free(frames);
}

void saveResults(const char *algorithm, int marcos, int pageFaults) {
	FILE *file = fopen(RESULT_FILE, "a");
	if (file == NULL) {
		printf("No se pudo abrir el archivo de resultados\n");
		return;
	}
	fprintf(file, "%s,%d,%d\n", algorithm, marcos, pageFaults);
	fclose(file);
}
