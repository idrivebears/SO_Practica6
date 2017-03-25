#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct msgbuf
{
  long mtype;
  int pid;
  int num;
} msg;

int numserver;
msg receive;

void procesa();

int main(int argc, char *argv[])
{
    int fp;
    int i;
    
    int length = sizeof(msg) - sizeof(long);

    

    int qid;
    key_t key;
    key = ftok(".",'m');
    qid = msgget(key, IPC_CREAT | 0660);

    if (argc != 2)
    {
        perror("Indica el número de servidor\n");
        exit(EXIT_FAILURE);
    }
    numserver = atoi(argv[1]);
    printf("Iniciando server %d\n", numserver);

    // Inicializa la secuencia de números random
    srand((unsigned int)getpid());
    
    for (i = 0; i < 10; i++)
    {
        
        msgrcv(qid, &receive, length, 1, 0);
        // Función que procesa al cliente
        procesa();

    }
    exit(0);
}

void procesa()
{
    printf("Inicia trabajo %d por servidor %d \n", receive.pid, numserver);
    sleep(rand() % 5);
    printf("Termina trabajo %d por servidor %d\n", receive.pid, numserver);
}