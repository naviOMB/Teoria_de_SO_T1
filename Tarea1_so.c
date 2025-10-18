// Librerias necesarias para funciones basicas
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>         // ftruncate, fork, etc.
#include <fcntl.h>          // O_CREAT, O_RDWR
#include <sys/mman.h>       // Para mmap() (manejo de memoria compartida)
#include <sys/stat.h>       // Para manejo de permisos
#include <sys/types.h>      // <- para off_t y evitar warning de ftruncate
#include <math.h>
#include <sys/wait.h>       // Para waitpid (esperar procesos hijos)

// Declaracion de prototipo de la función ftruncate, para evitar warnings
int ftruncate(int fd, off_t length);        

// Definimos el nombre del objeto de memoria compartida y el tamaño total de la memoria compartida en bytes
#define SMOBJ_NAME "/myMemoryObj"
#define SMOBJ_SIZE 210

// Funcion que encuentra numeros de Armstrong y los guarda en "buffer"
void encontrarArmstrong(int digitos, char* buffer) {
    // Calculamos el numero inicial y final del rango
    int inicio = (digitos == 1) ? 0 : pow(10, digitos - 1);
    int fin = pow(10, digitos) - 1;
    // Iniciamos un contador para guardar caracteres en el buffer
    int contador = 0;

    // Recorre cada numero en el rango para verificar si es Armstrong
    for (int num = inicio; num <= fin; num++) {
        int temp = num;
        int suma = 0;

        // Extrae cada digito y calcula la suma de las potencias
        while (temp > 0) {
            int digito = temp % 10;
            suma += pow(digito, digitos);
            temp /= 10;
        }

        // Verifica si es un numero de Armstrong
        if (suma == num) {
            // Guarda el numero encontrado en el buffer
            contador += sprintf(buffer + contador, "%d ", num);
        }
    }

    buffer[contador] = '\0';  // Cierre seguro de la cadena
}


// Funcion principal que inicia el programa
int main(int argc, char *argv[]) {
    // Verifica que se ingrese exactamente un argumento (ademas del nombre del programa)
    if (argc != 2) {
       printf("Uso: %s <numero entre 3 y 8>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);    // Convierte el argumento ingresado a entero
    
    // Verifica que el numero ingresado esté en el rango valido (3 a 8)
    if (n < 3 || n > 8) {
        printf("Error: El numero debe estar entre 3 y 8.\n");
        return 1;
    }

    // Variables para manejar los procesos hijos y la memoria compartida
    pid_t pid1,pid2,pid3;               // IDs de procesos hijos 
    int fd;                             // Descriptor para la memoria compartida
    char *ptr;                          // Puntero a la memoria compartida


    // Crea y abre la memoria compartida con permisos de lectura y escritura
    fd = shm_open(SMOBJ_NAME, O_CREAT | O_RDWR, 0666); 
    // Manejo de errores
    if (fd == -1){
            printf("Error al abrir la seccion de memoria compartida\n");
            exit(1);
        }
    if (-1 == ftruncate(fd, SMOBJ_SIZE)){
             printf("Error al asignar tamaño a la memoria compartida\n");
            exit(1);
        }
        
    // Mapea la memoria compartida en el espacio de direcciones del proceso
    ptr = mmap(0, SMOBJ_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (ptr == MAP_FAILED) {
            printf("Error al mapear la memoria compartida");
            exit(1);
        }


        // Creacion del primer proceso hijo
        pid1 = fork();          // Hijo 1
            if (pid1 < 0){      // Error al crear proceso hijo
                printf("Error en la creacion del proceso hijo 1");
                return -1;
            }
            else if(pid1 == 0){     /*Acciones del hijo 1*/
                encontrarArmstrong(n, ptr);  // Hijo 1: calcula y escribe numeros de Armstrong de n digitos en ptr
                exit(0);        // Termina el proceso hijo 1
            }

        // Crea el segundo proceso hijo
        pid2 = fork();          // Hijo 2
            if (pid2 < 0){      // Error al crear proceso hijo
                printf("Error en la creacion del proceso hijo 2");
                return -1;
            }
            else if(pid2 == 0){     /*Acciones del hijo 2*/
                encontrarArmstrong(n-1, ptr + 70);    // El hijo 2 calcula los números de Armstrong de (n-1) digitos y los guarda en el segundo bloque de memoria compartida (ptr + 70) para no sobreescribir los datos del hijo 1
                exit(0);        // Termina el proceso hijo 2
                }
    
        // Crea el tercer proceso hijo
        pid3 = fork();          // Hijo 3
            if (pid3 < 0){      // Error al crear proceso hijo
                printf("Error en la creacion del proceso hijo 3");
                return -1;
            }
            else if(pid3 == 0){     /*Acciones del hijo 3*/
                encontrarArmstrong(n-2, ptr + 140);      // El hijo 3 calcula los números de Armstrong de (n-2) dígitos y los guarda en el tercer bloque de memoria compartida (ptr + 140) para no sobreescribir los datos del hijo 2
                exit(0);        // Termina el proceso hijo 3
                }
        

        // El padre espera a que todos los hijos terminen
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        waitpid(pid3, NULL, 0);


        // Imprime los resultados almacenados por cada proceso hijo
        printf("\n");
        if (strlen(ptr) > 0)    // Imprime resultados del primer hijo (N digitos)
            printf("Números de %d dígitos: %s\n", n, ptr);
        else
            printf("No hay números de Armstrong de %d dígitos.\n", n);

        if (strlen(ptr + 70) > 0)    // Imprime resultados del segundo hijo (N-1 digitos)
            printf("Números de %d dígitos: %s\n", n - 1, ptr + 70);
        else
            printf("No hay números de Armstrong de %d dígitos.\n", n - 1);
    
        if (strlen(ptr + 140) > 0)   // Imprime resultados del tercer hijo (N-2 digitos)
            printf("Números de %d dígitos: %s\n", n - 2, ptr + 140);
        else
            printf("No hay números de Armstrong de %d dígitos.\n", n - 2);

        // Limpia y libera la memoria compartida al finalizar
        munmap(ptr, SMOBJ_SIZE);
        shm_unlink(SMOBJ_NAME);

    // Finaliza el programa correctamente
    return 0;
}
