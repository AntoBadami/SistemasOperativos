#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define VERB_MOD "-v"
#define SELECT_METHOD "-a"
#define PI "PI"
#define PD "PD" 
#define MEMORY_C 64 //tamaño de memoria
#define STATIC_PARTITION 8 //Tamaño de las particiones estaticas

typedef struct {
    int proc_num;
    int tiempo_arribo;
    int tam;
    int tiempo_final;
    int loaded; //indica si esta cargado en memoria
    int selected;
} Proceso;

typedef struct 
{
    int estado;//Libre - Ocupado
    int tam;//Tamaño en mb
    Proceso *duenio;
} Particion;

int verbMod = 0; //flag de modo verboso

int barPosition(char const *[], int, int);
void imprimirProcesos(Proceso *, int);
void memoryState(Particion *, int);
int getProgramInputData(const char*, Proceso**);
void piSimulation(Proceso**, int);
void pdSimulation(Proceso** , int);
float memoryUsage(Particion *, int);
float memoriaLibre(Particion *, int);

int main(int argc, char const *argv[])
{
    Proceso *procesos = NULL;
    int process_cont = getProgramInputData(argv[argc - 1], &procesos);

    //recuperamos parametros de entrada
    int barpos = barPosition(argv,argc,0);
    if(barpos == -1)
    {
        printf("No se selecciono ningun modo de operacion\n");
        exit(-1);
    }

    if(strcmp(argv[barpos],VERB_MOD) == 0)
    {
        printf("Modo verboso activado\n");
        verbMod = 1;
        barpos = barPosicion(argv,argc,barpos);
    }
    if (strcmp(argv[barpos], SELECT_METHOD) == 0)
    {
        //eleccion de algoritmo

        //printf("Se selecciona un metodo\n");

        if(strcmp(argv[barpos + 1], PI) == 0 ) //particiones fijas del mismo tamaño
        {
            printf("PI: Particiones fijas, mismo tam:\n");
            piSimulation(&procesos,process_cont);
        }
        else if(strcmp(argv[barpos + 1], PD) == 0)
        {
            printf("PD: Particiones fijas, diferente tam:\n");
            pdSimulation(&procesos,process_cont);
        }
        else // Metodo no reconocido
        {
            printf("Metodo no reconocido\n");
        }
    }
    
    free(procesos);

    return 0;
}

int barPosicion(char const *argv[], int argc, int inicial)
{
    for(int i = inicial+1; i < argc; i++)
    {
        if(*argv[i] =='-')
        {
            return i;
        }
    }
    return -1;
}

//recupera la informacion del archivo de entrada
int getProgramInputData(const char *filename, Proceso **procesos)
{
    FILE *archivo;
    char c = 233;
    archivo = fopen(filename,"r");
    if(archivo == NULL)
    {
        printf("fallo en abrir el archivo \n");
        exit(1);
    }

    //iniciamos vector
    *procesos = (Proceso *)malloc(sizeof(Proceso));
    if(*procesos == NULL)
    {
        printf("Fallo en asignacion de memoria\n");
        exit(2);
    }
    printf("Datos recuperados:\n");
    int i = 0;
    while (fscanf(archivo, "%d %d %d %d", &(*procesos)[i].proc_num, &(*procesos)[i].tiempo_arribo, &(*procesos)[i].tam, &(*procesos)[i].tiempo_final) == 4) {
        
        (*procesos)[i].loaded =0;
        (*procesos)[i].selected=0;
        printf("%d %d %d %d\n",(*procesos)[i].proc_num, (*procesos)[i].tiempo_arribo, (*procesos)[i].tam, (*procesos)[i].tiempo_final);
        i++;
        *procesos = (Proceso *)realloc(procesos,(i+1) * sizeof(Proceso));
        if(*procesos == NULL)
        {
            printf("Fallo en asignacion de memoria\n");
            exit(2);
        }
    }
    fclose(archivo);
    printf("Cantidad de procesos cargados: %d\n", i);
   
    return i;
}

