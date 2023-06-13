#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int datos[argc - 1], max = 0, j = 0;

    #pragma omp parallel num_threads(argc - 1)
    {
        FILE *lector = NULL;
        int i = 0, IdHilo, min = 10000;
        int contenido[500], caracter;

        IdHilo = omp_get_thread_num();

        lector = fopen(argv[IdHilo + 1], "r");

        while(feof(lector) == 0)
        {
            fscanf(lector, "%d", &caracter);
            contenido[i] = caracter;
            i++;
        }

        int cantidad = i;

        for (i = 0; i < cantidad ; i++)
        {
            if(contenido[i] < min)
                min = contenido[i];
        }
        
        datos[IdHilo] = min;

        printf("El minimo del hilo %d es: %d\n", IdHilo, min);
    }

    for (j = 0; j < argc - 1 ; j++)
    {
        if(datos[j] > max)
            max = datos[j];
    }

    printf("El máximo de los mínimos es: %d\n", max);


    return 0;
}