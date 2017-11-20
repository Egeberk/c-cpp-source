#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#define n 10
typedef struct PROCESS_ARRAY
{
    int process_num;
    pid_t id;
    char status[10];
}processes;
processes pids[n];
void child_does_this()
{
    while(1)
    {
        sleep(1);
        printf("\nHey! My ID is:%d hit my number again to terminate me.\n",getpid());
        sleep(1);
    }
}
void terminate_all()
{
    int i=0;
    for(i=0;i<n;i++)
        kill(pids[i].id,SIGQUIT);
}
int main()
{
    pid_t pid;
    int i=0;
    for(i=0;i<n;i++)
    {
        pid=fork();
        if(pid==0)
        {
            child_does_this();
            exit(0);
        }
        else
        {
            kill(pid,SIGSTOP);
            pids[i].id=pid;
            pids[i].process_num=i;
            strcpy(pids[i].status,"INACTIVE");
        }
    }
    int choice;
    while(1)
    {
        for(i=0;i<n;i++)
            printf("\n%d. Process ID:%d Status: %s",i+1,pids[i].id,pids[i].status);
        printf("\nEnter the process number to resume or pause the process (Hit 0(zero) to terminate the main program):");
        scanf("%d",&choice);
        if(choice==0)
        {
            terminate_all();
            exit(0);
        }
        if(choice>10 || choice<0)
            printf("INVALID CHOICE!!!\n");
        else
        {
            if(strcmp(pids[choice-1].status,"INACTIVE")==0)
            {
                strcpy(pids[choice-1].status,"ACTIVE");
                kill(pids[choice-1].id,SIGCONT);
            }
            else
            {
                strcpy(pids[choice-1].status,"INACTIVE");
                kill(pids[choice-1].id,SIGSTOP);
            }
        }
    }
    return 0;
}
