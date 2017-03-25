#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "semaphores.h"

#define MAXSERVERS 3

int main()
{
    int i;
    int pid;
    int status;
    char numserver[10];

    int *available_printer;
    int *sem_lb;
    
    int sem_load_balance_id;
    
    
    sem_load_balance_id = createsem(0x1234,3);
    
	if(sem_load_balance_id==-1)
	{
		perror("Error en la creación del semáforo load_balance");
		exit(1);
	}
    
    //Creamos clave para la memoria compartida que accesaran cada servidor y los clientes.
    key_t key = ftok("nextprint", 1);
    int segment_id = shmget(key, sizeof(int)*3, 0666 | IPC_CREAT);
    printf("servers:%d\n", segment_id);

    
    
    
    //Inicializamos la variable available printer en 0, la impresora 0 sera la primera. 
    available_printer = shmat(segment_id, NULL, 0);
    available_printer[0] = 1;
    available_printer[1] = 1;
    available_printer[2] = 1;
    printf("servers vals:%d %d %d\n", available_printer[0],available_printer[1],available_printer[2]);
    

    key = ftok("semloadid", 1);
    segment_id = shmget(key, sizeof(sem_lb), S_IRUSR | S_IWUSR | IPC_CREAT);
    sem_lb = shmat(segment_id, NULL, 0);
    *sem_lb = sem_load_balance_id;

    for(i=0;i<MAXSERVERS;i++)
    {
        pid=fork();
        if(pid==0)
        {
            sprintf(numserver,"%d",i);
            execlp("xterm", "xterm", "-e", "./server", numserver,NULL);
        }
    }

    for(i=0;i<MAXSERVERS;i++)
        wait(&status);

    erasesem(sem_load_balance_id);
}