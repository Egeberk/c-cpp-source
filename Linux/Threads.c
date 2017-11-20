#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define max 25
typedef struct numbers
{
  int a;
  int b;
  int c;
  int d;
  int e;
  int id;
  int cou;
}nums;
void *func(void* param)
{
  nums temp=*((nums*)param);
  int i=0;
  int r;
  char s[25]="nums";
  sprintf(s,"nums%d.txt",temp.id);
  FILE *f=fopen(s,"w");
  for(i=0;i<temp.e;i++)
  {
    r=rand()%temp.b+temp.a;
    if(r%temp.c==0 && r%temp.d!=0){
      fprintf(f,"%d ",r);
      temp.cou++;
    }
  }
  *((nums*)param) = temp;
  pthread_exit(NULL);
}
int main(int argc,char **argv)
{
  int i=0;
  nums n[max];
  for(i=0;i<max;i++){
  n[i].a=atoi(argv[1]);
  n[i].b=atoi(argv[2]);
  n[i].c=atoi(argv[3]);
  n[i].d=atoi(argv[4]);
  n[i].e=atoi(argv[5]);
  n[i].id=i;
  n[i].cou=0;
  }
  pthread_t pid[max];

  for(i=0;i<max;i++)
  {
    pthread_create(&pid[i],NULL,func,&n[i]);
  }
   for(i=0;i<max;i++)
  {
    pthread_join(pid[i],NULL);
  }
  for(i=0;i<max;i++){
   printf("Thread %d - %d\n",n[i].id,n[i].cou);
  }
  return 0;
}