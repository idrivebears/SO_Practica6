#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAXSERVERS 3

typedef struct msgbuf
{
  long mtype;
  int pid;
  int num;
} msg;


int main()
{
    int i;
    int qid;

    int pid;
    int status;
    char numserver[10];
    key_t key;
    key = ftok(".",'m');

    qid = msgget(key, IPC_CREAT | 0660);

    for(i=0;i<MAXSERVERS;i++)
    {

        pid=fork();
        if(pid==0)
        {
            sprintf(numserver,"%d",i);
            execlp("xterm", "xterm", "-e", "./server", numserver ,NULL);
        }
    }

    for(i=0;i<MAXSERVERS;i++)
        wait(&status);
}