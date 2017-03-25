#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCLIENTS 3

int main(int argc, char *argv[])
{
    int i;
    int pid;
    int status;
    char numserver[10];
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