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
#include "semaphores.h"

#define MAXSERVERS 3

int main()
{
    int i;
    int pid;
    int status;
    char numserver[10];

    int *sem_load_balance;
    int sem_load_balance_id;

    sem_load_balance_id = createsem(0x1234,3);

	if(sem_load_balance_id==-1)
	{
		perror("Error en la creación del semáforo load_balance");
		exit(1);
	}

    int segment_id = shmget(IPC_PRIVATE, sizeof(sem_load_balance), S_IRUSR | S_IWUSR);
    
    sem_load_balance = shmat(segment_id, NULL, 0);
    
    *sem_load_balance = sem_load_balance_id;

    for(i=0;i<MAXSERVERS;i++)
    {
        pid=fork();
        if(pid==0)
        {
            sprintf(numserver,"%d",i);
            execlp("xterm", "xterm", "-e", "./server",
            numserver,NULL);
        }
    }

    for(i=0;i<MAXSERVERS;i++)
        wait(&status);
}