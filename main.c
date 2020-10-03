/*
	Fernando Garrote de la Macorra A01027503
	Alejandra Nissan Leizorek A01024682
	
	Actividad 6 
    Servidor
*/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define TCP_PORT 8000

int servidor, cliente;
char buffer[1000];
void imprimirNormal(int);
void ctrlZ(int);
void ctrlC(int);

int main(int argc, const char * argv[])
{  

    struct sockaddr_in direccion;
    
    ssize_t leidos, escritos;
    int continuar = 1;
    pid_t pid;
    
    if (argc != 2) {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }


    // Crear el socket
    servidor = socket(PF_INET, SOCK_STREAM, 0);
    
    // Enlace con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    
    bind(servidor, (struct sockaddr *) &direccion, sizeof(direccion));
    
    // Escuhar
    listen(servidor, 10);
    
    escritos = sizeof(direccion);
    
    // Aceptar conexiones
    while (continuar)
    {
        cliente = accept(servidor, (struct sockaddr *) &direccion, &escritos);
        
        printf("Aceptando conexiones en %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));
        
        signal(SIGINT, ctrlC);
        signal(SIGTSTP, ctrlZ); 

        pid = fork();

        if (pid == 0){
            continuar = 0;
        } 
        
    }
    
    if (pid == 0) {


        close(servidor);



        if (cliente >= 0) {
            while(leidos=read(cliente, &buffer, sizeof(buffer))){
                printf("Semáforo: %s\n", buffer);
            }
            
        }
        
        close(cliente);
    }
    
    else if (pid > 0)
    {
        while (wait(NULL) != -1);
        
        // Cerrar sockets
        close(servidor);
        
    }
    return 0;
}

void ctrlZ(int signal) {
    sprintf(buffer, "%s", "R");
    write(cliente, &buffer, sizeof(buffer));
}


void ctrlC(int signal) {
    sprintf(buffer, "%s", "I");
    write(cliente, &buffer, sizeof(buffer));
}

//El código de socket con varios clientes esta basado en el visto en clase creado por Vicente Cueblls
