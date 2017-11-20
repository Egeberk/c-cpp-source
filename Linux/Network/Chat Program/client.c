#include<ctype.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE sizeof(struct sockaddr_in)
#define MAX 10
typedef struct names
{
    char name[20];
}names;
int main()
{
    char rec[20];
    char servermessage[128];
    names n[MAX];
    int sockfd,nread;
    char buf[128],name[20],enter,resp;
    fd_set fds;
    char IP[20];
    struct sockaddr_in server = {AF_INET,2000};
    printf("\n\n\n\n\nEnter IP address of the Server \n");
    scanf("%s%c",IP,&enter);
    server.sin_addr.s_addr = inet_addr(IP);
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        printf("Error creating SOCKET\n");
        return(0);
    }
    if( connect(sockfd,(struct sockaddr *)&server,SIZE)==-1)
    {
        printf("Connect failed\n");
        return(0);
    }
    // Send your name
    printf("Your chat name: \n");
    scanf("%s",name);
    send(sockfd,name,strlen(name),0);
    int e=0;
    // Get chat list
    memset(rec,0,sizeof(rec));
    for(e=0;e<MAX;e++)
    {
        recv(sockfd, rec, sizeof(rec), 0);
        strcpy(n[e].name,rec);
    }
    memset(rec,0,sizeof(rec));
    printf("People inside the chat room;\n");
    for(e=0;e<MAX;e++)
    {
        if(n[e].name[0]!='\0')
        {
            if(strcmp(n[e].name,name))
                printf("Client %d: %s\n",e+1,n[e].name);
            else
                printf("Client %d: %s (You)\n",e+1,n[e].name);
        }
    }
    
    printf("Commands are ;\n \t /chat *name*\n \t /userlist\n\n");
    printf("Enter a message (E to exit)\n");
    do
    {
        FD_ZERO(&fds);
        FD_SET(sockfd,&fds);
        FD_SET(0,&fds);
        /* Wait for some input. */
        select(sockfd+1,&fds,(fd_set *)0,(fd_set *)0,(struct timeval *)0);
        /* If either device has some input,read it and copy it to the other. */
        if( FD_ISSET(sockfd, &fds))
        {
            nread = recv(sockfd, buf, sizeof(buf), 0);
            /* If error or eof, terminate. */
            if(nread < 1)
            {
                close(sockfd);
                exit(0);
            }
            buf[nread]=0;
            printf("%s", buf);
        }
        if( FD_ISSET(0, &fds))
        {
            nread = read(0, buf, sizeof(buf));
            /* If error or eof, terminate. */
            if(nread < 1)
            {
                occupied:
                close(sockfd);
                exit(0);
            }
            else if((buf[0] =='e' || buf[0]=='E') && nread==2)
            {
                close(sockfd);
                exit(0);
            }
            else
            {
                send(sockfd,buf,nread,0);
                // Server message
                if(buf[0]=='/' && buf[1]=='c' && buf[2]=='h' && buf[3]=='a' && buf[4]=='t')
                {
                    nread = recv(sockfd, servermessage, sizeof(servermessage), 0);
                    printf("\nServer message: %s\n",servermessage);
                    memset(servermessage,0,sizeof(servermessage));
                }
                else if(buf[0]=='/' && buf[1]=='u' && buf[2]=='s' && buf[3]=='e' && buf[4]=='r' && buf[5]=='l' && buf[6]=='i' && buf[7]=='s' && buf[8]=='t')
                {
                    // Get chat list
                    memset(rec,0,sizeof(rec));
                    for(e=0;e<MAX;e++)
                    {
                        n[e].name[0]='\0';
                        recv(sockfd, rec, sizeof(rec), 0);
                        strcpy(n[e].name,rec);
                    }
                    memset(rec,0,sizeof(rec));
                    printf("People inside the chat room;\n");
                    for(e=0;e<MAX;e++)
                    {
                        if(n[e].name[0]!='\0')
                        {
                            if(strcmp(n[e].name,name))
                                printf("Client %d: %s\n",e+1,n[e].name);
                            else
                                printf("Client %d: %s (You)\n",e+1,n[e].name);
                        }
                    }
                }
            }
        }
    } while(1);
}
