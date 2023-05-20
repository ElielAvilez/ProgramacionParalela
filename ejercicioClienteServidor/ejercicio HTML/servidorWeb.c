#include<stdlib.h>
#include<stdio.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include <arpa/inet.h>
// recibe petición de pagina web desde un navegador web

// encabezado http
char encabezado[] = "HTTP/1.1 200 OK\n" 
"Server: miservidor.com\n"
"Content-Type: text/html; charset=utf-8\n\n";

char encabezadoError[] = "HTTP/1.1 404 OK\n" 
"Server: miservidor.com\n"
"Content-Type: text/html; charset=utf-8\n\n";


int main(int argc, char *argv[]){

int mi_socket = 0;
int puerto = 0;
int estado = 0;
struct sockaddr_in dir;

	int i=0;
	char pagina[50];
	char respuesta[1024];
	FILE *archivo;
	char caracter;
	int cont;

if(argc != 2){

printf("Use: %s <puerto>\n", argv[0]);
exit(1);

}//fin del if

mi_socket = socket(AF_INET, SOCK_STREAM, 0);

if(mi_socket == -1){
	printf("No es posible crear socket!!\n");
	exit(1);
}
else{
	printf("Socket Creado\n");
}//fin del if


//Recupera el numero de puerto para escuchar
puerto = atoi(argv[1]);

//levanta la estructura de direccion
//utiliza INADD_ANY para relacionar a todas las direcciones locales
dir.sin_family = AF_INET;
dir.sin_addr.s_addr = htonl(INADDR_ANY);
dir.sin_port = htons(puerto);


//relacionar la direccion y puerto con nuestro socket
estado = bind(mi_socket, (struct sockaddr *)&dir, sizeof(dir));


if(estado == 0){
	printf("Enlace completado\n");
}else{
	printf("No es posible enlace con la direccion\n");
	close(mi_socket);
	exit(1);
}//fin del if


//escuchar las conexiones en el socket
estado = listen(mi_socket, 5);

if(estado == -1){
	printf("No es posible escuchar por el socket\n");
	close(mi_socket);
	exit(1);	
}//fin del if


while(1){
	struct sockaddr_in cliente = { 0 };
	int socket_hijo = 0;
	socklen_t long_cliente = sizeof(cliente);
	//esperar aqui
	socket_hijo = accept(mi_socket, (struct sockaddr *)&cliente, &long_cliente);

	if(socket_hijo == -1){
		printf("No acepta conexiones!\n");
		close(mi_socket);
		exit(1);
	}
	
	//manejar la nueva solicitud de conexion
	char peticion[100];
	//Recibe petición del cliente
    int len = read(socket_hijo, peticion, sizeof(peticion));
	peticion[len]='\0';
	printf("%s\n",peticion );

	i=0;	
 	
	// Se busca despues del GET la pagina solicitada
    while (peticion[5 + i] != ' '){
        pagina[i] = peticion[5 + i];
        i++;
	}

	if(strcmp(pagina, "info.html") == 0)
	{
		system("python3 info.py");
	}
 	
	
	if(i>0 ) 
      pagina[i] = '\0'; // coloca terminador de cadena
    else // Si no se tiene pagina solicitada,  se enviara "index.html"
	  strcpy(pagina, "index.html");

 
	if (access(pagina, F_OK) == 0){ //Si la pagina existe envia el encabezado
		send (socket_hijo, encabezado, strlen(encabezado), 0);
	}
	else { // si la pagina no existe envia el encabezado y establece la pagina de error
		send (socket_hijo, encabezadoError, strlen(encabezadoError), 0);
		strcpy(pagina, "error404.html");
	}

	
	
    printf("Se entrega la pagina %s\n", pagina);
	
	
	// envia el archivo html
  	if ((archivo = fopen ( pagina, "r"))) {   
  		cont = 0;
		//--Mientras no sea el final del archivo leera un caracter tras otro
    	while ((caracter = getc(archivo)) != EOF){
			respuesta[cont++] = caracter ;
			//---Envia datos al cliente si el Buffer de envio se ha llenado
			if (cont >= 1024){ 
				write (socket_hijo, respuesta, cont);
				cont = 0;
			}
    	}
		//---Envia datos al cliente si se llego al final del archivo
    	write (socket_hijo, respuesta, cont);
	}
	fclose(archivo);
}//fin del while


close(mi_socket);

return 0;

}//fin del main