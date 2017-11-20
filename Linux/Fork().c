//////////////////////////
// Fork permute program //
////////////////////////// 

///////////////////////////////////////////////////////////////////////////////////////
//     THIS PART IS NECESSARY TO MODIFY THE PROGRAM OUTPUT IN ORDER TO EVALUATE      //
//                   PLEASE DO NOT MODIFY THIS PART OF THE CODE                      //
///////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>                                                                   //
#include <stdlib.h>                                                                  //
#include <unistd.h>                                                                  //
#include <sys/wait.h>                                                                //
#include <string.h>                                                                  //
int dp[2];                                                                           //
void sortOutput() {                                                                  //
    pipe(dp); pid_t p = fork();                                                      //
    if(p == 0) { close(STDOUT_FILENO); close(dp[0]); dup2(dp[1], STDOUT_FILENO); }   //
    else { close(STDIN_FILENO); close(dp[1]); dup2(dp[0], STDIN_FILENO);             //
        char str[256][256]; int lines = 0;                                           //
        while(!feof(stdin) && lines < 256) { fgets(str[lines],256,stdin); lines++; } //
        qsort(str,lines,256,(void*)strcmp);                                          //
        for(;lines>0;lines--) fputs(str[lines],stdout);                              //
        int s; waitpid(p,&s,0); exit(0);                                             //
    }                                                                                //
}                                                                                    //
///////////////////////////////////////////////////////////////////////////////////////
//
// Modify the program below
//

int arr[4];

void swap(int i, int j) {
    int t = arr[j];
    arr[j] = arr[i];
    arr[i] = t;
}

int main() {
    int n, i;
    // User should enter 3 integers here
    for(i = 0; i < 4; i++)
        scanf("%d",&arr[i]);
    printf("\n");
    sortOutput();   // DO NOT REMOVE THIS LINE
    
    pid_t p;
    int s;
    p = fork(); 
    if(p == 0) 
        swap(0,1);
    else 
    {
        waitpid(p,&s,0); 
    }
    // Both child and parent has a child now.
    p = fork(); 
    if(p == 0) 
        swap(0,2);
    else 
    {
        waitpid(p,&s,0);
        p = fork(); 
        if(p == 0) 
            swap(1,2);
        else 
        {
            waitpid(p,&s,0); 
            // *** AFTER THIS PART IS MODIFIED OVER ORIGINAL CODE ***
            p=fork();
            if(p==0)
                swap(2,3);
            else
            {
                waitpid(p,&s,0);
            }
        }
    }
    p=fork();
    if(p==0)
        swap(0,3);
    else
    {
        waitpid(p,&s,0);
        p=fork();
        if(p==0)
            swap(1,3);
        else
        {
            waitpid(p,&s,0);
        }
        
    }
    for(i = 0; i < 4; i++)
        printf("%d ", arr[i]);
    printf("\n");
    
    exit(0);
    return 0;
}
