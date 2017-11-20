#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// DECLEARETIONS
typedef struct THREAD
{
    int num;
    char id;
    int arrival;
    int burst;
    
}T;
#define THREAD_SIZE 10
int job_count=0;
int total_job=0;
sem_t mutex[THREAD_SIZE];
sem_t main_sem;
int locks[THREAD_SIZE]; // 1/ UNLOCKED ----- 0/ LOCKED
int finished[THREAD_SIZE]; // 1/ FINISHED ------ 0/ UNFINISHED
int started[THREAD_SIZE]; // 1/ STARTED -------- 0/ NOT STARTED
int waitings[THREAD_SIZE];
int turnarounds[THREAD_SIZE];
char *output;
int output_size=0;


// INITIALIZATIONS
void init_threads(T arr[], char *FILE_NAME)
{
    FILE *f=fopen(FILE_NAME,"r");
    if(f)
    {
        int i=0;
        char c;
        for(i=0;i<THREAD_SIZE;i++){
            fscanf(f,"%c%d%d\n",&c,&arr[i].arrival,&arr[i].burst);
            arr[i].id=c;
            arr[i].num=i;
            total_job+=arr[i].burst;
        }
        fclose(f);
    }
    else
    {
        printf("\n ERROR: FILE DOES NOT EXIST.\n\n");
        exit(0);
    }
}
void init_mutexes()
{
    int i=0;
    for(i=0;i<THREAD_SIZE;i++)
        sem_init(&mutex[i],0,0);
    sem_init(&main_sem,0,0);
}
// SORT (INSERTION)
void thread_sort(T arr[])
{
    int i,j;
    int temp_burst,temp_arrival,temp_lock;
    char temp_id;
    for(i=1;i<=THREAD_SIZE-1;i++)
    {
        j=i;
        for(;j>0 && arr[j].burst < arr[j-1].burst;j--)
        {
            temp_id=arr[j].id;
            temp_burst=arr[j].burst;
            temp_arrival=arr[j].arrival;
            
            arr[j].id=arr[j-1].id;
            arr[j].burst=arr[j-1].burst;
            arr[j].arrival=arr[j-1].arrival;
            
            arr[j-1].id=temp_id;
            arr[j-1].burst=temp_burst;
            arr[j-1].arrival=temp_arrival;
            
        }
    }
}
// FIND SHORTEST JOB
int find_shortest(T arr[])
{
    int min=999;
    int index;
    int i=0;
    for(i=0;i<THREAD_SIZE;i++)
    {
        if(arr[i].burst<=min && locks[i]==1 && finished[i]==0){
            min=arr[i].burst;
            index=i;
        }
    }
    return index;
}
// UNLOCK ARRIVED ONES
void unlock_arrived(T arr[])
{
    int i=0;
    for(i=0;i<THREAD_SIZE;i++)
        if(job_count>=arr[i].arrival)
            locks[i]=1;
}
// CHECKS IF ALL THREADS HAVE FINISHED
int check()
{
    int i=0;
    int status=0; 
    for(i=0;i<THREAD_SIZE;i++)
    {
        if(finished[i]==0){
            status=1;
        }
    }
    return status;
    
}
int check_arrived()
{
    int i=0;
    int status=0; 
    for(i=0;i<THREAD_SIZE;i++)
    {
        if(locks[i]==1){
            status++;
        }
    }
    return status;
}
// TIMING FUNCTIONS
void update_turnarounds()
{
    int i=0;
    for(i=0;i<THREAD_SIZE;i++)
        if(finished[i]!=1)
            turnarounds[i]++;
}
void update_waiting()
{
    int i=0;
    for(i=0;i<THREAD_SIZE;i++)
        if(started[i]==0)
            waitings[i]++;
}
double calc_avg_wait()
{
    int i=0,sum=0;
    for(i=0;i<THREAD_SIZE;i++)
        sum+=waitings[i];
    return sum/THREAD_SIZE;
}
double calc_avg_turnaround()
{
    int i=0,sum=0;
    for(i=0;i<THREAD_SIZE;i++)
        sum+=turnarounds[i];
    return sum/THREAD_SIZE;
}
// FUNCTIONS
void* func0(void *param)
{
    T data=*((T*)param);
    int i=0;
    while(finished[0]!=1)
    {
        if(locks[0]==1)
        {
            for(i=0;i<data.burst;i++)
            {
                sem_wait(&mutex[0]);
                update_waiting();
                started[0]=1;
                output[output_size++]=data.id;
                job_count++;
                update_turnarounds();
                sem_post(&main_sem);
            }
            finished[0]=1;
        }
    }
    sem_post(&main_sem);
    printf("Thread with id:%c is finished.\n",data.id);
    pthread_exit(NULL);
}    
void* func1(void *param)
{
    T data=*((T*)param);
    int i=0;
    
    while(finished[1]!=1)
    {
        if(locks[1]==1)
        {
            for(i=0;i<data.burst;i++)
            {
                sem_wait(&mutex[1]);
                update_waiting();
                started[1]=1;
                output[output_size++]=data.id;
                job_count++;
                update_turnarounds();
                sem_post(&main_sem);
            }
            finished[1]=1;
        }
    }
    sem_post(&main_sem);
    printf("Thread with id:%c is finished.\n",data.id);
    pthread_exit(NULL);
}    
void* func2(void *param)
{
    T data=*((T*)param);
    int i=0;
    
    while(finished[2]!=1)
    {
        if(locks[2]==1)
        {
            for(i=0;i<data.burst;i++)
            {
                sem_wait(&mutex[2]);
                update_waiting();
                started[2]=1;
                output[output_size++]=data.id;
                job_count++;
                update_turnarounds();
                sem_post(&main_sem);
                
            }
            finished[2]=1;
        }
    }
    sem_post(&main_sem);
    printf("Thread with id:%c is finished.\n",data.id);
    pthread_exit(NULL);
}    
void* func3(void *param)
{
    T data=*((T*)param);
    int i=0;
    
    while(finished[3]!=1)
    {
        if(locks[3]==1)
        {
            for(i=0;i<data.burst;i++)
            {
                sem_wait(&mutex[3]);
                update_waiting();
                started[3]=1;
                output[output_size++]=data.id;
                job_count++;
                update_turnarounds();
                sem_post(&main_sem);
                
            }
            finished[3]=1;
            
        }
    }
    sem_post(&main_sem);
    printf("Thread with id:%c is finished.\n",data.id);
    pthread_exit(NULL);
}    
void* func4(void *param)
{
    T data=*((T*)param);
    int i=0;
    
    while(finished[4]!=1)
    {
        if(locks[4]==1)
        {
            for(i=0;i<data.burst;i++)
            {
                
                sem_wait(&mutex[4]);
                update_waiting();
                started[4]=1;
                output[output_size++]=data.id;
                job_count++;
                update_turnarounds();
                sem_post(&main_sem);
                
            }
            finished[4]=1;
            
        }
    }
    sem_post(&main_sem);
    printf("Thread with id:%c is finished.\n",data.id);
    pthread_exit(NULL);
}    
void* func5(void *param)
{
    T data=*((T*)param);
    int i=0;
    
    while(finished[5]!=1)
    {
        if(locks[5]==1)
        {
            for(i=0;i<data.burst;i++)
            {
                sem_wait(&mutex[5]);
                update_waiting();
                started[5]=1;
                output[output_size++]=data.id;
                job_count++;
                update_turnarounds();
                sem_post(&main_sem);
            }
            finished[5]=1;
            
        }
    }
    sem_post(&main_sem);
    printf("Thread with id:%c is finished.\n",data.id);
    pthread_exit(NULL);
}    
void* func6(void *param)
{
    T data=*((T*)param);
    int i=0;
    
    while(finished[6]!=1)
    {
        if(locks[6]==1)
        {
            for(i=0;i<data.burst;i++)
            {
                sem_wait(&mutex[6]);
                update_waiting();
                started[6]=1;
                output[output_size++]=data.id;
                job_count++;
                update_turnarounds();
                sem_post(&main_sem);
            }
            finished[6]=1;
            
        }
    }
    sem_post(&main_sem);
    printf("Thread with id:%c is finished.\n",data.id);
    pthread_exit(NULL);
}    
void* func7(void *param)
{
    T data=*((T*)param);
    int i=0;
    while(finished[7]!=1)
    {
        if(locks[7]==1)
        {
            for(i=0;i<data.burst;i++)
            {
                sem_wait(&mutex[7]);
                update_waiting();
                started[7]=1;
                output[output_size++]=data.id;
                job_count++;
                update_turnarounds();
                sem_post(&main_sem);
            }
            finished[7]=1;
            
        }
    }
    sem_post(&main_sem);
    printf("Thread with id:%c is finished.\n",data.id);
    pthread_exit(NULL);
}    
void* func8(void *param)
{
    T data=*((T*)param);
    int i=0;
    
    while(finished[8]!=1)
    {
        if(locks[8]==1)
        {
            for(i=0;i<data.burst;i++)
            {
                sem_wait(&mutex[8]);
                update_waiting();
                started[8]=1;
                output[output_size++]=data.id;
                job_count++;
                update_turnarounds();
                sem_post(&main_sem);
                
            }
            
            finished[8]=1;
        }
    }
    sem_post(&main_sem);
    printf("Thread with id:%c is finished.\n",data.id);
    pthread_exit(NULL);
}    
void* func9(void *param)
{
    T data=*((T*)param);
    int i=0;
    
    while(finished[9]!=1)
    {
        if(locks[9]==1)
        {
            for(i=0;i<data.burst;i++)
            {
                sem_wait(&mutex[9]);
                update_waiting();
                started[9]=1;
                output[output_size++]=data.id;
                job_count++;
                update_turnarounds();
                sem_post(&main_sem);
                
            }
            finished[9]=1;
        }
    }
    sem_post(&main_sem);
    printf("Thread with id:%c is finished.\n",data.id);
    pthread_exit(NULL);
}    
// FUNCTIONS


