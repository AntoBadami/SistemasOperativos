#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

int main(int argc, char const *argv[])
{
    printf("Nombre de archivo ingresado: %s \n",argv[1]);

    struct stat stat_buf; 
    if(stat(argv[1], &stat_buf) < 0)
    { 
        printf("Error en stat\n");
        exit(0);
    }

    //imprimimos datos del archivo
    printf("st_ino: %ld\n", stat_buf.st_ino);
    printf("st_nlink: %ld\n", stat_buf.st_nlink);
    printf("st_uid: %d\n", stat_buf.st_uid);
    printf("st_gid: %d\n", stat_buf.st_gid);
    printf("st_size: %ld\n", stat_buf.st_size);
    printf("st_blksize: %ld\n", stat_buf.st_blksize);
    printf("st_blocks: %ld\n", stat_buf.st_blocks);


    //fecha
    printf("%s",ctime(&stat_buf.st_atime));

    //permisos
    printf("%#o\n", stat_buf.st_mode & ~(S_IFMT));


    //macros
    if (S_ISLNK(stat_buf.st_mode))
        printf("Tipo de archivo: Enlace simbólico\n");
    else if (S_ISREG(stat_buf.st_mode))
        printf("Tipo de archivo: Archivo regular\n");
    else if (S_ISDIR(stat_buf.st_mode))
        printf("Tipo de archivo: Directorio\n");
    else if (S_ISCHR(stat_buf.st_mode))
        printf("Tipo de archivo: Dispositivo de caracteres\n");
    else if (S_ISBLK(stat_buf.st_mode))
        printf("Tipo de archivo: Dispositivo de bloques\n");
    else if (S_ISFIFO(stat_buf.st_mode))
        printf("Tipo de archivo: FIFO/pipe\n");
    else if (S_ISSOCK(stat_buf.st_mode))
        printf("Tipo de archivo: Socket\n");
    else
        printf("Tipo de archivo: Desconocido\n");


    printf("numeros mayor y menor correspondiente al dispositivo fisico %ld - %ld\n", MAJOR(stat_buf.st_dev), MINOR(stat_buf.st_dev));

    return 0;
}
