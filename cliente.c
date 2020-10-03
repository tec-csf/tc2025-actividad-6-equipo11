/*
	Fernando Garrote de la Macorra A01027503
	Alejandra Nissan Leizorek A01024682
	
	Actividad 6 
    Cliente
*/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define TCP_PORT 8000

int pidSiguiente;
char buffer[1000];
int cliente;

void controladorSemaforos(int);
void cambioDeLuces(int);

int bloqueo=0;
char color;

int main(int argc, const char * argv[]) {
    
    struct sockaddr_in direccion;
    
    int leidos, escritos;

    char colorAnterior;
    
    
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D color (R (0) o V (1))\n", argv[0]);
        exit(-1);
    }
    
    signal(SIGUSR1, controladorSemaforos);
    signal(SIGALRM, cambioDeLuces);

    //Registrar color inicial
    printf("MI pid es %d\n", getpid());
    printf("Por favor indica el color: \n");
    scanf("%c", &color);

    //Crear el socket
    cliente = socket(PF_INET, SOCK_STREAM, 0);
    
    // Establecer conexión
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    inet_aton(argv[1], &direccion.sin_addr);
    
    int estado = connect(cliente, (struct sockaddr *) &direccion, sizeof(direccion));

    //Pidiendo pid del siguiente semáforo
    printf("Por favor ingresa el pid del siguiente semaforo (contrario a manesillas del reloj)\n");
    scanf("%d", &pidSiguiente);

    //Revisar que si esta en verde inicie el ciclo
    if(color == 'V'){
        sprintf(buffer, "%s %d", "Verde ", getpid());
        write(cliente, &buffer, sizeof(buffer));  
        alarm(10);       
    }

    if (estado == 0) {
        printf("Conectado a %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));
        
        // Leer de teclado y escribir en socket
        while (leidos = read(cliente, &buffer, sizeof(buffer))) {
            printf("Valor de recibido= %s\n", buffer);

            //Control cuando se recibe Alto total o Intermitente
            if(buffer[0]=='R' || buffer[0]=='I'){
                if(bloqueo==0){
                    bloqueo=1;
                    colorAnterior=color;
                     sprintf(buffer, "%c %d", buffer[0], getpid());
                     write(cliente, &buffer, sizeof(buffer));  
                     alarm(0); //Se detiene la alarma
                }
                else{
                    bloqueo=0;
                    color= colorAnterior;
                    if(color=='V'){
                        sprintf(buffer, "%s %d", "Verde ", getpid());
                        write(cliente, &buffer, sizeof(buffer));    
                        alarm(10);//Se vuelve a activar la alarma
                    }
                }
            }
            
            
        }
    }
    
    // Cerrar el socket
    close(cliente);
    
    return 0;
}

//Función principal para el control de semáforos, con esta se inicia
void controladorSemaforos(int signal){
    color = 'V';
    sprintf(buffer, "%s %d", "Verde ", getpid());
    write(cliente, &buffer, sizeof(buffer));
    alarm(10);
}

//Función para cambiar de verde a rojo y mandar al siguiente proceso a ponerse en verde
void cambioDeLuces(int signal){
    if(bloqueo==0){
        color = 'R';
        sprintf(buffer, "%s %d", "Rojo ", getpid());
        write(cliente, &buffer, sizeof(buffer)); 
        kill(pidSiguiente, SIGUSR1);   
    }
}

//El código de socket con varios clientes esta basado en el visto en clase creado por Vicente Cueblls