// MAIN
int main(int argc,char **argv)
{
    T threads[THREAD_SIZE];
    init_threads(threads,argv[1]);
    output=(char*)malloc(sizeof(char)*total_job);
    thread_sort(threads);
    pthread_t ids[THREAD_SIZE];
    int i=0;
    int simulation=1;
    printf("Num\tID\tArrival\tBurst\t\n");
    for(i=0;i<THREAD_SIZE;i++)
        printf("%d\t%c\t%d\t%d\t\n",threads[i].num,threads[i].id,threads[i].arrival,threads[i].burst);
    
    // PTHREAD_CREATE()
    pthread_create(&ids[0],NULL,func0,&threads[0]);
    pthread_create(&ids[1],NULL,func1,&threads[1]);
    pthread_create(&ids[2],NULL,func2,&threads[2]);
    pthread_create(&ids[3],NULL,func3,&threads[3]);
    pthread_create(&ids[4],NULL,func4,&threads[4]);
    pthread_create(&ids[5],NULL,func5,&threads[5]);
    pthread_create(&ids[6],NULL,func6,&threads[6]);
    pthread_create(&ids[7],NULL,func7,&threads[7]);
    pthread_create(&ids[8],NULL,func8,&threads[8]);
    pthread_create(&ids[9],NULL,func9,&threads[9]);
    
    // SIMULATION
    while(simulation==1)
    {
        if(check_arrived()!=10){
            unlock_arrived(threads);
        }
        int key=find_shortest(threads);
        sem_post(&mutex[key]);
        if(simulation==0)
            sem_post(&main_sem);
        sem_wait(&main_sem);
        simulation=check();
    }
    // WAITING OTHER THREADS TO FINISH
    pthread_join(ids[0],NULL);
    pthread_join(ids[1],NULL);
    pthread_join(ids[2],NULL);
    pthread_join(ids[3],NULL);
    pthread_join(ids[4],NULL);
    pthread_join(ids[5],NULL);
    pthread_join(ids[6],NULL);
    pthread_join(ids[7],NULL);
    pthread_join(ids[8],NULL);
    pthread_join(ids[9],NULL);
    // RESULTS
    printf("\nDone...\nOutput: ");
    for(i=0;i<output_size;i++)
        printf("|%c|",output[i]);
    printf("\nJob Done: %d\n",job_count);
    
    printf("Thread\t Wait Time\tTurn Around Time\n");
    for(i=0;i<THREAD_SIZE;i++)
        printf("%c\t\t%d\t\t%d\n",threads[i].id,--waitings[i],turnarounds[i]);
    printf("AVG (Wait Time): %.2lf\n",calc_avg_wait());
    printf("AVG (Turn Around Time): %.2lf\n",calc_avg_turnaround());
    return 0;
}