//Se encarga de simular el metodo de particiones fijas del mismo tamanio
void piSimulation(Proceso** procesos, int contador_procesos)
{
    //imprime informacion de procesos
    if(verbMod)
    {
        printf("Procesos que ingresan a la simulacion:\n");
        imprimirProcesos(*procesos,contador_procesos);
    }

    //iniciamos memoria:
    int units = (int) MEMORY_C / STATIC_PARTITION;

    Particion particiones[units];
    printf("Memoria total: %d\n\n",MEMORY_C);
    printf("Numero de particiones: %d\n\n",units);
    printf("Tamaño particion: %d\n\n",STATIC_PARTITION);

    for(int i =0; i<units;i++)
    {
        particiones[i].estado = 0;
        particiones[i].tam = STATIC_PARTITION;
    }

    //ejecutar simulacion:
    Proceso SO = {0};

    //ocupamos una con SO
    particiones[0].estado=1;
    particiones[0].duenio = &SO;
    particiones[0].duenio->tam=8;
    particiones[0].duenio->loaded= 1;
    particiones[0].duenio->selected=1;

    //inicializamos estados
    int process_left = contador_procesos;
    int localizado;

    //puntero a procesos
    Proceso * processptr = (Proceso *)malloc(sizeof(Proceso)), *ptrtofree;
    ptrtofree = processptr; //puntero auxiliar para liberar memoria cuando ya no sea requerida
    processptr->selected=0;
    processptr->loaded = 0;

    //variables estadisticas
    float frag_int = 0;
    int fallos_asig = 0;
    float uso_mem = 0;
    float frag_ext = 0;
    int time;

    printf("----------------------------Ejecutando simulacion------------------------------\n\n");

    for(time = 0; process_left > 0; time++)
    {
        if(!(*processptr).selected)
        {
            localizado = 0;
            for(int i = 0; i<contador_procesos; i++)
            {
                if(!(*procesos)[i].selected && !(*procesos)[i].loaded)//no fue seleccionado
                {
                    if(time >= (*procesos)[i].tiempo_arribo)
                    {
                        //seleccionamos proceso
                        (*procesos)[i].selected = 1;
                        processptr = &(*procesos)[i];
                        if(verbMod)printf("T:%d >> seleccionado proceso nro: %d\n\n",time,processptr->proc_num);
                        //imprimirProcesos(&process_select,1);
                        localizado=1;
                        break;
                    }
                }
            }
            if(!localizado)
            {
                printf("No se encontraron procesos seleccionables en el tiempo %d\n",time);
            }
        }
        

        if(!(*processptr).loaded && processptr->selected)//el proceso fue seleccionado y no fue cargado
        {
            //first fit para buscar un hueco libre
            localizado=0;

            for(int i=0; i< units; i++)
            {
                if (!particiones[i].estado)
                {
                    //verificar si el proceso entra
                    if (particiones[i].tam < processptr->tam)//verificamos que proceso entre
                    {
                        if(verbMod)printf("T:%d >> proceso nro: %d no asignado a particion nro %d (%d / %dMB)\n\n",time,processptr->proc_num,i,processptr->tam,particiones[i].tam);
                        //tenemos memoria libre pero no es posible asignarla
                        //exit ya que no puede haber procesos de mas de 8mb en este simulador
                        printf("Proceso excede los 8MB abortando simulacion\n\n");
                        exit(0);
                    }
                    else if(!localizado)
                    {
                        processptr->loaded=1;
                        processptr->selected = 0; // ya podemos seleccionar otro proceso
                        particiones[i].duenio = processptr;
                        //imprimimos proceso duenio
                        //imprimirProcesos(&(particiones[i].duenio),1);
                        particiones[i].estado = 1; // fijamos la particion como ocupada
                        if(verbMod)printf("T:%d >> proceso %d asignado, particion nro %d\n\n",time, particiones[i].duenio->proc_num,i);
                        //fragmentacion interna
                        frag_int += particiones[i].tam - particiones[i].duenio->tam;
                        localizado =1;

                    }
                    
                }
                
            }

            if (!localizado)
            {
                //printf("No se encontro espacio libre en el tiempo de ejecucion %d, proceso %d sigue esperando\n", time,processptr->proc_num);
                fallos_asig++;
                //memoryState(particiones,units);
            }
        }
        

        //liberar procesos que ya terminaron o reducir su tiempo restante
        for (int i = 1; i < units; i++)
        {
            if(particiones[i].estado == 1)
            {

                if(particiones[i].duenio->tiempo_final > 0) //reducimos contador de tiempo
                {
                    particiones[i].duenio->tiempo_final--;
                }
                else if(particiones[i].duenio->tiempo_final == 0)
                {
                    if(verbMod)printf("T:%d >> finaliza proceso %d\n\n",time,particiones[i].duenio->proc_num);
                    particiones[i].estado= 0;
                    process_left--;

                }
            }
        }
        
        //porcentaje de memoria usado
        uso_mem += memoryUsage(particiones,units);
        //printf("uso de memoria %.2f\n",uso_mem);
    }
    printf("----------------------------Finalizo simulacion------------------------------\n\n");    
    free(ptrtofree);

    printf("Estadisticas simulacion PI:\n");
    printf("-Tiempo de ejecucion: %d\n",time-1);
    printf("-Fallos de asignacion totales: %d\n",fallos_asig);
    
    
    //Calculos estadisticos
    uso_mem /= (time-1);
    printf("-Promedio de memoria usado durante la ejecucion %2.f%%\n",uso_mem);
    frag_int /= contador_procesos;
    printf("-Fragmentacion interna promedio: %.2f MB\n",frag_int);

    //no se da la fregmentacion externa en este metodo dado que los procesos no exceden los 8 MB
    printf("-Fragmentacion externa promedio: %.2f MB\n\n",frag_ext);

}

