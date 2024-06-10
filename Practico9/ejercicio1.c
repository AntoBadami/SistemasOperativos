#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define TAMANO_CABECERA 24
#define TAMANO_ENTRADA_DIRECTORIO 12

// Función para leer el archivo y procesar su contenido
void leerArchivo(char *);

int main(int argc, char *argv[]) {

    // Llamamos a la función para leer el archivo
    leerArchivo(argv[1]);
    return 0;
}

void leerArchivo(char *nombre_archivo) {
    // Abrimos el archivo en modo de solo lectura
    int archivo = open(nombre_archivo, O_RDONLY);
    if (archivo < 0) {
        printf("Error al abrir el archivo.\n");
        exit(0);
    }

    char cabecera[TAMANO_CABECERA];
    
    // Leemos la cabecera del archivo
    if (read(archivo, cabecera, TAMANO_CABECERA) != TAMANO_CABECERA) {
        printf("Error al leer la cabecera.\n");
        close(archivo);
        exit(0);
    }

    // Obtenemos la longitud del registro, la dirección base y la longitud del directorio
    int longitud_registro = atoi(cabecera);
    int direccion_base = atoi(cabecera + 12);

    //calculamos longitud total del directorio
    int longitud_directorio = direccion_base - TAMANO_CABECERA - 1;

    // Reservamos memoria para el directorio y lo leemos desde el archivo
    char *directorio = (char *)malloc(longitud_directorio);
    if (read(archivo, directorio, longitud_directorio) != longitud_directorio) {
        printf("Error al leer el directorio.\n");
        free(directorio);
        close(archivo);
        exit(0);
    }

    // Calculamos la longitud de los datos y los leemos desde el archivo
    int longitud_datos = longitud_registro - direccion_base;
    char *datos = (char *)malloc(longitud_datos);
    lseek(archivo, direccion_base, SEEK_SET);
    if (read(archivo, datos, longitud_datos) != longitud_datos) {
        printf("Error al leer los datos.\n");
        free(directorio);
        free(datos);
        close(archivo);
        exit(0);
    }

    // Iteramos sobre el directorio para obtener las etiquetas y datos correspondientes
    int i;
    for (i = 0; i < longitud_directorio; i += TAMANO_ENTRADA_DIRECTORIO) {
        char etiqueta[4];
        strncpy(etiqueta, directorio + i, 3);
        etiqueta[3] = '\0';

        int longitud = atoi(directorio + i + 3);
        int inicio = atoi(directorio + i + 7);

        // Extraemos los datos del campo y los mostramos en la consola
        char *datos_campo = (char *)malloc(longitud + 1);
        strncpy(datos_campo, datos + inicio, longitud);
        datos_campo[longitud] = '\0';

        printf("Etiqueta: %s, Datos: %s\n", etiqueta, datos_campo);
        free(datos_campo);
    }

    // Liberamos la memoria utilizada y cerramos el archivo
    free(directorio);
    free(datos);
    close(archivo);

}