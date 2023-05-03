#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

	FILE *da;  // Descriptor de archivos tipo FILE fgdg

	int i, cont, suma = 0, mult = 0,
        a[2], b[2], //Descriptores de tuberias
        valor;      // Variables de trabajo
    
    pipe(a); // Crea Tuberia
    pipe(b); // Crea Tuberia

    int hijo = fork();  //Crea proceso hijo

	if(hijo == 0)
	{   // Codigo el proceso hijo
		close(a[1]);    //cierra la escritura para el hijo
		close(b[0]);    //cierra la lectura para el hijo

		da = fdopen(a[0], "r"); //convierte a descriptor a FILE

        int p, q, s, m, cantidad;
        int arregloEntradaH[argc - 1];

		fscanf(da, "%d", &cantidad); 	//recibe los datos a sumar desde el proceso padre por la tuberia
        for(i = 0; i < cantidad; i++)
            fscanf(da, "%d", &arregloEntradaH[i]);
            
        for(i = 0; i < cantidad; i++)
        {
            s += arregloEntradaH[i];
            m *= arregloEntradaH[i];
        }
            
        fclose(da); 

		da = fdopen(b[1], "w");
        fprintf(da, "%d %d", s, m);
		
        close(a[0]);    //Cierra tuberia
	} 
    else
    {   //Codigo del proceso padre
        close(a[0]);    //cierra la lectura
        close(b[1]);    //cierra la escritura

        da = fdopen(a[1], "w"); 

        fprintf(da, "%d ", argc - 1);   // Envia por la tuberia el numero de datos al proceso hijo	
        for(i = 1; i < argc; i++)
            fprintf(da, "%d ", atoi(argv[i]));

        fclose(da);     

        da = fdopen(b[0], "r"); 
        fscanf(da, "%d %d", &suma, &mult);

        printf ("la suma es = %d \n", suma);  
        printf ("la mult es = %d \n", mult);  
    }

    return 0;
}