//Se encarga de simular el metodo de particiones fijas de diferente tamaño
void pdSimulation(Proceso** procesos, int contador_procesos)
{
    //imprimo informacion de los procesos
    if(verbMod)
    {
        printf("Procesos que ingresan a la simulacion:\n\n");
        imprimirProcesos(*procesos,contador_procesos);
    }

    //iniciamos memoria:
    /*Particiones definidas:
        2 * 2MB
        4 * 4MB
        2 * 6MB
        4 * 8MB
    */
    int units = 12;
    Particion particiones[units];
    printf("Memoria total: %d\n\n",MEMORY_C);
    printf("Numero de particiones: %d\n\n",units);
    for(int i =0; i<units;i++)
    {
        if(i <= 1)
        {
            particiones[i].tam = 2;
        }
        if(i > 1 && i <=5)
        {
            particiones[i].tam = 4;
        }
        
        if(i > 5 && i <=7)
        {
            particiones[i].tam = 6;
        }

        if(i > 7)
        {
            particiones[i].tam= 8;
        }

        particiones[i].estado = 0;
    }

    //ejecutar simulacion
    Proceso SO ={0};

    //ocupamos una con SO
    particiones[11].estado=1;
    particiones[11].duenio = &SO;
    particiones[11].duenio->tam=8;
    particiones[11].duenio->loaded= 1;
    particiones[11].duenio->selected=1;

    //inicializamos estados
    int process_left = contador_procesos;
    int localizado;

    //puntero a procesos
    Proceso * processptr = (Proceso *)malloc(sizeof(Proceso)), *ptrtofree;
    ptrtofree = processptr; //puntero auxiliar para liberar memoria cuando ya no sea requerida
    processptr->selected=0;
    processptr->loaded = 0;

    //variables estadisticas
    float frag_int = 0;
    float frag_ext=0;
    int mem_noasig=0;
    int memlib_prom=0;
    int fallos_asig = 0;
    float uso_mem = 0;
    int time;

    if(verbMod)printf("----------------------------Ejecutando simulacion------------------------------\n\n");
    for(time =0;process_left > 0;time++)
    {
        //memoryState(particiones,units);
        
        if(!(*processptr).selected) // si el proceso no fue cargado
        {
            //busco proceso que no este cargado en memoria y que no haya sido seleccionado
            localizado=0;
            for(int i =0; i<contador_procesos;i++)
            {
                if(!(*procesos)[i].selected && !(*procesos)[i].loaded)//no fue seleccionado
                {
                    if(time >= (*procesos)[i].tiempo_arribo)
                    {
                    //seleccionamos proceso
                        (*procesos)[i].selected = 1;
                        processptr = &(*procesos)[i];
                        if(verbMod)printf("T:%d >> seleccionado proceso nro: %d\n\n",time,processptr->proc_num);
                        //imprimirProcesos(processptr,1);
                        localizado=1;
                        break;
                    }
                }
            }
            if(!localizado)
            {
                //printf("Advertencia: No se encontraron procesos seleccionables en el tiempo %d\n",time);
            }

        }


        if(!(*processptr).loaded && processptr->selected)//el proceso fue seleccionado y no fue cargado
        {

            //first fit para buscar un hueco libre
            localizado=0;
            for(int i =0; i<units;i++)
            {
                if(!particiones[i].estado) //particion esta libre
                {

                    //detectamos posibles fallos de asignacion
                    if(particiones[i].tam < processptr->tam)//verificamos que proceso entre
                    {
                        if(verbMod)printf("T:%d >> proceso nro: %d no asignado a particion nro %d (%d / %dMB)\n\n",time,processptr->proc_num,i,processptr->tam,particiones[i].tam);
                        //tenemos memoria libre pero no asignable
                        mem_noasig+= particiones[i].tam;
                    }
                    else if(!localizado)
                    {
                        processptr->loaded=1;
                        processptr->selected = 0; // ya podemos seleccionar otro proceso0
                        particiones[i].duenio = processptr;
                        //imprimimos proceso duenio
                        //imprimirProcesos(&(particiones[i].duenio),1);
                        particiones[i].estado = 1; // fijamos la particion como ocupada
                        if(verbMod)printf("T:%d >> proceso %d asignado, particion nro %d\n\n",time, particiones[i].duenio->proc_num,i);
                        //fragmentacion interna
                        frag_int += particiones[i].tam - particiones[i].duenio->tam;
                        localizado =1;

                    }
                }
                
            }

            if (!localizado)
            {
                //printf("No se encontro espacio libre en el tiempo de ejecucion %d, proceso %d sigue esperando\n", time,processptr->proc_num);
                fallos_asig++;
                //memoryState(particiones,units);
            }
            
           
        }
        

    
        //liberar procesos que ya terminaron o reducir su tiempo restante
        for(int i = 0; i< units-1;i++)
        {
            if(particiones[i].estado == 1)
            {

                if(particiones[i].duenio->tiempo_final > 0) //reducimos contador de tiempo
                {
                    particiones[i].duenio->tiempo_final--;
                }
                else if(particiones[i].duenio->tiempo_final == 0)
                {
                    if(verbMod)printf("T:%d >> finaliza proceso %d\n\n",time,particiones[i].duenio->proc_num);
                    particiones[i].estado= 0;
                    process_left--;

                }
            }
        }
        
        //porcentaje de memoria usado
        uso_mem += memoryUsage(particiones,units);
        //printf("uso de memoria %.2f\n",uso_mem);
        memlib_prom += memoriaLibre(particiones,units);
    }
    if(verbMod)printf("----------------------------Finaliza simulacion--------------------------------\n\n");
    free(ptrtofree);

    printf("Estadisticas simulacion PD:\n");
    printf("-Tiempo de ejecucion: %d\n",time-1);
    printf("-Fallos de asignacion totales: %d\n",fallos_asig);
    
    
    //Calculos estadisticos
    uso_mem /= (time-1);
    printf("-Promedio de memoria usado durante la ejecucion %2.f%%\n",uso_mem);
    frag_int /= contador_procesos;
    printf("-Fragmentacion interna promedio: %.2f MB\n",frag_int);
    memlib_prom /= (time-1);

    /*----------en desarrollo
    frag_ext = (float)memlib_prom / fallos_asig;
    printf("-Fragmentacion externa promedio: %.2f MB\n\n",frag_ext);*/

    //---------alternativa mejor
    frag_ext = (float)mem_noasig / contador_procesos;
    printf("-Fragmentacion externa promedio: %.2f MB\n\n",frag_ext);
}

