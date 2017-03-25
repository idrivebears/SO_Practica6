#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include "semaphores.h"

int main()
{
    FILE *fp;
    char coladest[10];
    int *sem_lb;
    int *available_printer;
    int sem_load_balance_id;
    int printer_chosen = 0;

    key_t key = ftok("semloadid", 1);
    int segment_id = shmget(key, sizeof(sem_lb), 0666);
    sem_lb = shmat(segment_id, NULL, 0);
    sem_load_balance_id = *sem_lb;

    key = ftok("nextprint", 1);
    segment_id = shmget(key, sizeof(int)*3, 0666);

    available_printer = (int *) shmat(segment_id, NULL, 0);

    printf("cliente vals:%d %d %d\n", available_printer[0],available_printer[1],available_printer[2]);

    semwait(sem_load_balance_id);

    int i = 0;
    for(i = 0; i < 3; i++) {
        if(available_printer[i] == 1) {
            printer_chosen = i;
            printf("Printer chosen: %d\n", printer_chosen);
            break;
        }
    }

    sprintf(coladest, "fifo_%d", printer_chosen);
    printf("Trabajo a enviarse en %s\n", coladest);
    printf("Trabajo a enviarse en %d\n", printer_chosen);

    // Abre el pipe del servidor 1 en modo de escritura
    fp = fopen(coladest, "w+");
    
    // Escribe en el pipe, en este caso el pid del proceso
    fprintf(fp, "%d", getpid());
    fclose(fp);
}