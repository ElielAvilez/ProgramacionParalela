// reducción calculado la Suma
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
// #define tam 100


long maxGlobal = -1, maxParcial = -1;

int idProc, nProc,i, tag = 99, primero, ultimo, bloque;
int contadorMax = 0, contadorMaxGlobal = 0, max = -1,
     contadorMin = 0, contadorMinGlobal = 0, min = 10000;
MPI_Status status;

int main(int argc, char **argv)
{
    char archivo[40];
    strcpy(archivo, argv[2]);

    int tam = atoi(argv[1]);
    int datos[tam];

    /* Rellena el vector datos */
    FILE *lector = NULL;
    int i = 0, j = 0, suma = 0;

    for (i = 0; i < tam ; i++)
        datos[i] = 0;

    i = 0;

    lector = fopen(archivo, "r");

    while(feof(lector) == 0)
    {
        fscanf(lector, " %d", &datos[i]);
        i++;
    }

    for (i = 0; i < tam; i++)
    {
        if(datos[i] > max) max = datos[i];
        if(datos[i] < min) min = datos[i];
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    // Difunde el vector a *
    MPI_Bcast(datos, tam, MPI_INT, 0, MPI_COMM_WORLD);

    // Determina nmeros a sumar
    bloque = tam / nProc;
    if (idProc == nProc - 1)
    {
        primero = (nProc - 1) * bloque;
        ultimo = tam - 1;
    }
    else
    {
        primero = idProc * bloque;
        ultimo = (idProc + 1) * bloque - 1;
    }
    
    // Computa conteo parcial
    for (i = primero; i <= ultimo; i++)
        if(datos[i] == max)
            contadorMax++;
        if(datos[i] == min)
            contadorMin++;

    // Hacer la reduccin de las sumas parciales para obtener la suma total

    printf("Conteo max parcial: %d, proceso: %d \n", contadorMax, idProc);
    printf("Conteo min parcial: %d, proceso: %d \n\n", contadorMin, idProc);

    MPI_Reduce(&contadorMax, &contadorMaxGlobal, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&contadorMin, &contadorMinGlobal, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (idProc == 0)
    {
        printf("Apariciones máximo global: %d\n", contadorMaxGlobal);
        printf("Apariciones mínimo global: %d\n", contadorMinGlobal);
    }

    MPI_Finalize();
}