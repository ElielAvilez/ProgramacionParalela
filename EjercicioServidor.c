#include<stdlib.h>
#include<stdio.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

// recibe un nombre de un cliente y retorna el servidor un saludo
int main(int argc, char *argv[])
{
	int mi_socket = 0;
	int puerto = 0;
	int estado = 0;
	struct sockaddr_in dir;

	if(argc != 2)
	{
		fprintf(stderr, "Use: %s <puerto>\n", argv[0]);
		exit(1);
	}

	mi_socket = socket(AF_INET, SOCK_STREAM, 0);

	if(mi_socket == -1)
	{
		printf("No es posible crear socket!!\n");
		exit(1);
	}
	else
		printf("Socket Creado\n");

	//Recupera el numero de puerto para escuchar
	puerto = atoi(argv[1]);

	//levanta la estructura de direccion
	//utiliza INADD_ANY para relacionar a todas las direcciones locales
	dir.sin_family = AF_INET;
	dir.sin_addr.s_addr = htonl(INADDR_ANY);
	dir.sin_port = htons(puerto);

	//relacionar la direccion y puerto con nuestro socket
	estado = bind(mi_socket, (struct sockaddr *)&dir, sizeof(dir));

	if(estado == 0)
		printf("Enlace completado\n");
	else
	{
		printf("No es posible enlace con la direccion\n");
		close(mi_socket);
		exit(1);
	}

	//escuchar las conexiones en el socket
	estado = listen(mi_socket, 5);

	if(estado == -1)
	{
		printf("No es posible escuchar por el socket\n");
		close(mi_socket);
		exit(1);	
	}


	while(1)
	{
		struct sockaddr_in cliente = { 0 };
		int socket_hijo = 0;
		socklen_t long_cliente = sizeof(cliente);

		//esperar aqui
		socket_hijo = accept(mi_socket, (struct sockaddr *)&cliente, &long_cliente);

		if(socket_hijo == -1)
		{
			printf("No acepta conexiones!\n");
			close(mi_socket);
			exit(1);
		}

		//maneja la nueva solicitud de conexion
		char msg_envio[150] = "La suma es: ",
			 sumS[10], mulS[10];
		int n1 = 0, n2 = 0, sum, mul;
		
		//Recibe mensaje del cliente
		read(socket_hijo, &n1, sizeof(n1));
		printf("Primer número recibido: %d\n",n1);

		read(socket_hijo, &n2, sizeof(n2));
		printf("Segundo número recibido: %d\n",n2 );
		printf("Calculando suma y multiplicación...\n");

		sum = n1 + n2;
		mul = n1 * n2;

		sprintf(sumS, "%d", sum);
		sprintf(mulS, "%d", mul);

		strcat(msg_envio, sumS);
		strcat(msg_envio, " y la multiplicación es: \0");
		strcat(msg_envio, mulS);
					
		//envia el mensaje al cliente
		write(socket_hijo, msg_envio, strlen(msg_envio));
		close(socket_hijo);
	}

	close(mi_socket);

	return 0;
}