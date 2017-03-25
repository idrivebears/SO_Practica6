#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h> // for open
#include "semaphores.h"

int numserver;

void procesa(char *buffer);

int main(int argc, char *argv[])
{
    int fp;
    int i;
    char buffer[10];
    char namefifo[10];

    if (argc != 2)
    {
        perror("Indica el número de servidor\n");
        exit(EXIT_FAILURE);
    }
    numserver = atoi(argv[1]);
    printf("Iniciando server %d\n", numserver);

    int sem_load_balance_id;
    int *sem_lb;

    key_t key = ftok("semloadid", 1);
    int segment_id = shmget(key, sizeof(sem_lb), 0666);
    sem_lb = shmat(segment_id, NULL, 0);
    sem_load_balance_id = *sem_lb;
    
    int *available_printer;

    //Conseguimos el ID de la memoria compartida.
    key = ftok("nextprint", 1);
    segment_id = shmget(key, sizeof(int)*3, 0666);
    printf("server:%d\n", segment_id);
    
    
    //Inicializamos la variable available printer en 0, la impresora 0 sera la primera. 
    available_printer = shmat(segment_id, NULL, 0);

    // Inicializa la secuencia de números random
    srand((unsigned int)getpid());

    // Nombra el fifo_n donde n es el número del servidor
    sprintf(namefifo, "fifo_%d", numserver);

    // Elimina y crea un nuevo pipe a través del cual recibe datos del cliente
    unlink(namefifo);
    mkfifo(namefifo, 0644);
    
    for (i = 0; i < 10; i++)
    {
        available_printer[numserver] = 0;
        printf("server vals:%d %d %d\n", available_printer[0],available_printer[1],available_printer[2]);
        memset(buffer, 0, 10);

        // Abre el pipe fifo_n
        fp = open(namefifo, 0);

        // Lee info del pipe
        read(fp, buffer, sizeof(buffer));

        // Función que procesa al cliente
        procesa(buffer);
        
        // Cierra el pipe
        close(fp);
        available_printer[numserver] = 1;
        semsignal(sem_load_balance_id);
    }

    exit(0);
}

void procesa(char *buffer)
{
    printf("Inicia trabajo %s por servidor %d\n", buffer, numserver);
    sleep(rand() % 5);
    printf("Termina trabajo %s por servidor %d\n", buffer, numserver);
}