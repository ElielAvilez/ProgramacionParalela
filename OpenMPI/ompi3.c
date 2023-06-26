#include <stdio.h>
#include <string.h>
#include <mpi.h>

int idproc, nproc, resultlen;
char nombre[30];

int main (int argc, char **argv)
{
    char archivos[4][40];
    
    strcpy(archivos[0], argv[1]);
    strcpy(archivos[1], argv[2]);
    strcpy(archivos[2], argv[3]);
    strcpy(archivos[3], argv[4]);

    MPI_Init(&argc, &argv);

    FILE *lector = NULL;
    int i = 0, j = 0, min = 1000, max = -1, suma = 0;
    int contenido[40];
    float promedio = 0;

    for (i = 0; i < 40 ; i++)
        contenido[i] = 0;

    i = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &idproc);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Get_processor_name(nombre, &resultlen);

    lector = fopen(archivos[idproc], "r");

    while(feof(lector) == 0)
    {
        fscanf(lector, " %d", &contenido[i]);
        i++;
    }

    fclose(lector);

    int cantidad = i;

    printf("Hola Mundo! Yo soy %d de %d corriendo en %s\n",idproc, nproc, nombre);
    printf("Yo me encargo de %s\n", archivos[idproc]);

    //obtener minimo y maximo
    for (i = 0; i < cantidad ; i++)
    {
        if(contenido[i] < min)
            min = contenido[i];

        if(contenido[i] > max)
            max = contenido[i];

        suma += contenido[i];
    }

    promedio = (float)suma / cantidad;

    printf("El máximo es %d y el mínimo es %d\n", max, min);

    char archivoSalida[50];

    strcat(strcpy(archivoSalida, "salida"), archivos[idproc]);

    printf("%s\n", archivoSalida);
    lector = fopen(archivoSalida, "w+");

    fprintf(lector, "El mínimo de %s es: %d\n", archivos[idproc], min);
    fprintf(lector, "El máximo de %s es: %d\n", archivos[idproc], max);
    fprintf(lector, "El promedio de %s es: %f\n", archivos[idproc], promedio);

    fclose(lector);
    printf("\n\n");

    MPI_Finalize();
}
