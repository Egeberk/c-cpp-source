//////////////////////////
// Fork permute program //
//////////////////////////
/*
 * Answer 1) 
 * When main process(parent) arrives first fork() , creates a child process. Lets call it c1. 
 * Then it goes into if statement and calls swap(0,1) while its parent enters else and waits the child.
 * After that both c1 and its parent arrives another fork(), both creating a child process.
 * Now we have 4 processes. Lets call them c1's child c2, and main process' child c3.
 * Both c2 and c3 will go into the if statement as they are now 'new childs', while parent (still main process) and c1 enters the else statement.
 * Child of c1 (c2) will call swap(0,2) modifying the array changed by its parent. And c3 does the same as it calles swap(0,2). Their job is done for swaps.
 * Now the parents (main and c1) will go into the else statement waiting for others. Then they both arrive 3rd fork().
 * This will create a child both for c1 and main. Lets call main's new child c4 and c1's new child c5.
 * After this event new childs enter the if statement and both call swap(1,2), modifying their parents arrays.
 * Parents go into the else and waits for all.
 * As a result we have 6 processes, (main-c1-c2-c3-c4-c5) both modified and array for their own thus we have 6 different arrays.
 * Each of the processes have one more job which is printing their own arrays.
 */
// Answer 2) 

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
