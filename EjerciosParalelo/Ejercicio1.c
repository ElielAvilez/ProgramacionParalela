#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
	int i, j = 0;
    int idHilo;
    int contenido[500], caracter, suma = 0, sumasParciales[atoi(argv[1])];
    float promedio;

    FILE *lector = NULL;

    lector = fopen(argv[2], "r");
  
    while(feof(lector) == 0)
    {
        fscanf(lector, "%d", &caracter);
        contenido[j] = caracter;
        j++;
    }

    i = 0;

    for(i = 0; i < atoi(argv[1]); i++)
        sumasParciales[i] = 0;    
    
    #pragma omp parallel for 
    for(i = 0; i < j; i++) 
    {
        idHilo = omp_get_thread_num();
        sumasParciales[idHilo] += contenido[i];    
    }
    
    for(i = 0; i < atoi(argv[1]); i++) 
    {
        suma += sumasParciales[i];  
        printf("suma parcial del hilo %d: %d \n", i, sumasParciales[i]); 
    }

    printf("suma total: %d\n", suma);
    printf("Promedio: %f \n", (float)suma/(float)j);
    return 0;
}

