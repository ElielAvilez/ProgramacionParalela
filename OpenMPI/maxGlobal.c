// Funciones Básicas de Comunicación
// MPI_Send y MPI_ Recv.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
// #define tam 10

int idProc, nProc, i, tag = 99, primero, tamBloque, flag, tamNombre;
char nombre[30];

long max, maxParcial, maxGlobal;
MPI_Status status;

int main(int argc, char **argv)
{
    char archivo[40];
    strcpy(archivo, argv[2]);
    
    int tam = atoi(argv[1]);
    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Get_processor_name(nombre, &tamNombre);

    int resultadosParciales[nProc];

    FILE *lector = NULL;
    int i = 0, j = 0, min = 1000, max = -1, suma = 0;
    int contenido[40];
    float promedio = 0;

    for (i = 0; i < 40 ; i++)
        contenido[i] = 0;

    i = 0;

    nombre[tamNombre] = '\0';

    /* Calcula el tamaño de las partes a enviar a los otros */
    tamBloque = tam / nProc;
    if (idProc == nProc - 1)
    {
        /* Inicializa el vector a */
        printf("datos: ");
        lector = fopen(archivo, "r");

        while(feof(lector) == 0)
        {
            fscanf(lector, " %d", &contenido[i]);
            i++;
        }

        printf("\n");

        /* Envia los bloques de datos los otros procesos*/
        for (i = 0; i < nProc - 1; i++)
        {
            primero = i * tamBloque;
            MPI_Send(&contenido[primero], tamBloque, MPI_INT, i, tag, MPI_COMM_WORLD);
        }
        /* determina lo que debo sumar yo (nproc-1) */
        primero = idProc * tamBloque;
        /* efectuo mi búsqueda parcial */
        max = -1;
        for (j = 0; j < tam ; j++)
        {
            if(contenido[j] > max)
                max = contenido[j];
        }
        printf ("Mi máximo es: %d\n", max);
        resultadosParciales[idProc] = max;
            /* obtener las búsquedas parciales y calcula la total */
        for (i = 0; i < nProc - 1; i++)
        {
            MPI_Recv(&maxParcial, 1, MPI_LONG, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            printf("Respondio %d - envio: %10ld\n", status.MPI_SOURCE, maxParcial);

            resultadosParciales[status.MPI_SOURCE] = maxParcial;
        }

        maxGlobal = -1;

        for (j = 0; j < nProc ; j++)
        {
            if(resultadosParciales[j] > maxGlobal)
                maxGlobal = resultadosParciales[j];
        }
        printf("Máximo global = %10ld\n", maxGlobal);
    }
    else
    {
        /* recibe mi parte */
        MPI_Recv(contenido, tamBloque, MPI_LONG, nProc - 1, tag, MPI_COMM_WORLD, &status);
        /* efectua mi búsqueda parcial */
        maxParcial = -1;

        for (j = 0; j < tamBloque ; j++)
        {
            if(contenido[j] > maxParcial)
                maxParcial = contenido[j];
        }
        /* devuelve mi búsqueda parcial a (nproc - 1) */
        MPI_Send(&maxParcial, 1, MPI_LONG, nProc - 1, tag, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
