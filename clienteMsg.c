#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


typedef struct msgbuf
{
  long mtype;
  int pid;
  int num;
} msg;


int main(int argc, char *argv[])
{
    FILE *fp;
    char coladest[10];
    int length = sizeof(msg) - sizeof(long);

    msg sent;

    int qid;
    key_t key;
    key = ftok(".",'m');

    qid = msgget(key, IPC_CREAT | 0660);

    // Valida que reciba el argumento

    sent.mtype = 1;
    sent.pid = getpid();
    sent.num = 1;
    msgsnd(qid, &sent, length, 0);
}