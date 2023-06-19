#include <stdio.h>
#include <omp.h>

int sumarDiagonal(int (*contenido)[5], int filas, int columnas)
{
    int i = 0, j = 0, sumaDiagonal = 0;
    
    for(i = 0; i < filas; i++)
        for(j = 0; j < columnas; j++)
            if(i == j)
                sumaDiagonal += contenido[i][j];
        

    return sumaDiagonal;
}

int sumarTriangularInferior(int (*contenido)[5], int filas, int columnas)
{
    int i = 0, j = 0, sumaTriangularInferior = 0;
    
    for(i = 0; i < filas; i++)
        for(j = 0; j < columnas; j++)
            if(i > 0 && i > j)
                sumaTriangularInferior += contenido[i][j];
        

    return sumaTriangularInferior;
}

int sumarTriangularSuperior(int (*contenido)[5], int filas, int columnas)
{
    int i = 0, j = 0, sumaTriangularSuperior = 0;
    
    for(i = 0; i < filas; i++)
        for(j = 0; j < columnas; j++)
            if(j > 0 && j > i)
                sumaTriangularSuperior += contenido[i][j];
        

    return sumaTriangularSuperior;
}



int main()
{
    FILE *lector = NULL;
    int i = 0, j = 0, x = 0, y = 0, 
        IdHilo, 
        filas = 5, columnas = 5, suma, N = 0, 
        sumaDiagonal = 0, sumaTriangularInferior = 0, sumaTriangularSuperior = 0;
    float media;

    int contenido[filas][columnas];

    lector = fopen("num1Matriz.txt", "r");

    while(feof(lector) == 0)
    {
        fscanf(lector, "%d", &contenido[x][y]);

        y++;

        if(y == 5)
        {
            x++;
            y = 0;
        }  
    }

    for(i = 0; i < filas; i++)
    { 
        for(j = 0; j < columnas; j++)
            printf("%d ", contenido[i][j]);
        
        printf("\n");
    }

    printf("\n");

    // Funciones para sumar diagonal y triangulares en distintas sections
    #pragma omp parallel sections 
    {
        #pragma omp section
        {
            printf("Sumando la diagonal en el hilo %d\n",omp_get_thread_num()); 
            sumaDiagonal  = sumarDiagonal(contenido, filas, columnas);       
        }
        #pragma omp section
        {
            printf("Sumando la triangular inferior en el hilo %d\n",omp_get_thread_num()); 
            sumaTriangularInferior  = sumarTriangularInferior(contenido, filas, columnas);
        }
        #pragma omp section
        {
            printf("Buscando el menor en el hilo %d\n",omp_get_thread_num()); 
            sumaTriangularSuperior  = sumarTriangularSuperior(contenido, filas, columnas);
        }
    }
    
    printf("\n");

    printf("La suma de la diagonal es %d\n",sumaDiagonal);
    printf("La suma de la triangular inferior es %d\n",sumaTriangularInferior);
    printf("La suma de la triangular superior es %d\n",sumaTriangularSuperior);

    return 1;
}