#include<ctype.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<signal.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE sizeof(struct sockaddr_in)
#define MAX 10
typedef struct clients
{
    char name[20];
    int client_fd;
    int occupied_fd;
}clients;
clients all[MAX];
int ActiveClients=0;
char ocp[9]={'O','c','c','u','p','i','e','d','\0'};
char not_found[15]={'U','s','e','r',' ','n','o','t',' ','f','o','u','n','d','\0'};
void findMax(int *maxfd)
{
    int i;
    *maxfd=all[0].client_fd;
    for(i=1;i<MAX;i++)
        if(all[i].client_fd > *maxfd)
            *maxfd=all[i].client_fd;
}
int main()
{
    FILE *f;
    int e=0,flag=0;
    for(e=0;e<MAX;e++)
        all[e].occupied_fd=-1; // -1 for not occupied
        int sockfd, maxfd,nread,found,i,j;
    char buf[128];
    char chat_with[128];
    char log_record[128];
    fd_set fds;
    struct sockaddr_in server = {AF_INET,2000,INADDR_ANY};
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        printf("Error creating SOCKET\n");
        return(0);
    }
    if( bind(sockfd,(struct sockaddr *)&server,SIZE)==-1)
    {
        printf("bind failed\n");
        return(0);
    }
    if( listen(sockfd,5)==-1)
    {
        printf("listen failed\n");
        return(0);
    }
    findMax(&maxfd);
    int k=0;
    for(;;)
    {
        memset(buf,0,sizeof(buf));
        printf("Current clients : %d \n",ActiveClients);
        for(k=0;k<MAX;k++)
        {
            printf("client %d : %d  Occupied with: %d  Name: %s\n",k,all[k].client_fd,all[k].occupied_fd,all[k].name);
        }
        findMax(&maxfd);
        maxfd=(maxfd>sockfd?maxfd:sockfd)+1;
        FD_ZERO(&fds);
        FD_SET(sockfd,&fds);
        for(i=0;i<MAX;i++)
            if( all[i].client_fd != 0 )
                FD_SET(all[i].client_fd,&fds);
            /* Wait for some input or connection request. */
            select(maxfd, &fds,(fd_set *)0,(fd_set *) 0,(struct timeval *) 0);
        /*If one of the clients has some input, read and send it to all others.*/
        for(i=0;i<MAX;i++)
            if( FD_ISSET(all[i].client_fd, &fds))
            {
                nread = recv(all[i].client_fd, buf,sizeof(buf), 0);
                printf("\n%s\n",buf);
                // Chat request
                if(buf[0]=='/' && buf[1]=='c' && buf[2]=='h' && buf[3]=='a' && buf[4]=='t')
                {
                    //Divide string
                    char *token;
                    char arr[20];
                    token=strpbrk(buf," ");
                    strcpy(arr,token+1);
                    // Find the guy
                    flag=0;
                    int cmp,found;
                    int len=strlen(arr);
                    arr[len-1]='\0';
                    for(e=0;e<MAX;e++)
                    {
                        cmp=strcmp(all[e].name,arr);
                        if(cmp==0)
                        {
                            found=e;
                            flag=1;
                            break;
                        }
                    }
                    // If user exists
                    if(flag)
                    {
                        // Check if he is occupied
                    printf("Found=%d and name=%s\n",found,all[e].name);
		     //Send you are already in a chat
		    if(all[i].occupied_fd!=-1)
		    {
		      strcpy(chat_with,"You are already in a chat with someone else.. ");
		      send( all[i].client_fd, chat_with, sizeof(chat_with), 0); 
		    }
                    else if(all[found].occupied_fd==-1)
                    {
                        // Server Message Generator
                        strcpy(chat_with,"You are now in a chat with ");
                        strcat(chat_with,all[found].name);                      
                        // Send confirm
                        all[found].occupied_fd=all[i].client_fd;
                        all[i].occupied_fd=all[found].client_fd;
                        send( all[i].client_fd, chat_with, sizeof(chat_with), 0);
                        
                    }
                    else
                        // Send occupied
                        send( all[i].client_fd, ocp, sizeof(ocp), 0);
                    }
                    else
                        // Send does not exist
                        send( all[i].client_fd, not_found, sizeof(not_found), 0);
                    
                }
                // Userlist request
                else if(buf[0]=='/' && buf[1]=='u' && buf[2]=='s' && buf[3]=='e' && buf[4]=='r' && buf[5]=='l' && buf[6]=='i' && buf[7]=='s' && buf[8]=='t')
                {
                    // Sending list of users
                    for(e=0;e<MAX;e++)
                        send( all[i].client_fd, all[e].name, sizeof(all[e].name), 0);  
                }
                else
                {
                    /* If error or eof, terminate the connection */
                    if(nread < 1)
                    {
                        close(all[i].client_fd);
                        memset(all[i].name,0,sizeof(all[i].name));
                        all[i].occupied_fd=-1;
                        for(e=0;e<MAX;e++)
                            if(all[e].occupied_fd==all[i].client_fd)
                                all[e].occupied_fd=-1;
                            all[i].client_fd=0;
                        ActiveClients--;
                    }
                    else
                    {
                        // Log the message
                        f=fopen("log.txt","a");
                        
                        // Direct the message
                        for(j=0;j<MAX;j++)
                            if(all[j].client_fd!=0 && all[j].client_fd==all[i].occupied_fd)
                            {
                                send( all[j].client_fd, buf, nread, 0);
                                strcpy(log_record,all[i].name);
                                strcat(log_record," said *");
                                strcat(log_record,buf);
                                strcat(log_record,"* to ");
                                strcat(log_record,all[j].name);
                                break;
                            }
                        fprintf(f,"%s \n",log_record);
                        fclose(f);
                    }
                }
            }
            
            
            
            // New connection
            
            
            /* If there is a request for a new connection */
            if( FD_ISSET(sockfd, &fds))
            {
                // Check if enough space for new connection
                if(ActiveClients < MAX)
                {
                    found = 0;
                    for(i=0;i<MAX && !found ;i++ )
                        if( all[i].client_fd==0 )
                        {
                            all[i].client_fd=accept(sockfd,NULL,NULL);
                            printf("New Client Joined..\n");
                            recv(all[i].client_fd, all[i].name,sizeof(all[i].name), 0);
                            found=1;
                            ActiveClients++;
                            int e=0;
                            // Sending list of users
                            for(e=0;e<MAX;e++)
                                send( all[i].client_fd, all[e].name, sizeof(all[e].name), 0);  
                        }
                }
            }
    }
    
}