void imprimirProcesos(Proceso *procesos, int cantidad) {
    
    for (int i = 0; i < cantidad; i++) {
        printf("  Numero de Proceso: %d\n", procesos[i].proc_num);
        printf("  Tiempo de Arribo: %d\n", procesos[i].tiempo_arribo);
        printf("  Tamaño: %d\n", procesos[i].tam);
        printf("  Tiempo Final: %d\n", procesos[i].tiempo_final);
        printf("\n");
    }
}

void memoryState(Particion *particiones, int cantidad) {
    printf("Estado de Memoria:\n");
    for (int i = 0; i < cantidad; i++) {
        printf("Particion %d:\n", i);
        printf("  Estado: %s\n", (particiones[i].estado == 0) ? "Libre": "Ocupada" );
        printf("  Tamaño: %d MB\n", particiones[i].tam);
        if (particiones[i].estado != 0) {
            printf("  Dueño del Proceso:\n");
            printf("    Numero de Proceso: %d\n", particiones[i].duenio->proc_num);
            printf("    Tiempo de Arribo: %d\n", particiones[i].duenio->tiempo_arribo);
            printf("    Tamaño del Proceso: %d MB\n", particiones[i].duenio->tam);
            printf("    Tiempo Final: %d\n", particiones[i].duenio->tiempo_final);
            printf("    Loaded: %d\n", particiones[i].duenio->loaded);
            printf("    Selected: %d\n", particiones[i].duenio->selected);
        }
        printf("\n");
    }
}

//Retorna el porcentaje actual usado de la memoria
float memoryUsage(Particion *particiones, int cantidad)
{
    int usado = 0;
    for (int i = 0; i < cantidad; i++) 
    {
        if (particiones[i].estado != 0)
        {
            usado += particiones[i].duenio->tam;
        }
    }
    //printf ("Usado %dMB\n",usado);
    return (float) (usado*100)/MEMORY_C;
}

float memoriaLibre(Particion *particiones, int cantidad)
{
    int usado = 0;
    for (int i = 0; i < cantidad; i++) 
    {
        if (particiones[i].estado != 0)
        {
            usado += particiones[i].duenio->tam;
        }
    }
    return MEMORY_C - usado;
}