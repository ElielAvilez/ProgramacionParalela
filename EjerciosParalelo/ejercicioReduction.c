#include <stdio.h>
#include <omp.h>
#include <math.h>

int main(int argc, char *argv[])
{
    FILE *lector = NULL;
    int i = 0, IdHilo;
    int contenido[500], caracter, suma, N = 0;
    float media, stdev, var, sumaVarianza;

    lector = fopen("num1.txt", "r");

    while(feof(lector) == 0)
    {
        fscanf(lector, "%d", &contenido[N]);
        //contenido[N] = caracter;
        N++;
    }

    N--;

    // for(i = 0; i < N; i++)
    // { 
    //     printf("contenido %d: %d\n", i, contenido[i]);
    // }

    // Sumatoria
    #pragma omp parallel for private(i) reduction(+:suma)   
    for(i = 0; i < N; i++)
    { 
        suma += contenido[i];
        printf("Suma %d en el hilo %d\n", suma, omp_get_thread_num());
    }

    printf("\n");

    printf("El  valor de suma es %d \n",suma);

    media = (float)suma/(float)N;

    printf("La media es: %f\n", media);

    printf("\n");

    // SUmatoria para varianza
    #pragma omp parallel for private(i) reduction(+:suma)   
    for(i = 0; i < N; i++)
    { 
        sumaVarianza += pow(contenido[i] - media, 2);
        printf("Suma para varianza %f en el hilo %d\n", sumaVarianza, omp_get_thread_num());
    }

    printf("\n");

    var = sumaVarianza / N;

    stdev = sqrt(var);

    printf("La varianza es: %f y la desviación estándar es: %f\n", var, stdev);

    printf("\n");
    
    return 0;
}