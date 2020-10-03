# tc2025-actividad-6-equipo11
### Sockets y señales
### Fernando Garrote
### Alejandra Nissan

#### Instrucciones para compilar y correr el programa:
1. Se debe compilar cada archivo, recomendamos utilizar gcc
2. Se debe correr una vez el servidor (archivo main) agregando la dirección loopback address:
./main 127.0.0.1
3. Se debe correr cuatro veces el archivo cliente.c (simbolizando a los cuatro semáforos)
./client 127.0.0.1
4. Para cada semáforo preguntará el color con un caracter (V o R), en el siguiente orden: V R R R
5. En cada semáforo se imprimirá su pid
6. Se pedirá ingresar el pid del siguiente proceso (el que esta a la derecha). 
Es importante mencionar que el último en ingresarse debe ser el del semáforo que esta en verde. 
Es decir: si los siguientes semáforos tuvieran estos pid 
V(1) R(2) R(3) R(4)
entonces sus próximos pid se deben ingresar de la siguiente manera:
V(2) R(3) R(4) V(1) Recordando ingresar al final el siguiente pid del semáforo que esta en verde
7. En la consola se puede ver el orden en el que se activan y desactivan los semáforos
8. En caso de mandar (ctrl c o ctrl z)
Aparecerá en pantalla que cambiaron de estado, se deberá volver a ingresar la misma señal para sacarlo de ese estado. 
9. El flujo seguirá con normalidad en donde había sido interrumpido anteriormente. 

