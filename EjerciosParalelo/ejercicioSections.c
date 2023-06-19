#include <stdio.h>
#include <omp.h>

int buscaMayores(int *contenido, int N, float media)
{
    int i = 0, cantidadMayores = 0;
    
    for(i = 0; i < N; i++)
    {
        if((float)contenido[i] > media)
            cantidadMayores++;
    }

    return cantidadMayores;
}

int buscaMenores(int *contenido, int N, float media)
{
    int i = 0, cantidadMenores = 0;
    
    for(i = 0; i < N; i++)
    {
        if((float)contenido[i] < media)
            cantidadMenores++;
    }

    return cantidadMenores;
}



int main()
{
    FILE *lector = NULL;
    int i = 0, IdHilo, contenido[500], suma, N = 0, mayores, menores;
    float media;

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

    // Funciones para buscar números mayores y menores que la media en distintas sections
    #pragma omp parallel sections 
    {
        #pragma omp section
        {
            printf("Buscando el mayor en el hilo %d\n",omp_get_thread_num()); 
            mayores  = buscaMayores(contenido, N, media);
                
        }
        #pragma omp section
        {
            printf("Buscando el menor en el hilo %d\n",omp_get_thread_num()); 
            menores  = buscaMenores(contenido, N, media);
        }
    }

    printf("La cantidad de valores mayores es %d\n",mayores);
    printf("La cantidad de valroes menores es %d\n",menores);

    return 1;
}