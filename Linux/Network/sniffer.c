#define _GNU_SOURCE
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen

#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include <ctype.h>

void ProcessPacket(unsigned char* , int,char*,char,int);
void print_tcp_packet(unsigned char * , int,char *,char,int);
void print_udp_packet(unsigned char * , int,char *,char,int );
void print_icmp_packet(unsigned char* ,char* ,char,int );
void PrintData (unsigned char* , int);

struct sockaddr_in source,dest;
int tcp=0,udp=0,icmp=0,total=0,i,j; 

int main()
{
  
  int saddr_size , data_size;
  struct sockaddr saddr;
  
  unsigned char *buffer = (unsigned char *) malloc(65536); //Its Big!
  printf("Starting...\n");
  int port=0;
  char ch ,s[30];
  printf("\nEnter port,ip, string or no filter(p/s/i/n):");
  scanf("%c",&ch);
  if(ch == 'p' || ch == 'P')
  {
    printf("\nEnter port number: ");
    scanf("%d",&port);
  }
  else if(ch == 's' || ch == 'S')
  {
    printf("\nEnter string: ");
    scanf("%s",s);
    getchar();
  }
  else if(ch == 'i' || ch == 'I')
  {
    printf("\nEnter IP address: ");
    scanf("%s",s);
    getchar();
  }
  
  int sock_raw = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;
  
  if(sock_raw < 0)
  {
    //Print the error with proper message
    perror("Socket Error");
    return 1;
  }
  while(1)
  {
    saddr_size = sizeof saddr;
    //Receive a packet
    data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , (socklen_t*)&saddr_size);
    if(data_size <0 )
    {
      printf("Recvfrom error , failed to get packets\n");
      return 1;
    }
    //Now process the packet
    ProcessPacket(buffer, data_size,s,ch,port);
  }
  close(sock_raw);
  printf("Finished");
  return 0;
}

void ProcessPacket(unsigned char* buffer, int size, char *s,char ch,int port)
{
  //Get the IP Header part of this packet , excluding the ethernet header
  struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
  //check
  switch (iph->protocol) 
    {
    case 1:  //ICMP Protocol
            if(port==0)
	    {
	      ++total;
	      ++icmp;
	      print_icmp_packet( buffer ,s ,ch,size);
	      
	    }
	    break;    
	    
    case 6:  //TCP Protocol
            ++tcp;
	    ++total;
	    print_tcp_packet(buffer , size,s,ch ,port);
	    break;
	    
    case 17: //UDP Protocol
            ++udp;
	    ++total;
	    print_udp_packet(buffer , size,s,ch, port);
	    break;
	    
    }
    //printf("\nTCP : %d   UDP : %d   ICMP : %d Total : %d\r", tcp , udp , icmp , total);
}
void print_tcp_packet(unsigned char* Buffer, int Size, char *s,char ch, int port)
{
  unsigned short iphdrlen;
  
  struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
  iphdrlen = iph->ihl*4;
  
  struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
  
  int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;
  
  memset(&source,0,sizeof(source));
  source.sin_addr.s_addr=iph->saddr;
  
  memset(&dest,0,sizeof(dest));
  dest.sin_addr.s_addr=iph->daddr;
  
  /*printf("Source port = %d\n",ntohs(tcph->source));
   *    printf("Destination port = %d\n",ntohs(tcph->dest));*/

  if(ch=='p')
  {
    if( port == ntohs(tcph->source))
    {
      printf("\n\n***********************TCP Packet*************************\n");
      printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      printf("\n");
      printf("TCP Header\n");
      printf("   |-Source Port      : %u\n",ntohs(tcph->source));
      printf("   |-Destination Port : %u\n",ntohs(tcph->dest));
      printf("                        DATA Dump                         ");
      printf( "\n");
      printf("IP Header\n");
      PrintData(Buffer , iphdrlen);
      printf("TCP Header\n");
      PrintData(Buffer+iphdrlen,tcph->doff*4);
      
      printf("Data Payload\n");    
      PrintData(Buffer + header_size , Size - header_size );
    }
    else if(port == ntohs(tcph->dest))
    {
      printf("\n\n***********************TCP Packet*************************\n");
      printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      printf("\n");
      printf("TCP Header\n");
      printf("   |-Source Port      : %u\n",ntohs(tcph->source));
      printf("   |-Destination Port : %u\n",ntohs(tcph->dest));
      printf("                        DATA Dump                         ");
      printf("\n");
      printf("IP Header\n");
      PrintData(Buffer , iphdrlen);
      printf("TCP Header\n");
      PrintData(Buffer+iphdrlen,tcph->doff*4);
      
      printf("Data Payload\n");    
      PrintData(Buffer + header_size , Size - header_size );
    }
  }
  else if(ch=='i')
  {
    int cmp=-1;
    int cmp2=-1;
    cmp=strcmp(s,inet_ntoa(source.sin_addr));
    cmp2=strcmp(s,inet_ntoa(dest.sin_addr));
    if( cmp==0)
    {
      printf("\n\n***********************TCP Packet*************************\n");
      printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      printf("\n");
      printf("TCP Header\n");
      printf("   |-Source Port      : %u\n",ntohs(tcph->source));
      printf("   |-Destination Port : %u\n",ntohs(tcph->dest));
      printf("                        DATA Dump                         ");
      printf( "\n");
      printf("IP Header\n");
      PrintData(Buffer , iphdrlen);
      printf("TCP Header\n");
      PrintData(Buffer+iphdrlen,tcph->doff*4);
      
      printf("Data Payload\n");    
      PrintData(Buffer + header_size , Size - header_size );
    }
    else if(cmp2==0)
    {
      printf("\n\n***********************TCP Packet*************************\n");
      printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      printf("\n");
      printf("TCP Header\n");
      printf("   |-Source Port      : %u\n",ntohs(tcph->source));
      printf("   |-Destination Port : %u\n",ntohs(tcph->dest));
      printf("                        DATA Dump                         ");
      printf("\n");
      printf("IP Header\n");
      PrintData(Buffer , iphdrlen);
      printf("TCP Header\n");
      PrintData(Buffer+iphdrlen,tcph->doff*4);
      
      printf("Data Payload\n");    
      PrintData(Buffer + header_size , Size - header_size );
    }
    
  }
  else if(ch=='s')
  {
    int flag=0;
    int size=Size-header_size;
    char *try=Buffer+header_size;
    for(i=0;i<size;i++)
    {
      for(j=0;j<strlen(s);j++)
	if(try[i]==s[j])
	  i++;
	else
	  break;
	if(j==strlen(s))
	  flag=1;
    }
    if(flag)
    {
      printf("\n\n***********************TCP Packet*************************\n");
      printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      printf("\n");
      printf("TCP Header\n");
      printf("   |-Source Port      : %u\n",ntohs(tcph->source));
      printf("   |-Destination Port : %u\n",ntohs(tcph->dest));
      printf("                        DATA Dump                         ");
      printf( "\n");
      printf("IP Header\n");
      PrintData(Buffer , iphdrlen);
      printf("TCP Header\n");
      PrintData(Buffer+iphdrlen,tcph->doff*4);
      
      printf("Data Payload\n");    
      PrintData(Buffer + header_size , Size - header_size );
    }
  }
  else
  {
    printf("\n\n***********************TCP Packet*************************\n");
    printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
    printf("\n");
    printf("TCP Header\n");
    printf("   |-Source Port      : %u\n",ntohs(tcph->source));
    printf("   |-Destination Port : %u\n",ntohs(tcph->dest));
    printf("                        DATA Dump                         ");
    printf( "\n");
    printf("IP Header\n");
    PrintData(Buffer , iphdrlen);
    printf("TCP Header\n");
    PrintData(Buffer+iphdrlen,tcph->doff*4);
    
    printf("Data Payload\n");    
    PrintData(Buffer + header_size , Size - header_size );
  }
  
}

void print_udp_packet(unsigned char *Buffer , int Size,  char *s,char ch,int port)
{
  unsigned short iphdrlen;
  
  struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
  iphdrlen = iph->ihl*4;
  
  struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));
  
  int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;
  
  memset(&source,0,sizeof(source));
  source.sin_addr.s_addr=iph->saddr;
  
  memset(&dest,0,sizeof(dest));
  dest.sin_addr.s_addr=iph->daddr;
  
  
  
  /*printf("Source port = %d\n",ntohs(udph->source));
   *    printf("Destination port = %d\n",ntohs(udph->dest));*/

  if(ch=='p')
  {
    if( port == ntohs(udph->source))
    {
      printf("\n\n***********************UDP Packet*************************\n");
        printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      printf("\nUDP Header\n");
      printf("   |-Source Port      : %d\n" , ntohs(udph->source));
      printf("   |-Destination Port : %d\n" , ntohs(udph->dest));
      printf("                        DATA Dump                         ");
      printf("\n");
      printf("IP Header\n");
      PrintData(Buffer , iphdrlen);
      printf("UDP Header\n");
      PrintData(Buffer+iphdrlen , sizeof udph);
      
      printf("Data Payload\n");    
      
      //Move the pointer ahead and reduce the size of string
      PrintData(Buffer + header_size , Size - header_size);
    }
    else if( port == ntohs(udph->dest))
    {
      printf("\n\n***********************UDP Packet*************************\n");
        printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      printf("\nUDP Header\n");
      printf("   |-Source Port      : %d\n" , ntohs(udph->source));
      printf("   |-Destination Port : %d\n" , ntohs(udph->dest));
      printf("                        DATA Dump                         ");
      printf("\n");
      printf("IP Header\n");
      PrintData(Buffer , iphdrlen);
      printf("UDP Header\n");
      PrintData(Buffer+iphdrlen , sizeof udph);
      
      printf("Data Payload\n");    
      
      //Move the pointer ahead and reduce the size of string
      PrintData(Buffer + header_size , Size - header_size);
    }
  }
  else if (ch=='s')
  {
    int flag=0;
    int size=Size-header_size;
    char *try=Buffer+header_size;
    for(i=0;i<size;i++)
    {
      for(j=0;j<strlen(s);j++)
	if(try[i]==s[j])
	  i++;
	else
	  break;
	if(j==strlen(s))
	  flag=1;
    }
    //strcasestr((unsigned char*)Buffer+header_size,s)!=NULL
    if(flag)
    {
      printf("\n\n***********************UDP Packet*************************\n");
        printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      printf("\nUDP Header\n");
      printf("   |-Source Port      : %d\n" , ntohs(udph->source));
      printf("   |-Destination Port : %d\n" , ntohs(udph->dest));
      printf("                        DATA Dump                         ");
      printf("\n");
      printf("IP Header\n");
      PrintData(Buffer , iphdrlen);
      printf("UDP Header\n");
      PrintData(Buffer+iphdrlen , sizeof udph);
      
      printf("Data Payload\n");    
      
      //Move the pointer ahead and reduce the size of string
      PrintData(Buffer + header_size , Size - header_size);
    }
  }
  else if(ch=='i')
  {
    int cmp=-1;
    int cmp2=-1;
    cmp=strcmp(s,inet_ntoa(source.sin_addr));
    cmp2=strcmp(s,inet_ntoa(dest.sin_addr));
    if(cmp==0 )
    {
      printf("\n\n***********************UDP Packet*************************\n");
        printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      printf("\nUDP Header\n");
      printf("   |-Source Port      : %d\n" , ntohs(udph->source));
      printf("   |-Destination Port : %d\n" , ntohs(udph->dest));
      printf("                        DATA Dump                         ");
      printf("\n");
      printf("IP Header\n");
      PrintData(Buffer , iphdrlen);
      printf("UDP Header\n");
      PrintData(Buffer+iphdrlen , sizeof udph);
      
      printf("Data Payload\n");    
      
      //Move the pointer ahead and reduce the size of string
      PrintData(Buffer + header_size , Size - header_size);
    }
    else if( cmp2==0)
    {
      printf("\n\n***********************UDP Packet*************************\n");
        printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      printf("\nUDP Header\n");
      printf("   |-Source Port      : %d\n" , ntohs(udph->source));
      printf("   |-Destination Port : %d\n" , ntohs(udph->dest));
      printf("                        DATA Dump                         ");
      printf("\n");
      printf("IP Header\n");
      PrintData(Buffer , iphdrlen);
      printf("UDP Header\n");
      PrintData(Buffer+iphdrlen , sizeof udph);
      
      printf("Data Payload\n");    
      
      //Move the pointer ahead and reduce the size of string
      PrintData(Buffer + header_size , Size - header_size);
    }
  }
  else
  {
    printf("\n\n***********************UDP Packet*************************\n");
      printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
  printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
    printf("\nUDP Header\n");
    printf("   |-Source Port      : %d\n" , ntohs(udph->source));
    printf("   |-Destination Port : %d\n" , ntohs(udph->dest));
    printf("                        DATA Dump                         ");
    printf("\n");
    printf("IP Header\n");
    PrintData(Buffer , iphdrlen);
    printf("UDP Header\n");
    PrintData(Buffer+iphdrlen , sizeof udph);
    
    printf("Data Payload\n");    
    
    //Move the pointer ahead and reduce the size of string
    PrintData(Buffer + header_size , Size - header_size);
  }
  
  
}

void print_icmp_packet(unsigned char* Buffer ,char *s ,char ch,int Size)
{
  unsigned short iphdrlen;
  
  struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr));
  iphdrlen = iph->ihl * 4;
  
  struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen  + sizeof(struct ethhdr));
  
  int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof icmph;
  
  memset(&source,0,sizeof(source));
  source.sin_addr.s_addr=iph->saddr;
  
  memset(&dest,0,sizeof(dest));
  dest.sin_addr.s_addr=iph->daddr;
  
  
  if(ch=='s')
  {
    if(strstr(Buffer+ header_size,s)==NULL)
    {
      printf("\n\n***********************ICMP Packet*************************\n"); 
      
      printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
      printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
    }
    else if (strstr(Buffer+ header_size,s)!=NULL)
    {
      printf("\n\n***********************ICMP Packet*************************\n"); 
      
      printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
      printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
      
      
      printf("\n");
      
      printf("ICMP Header\n");
      printf("   |-Type : %d",(unsigned int)(icmph->type));
      
      if((unsigned int)(icmph->type) == 11)
      {
	printf("  (TTL Expired)\n");
      }
      else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY)
      {
	printf("  (ICMP Echo Reply)\n");
      }
      
      printf("   |-Code : %d\n",(unsigned int)(icmph->code));
	printf("   |-Checksum : %d\n",ntohs(icmph->checksum));
	//printf("   |-ID       : %d\n",ntohs(icmph->id));
	//printf( "   |-Sequence : %d\n",ntohs(icmph->sequence));
	printf("\n");
	
	printf("IP Header\n");
	PrintData(Buffer,iphdrlen);
	
	printf("UDP Header\n");
	PrintData(Buffer + iphdrlen , sizeof icmph);
	
	printf("Data Payload\n");    
	
	//Move the pointer ahead and reduce the size of string
	PrintData(Buffer + header_size , (Size - header_size) );
    }
  }
  else if(ch=='i')
  {
    if(strcmp(inet_ntoa(source.sin_addr),s)==0)
    {
      printf("\n\n***********************ICMP Packet*************************\n"); 
      
      printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
	printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
	printf("\n");
	
	printf("ICMP Header\n");
	printf("   |-Type : %d",(unsigned int)(icmph->type));
	
	if((unsigned int)(icmph->type) == 11)
	{
	  printf("  (TTL Expired)\n");
	}
	else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY)
	{
	  printf("  (ICMP Echo Reply)\n");
	}
	
	printf("   |-Code : %d\n",(unsigned int)(icmph->code));
	printf("   |-Checksum : %d\n",ntohs(icmph->checksum));
	//printf("   |-ID       : %d\n",ntohs(icmph->id));
	//printf( "   |-Sequence : %d\n",ntohs(icmph->sequence));
	printf("\n");
	
	printf("IP Header\n");
	PrintData(Buffer,iphdrlen);
	
	printf("UDP Header\n");
	PrintData(Buffer + iphdrlen , sizeof icmph);
	
	printf("Data Payload\n");    
	
	//Move the pointer ahead and reduce the size of string
	PrintData(Buffer + header_size , (Size - header_size) );
    }
    else if (strcmp(inet_ntoa(dest.sin_addr),s)==0)
    {
      printf("\n\n***********************ICMP Packet*************************\n"); 
      
      printf("Source ip= %s\n",inet_ntoa(source.sin_addr));
	printf("Destination ip= %s\n",inet_ntoa(dest.sin_addr));
	
	
	printf("\n");
	
	printf("ICMP Header\n");
	printf("   |-Type : %d",(unsigned int)(icmph->type));
	
	if((unsigned int)(icmph->type) == 11)
	{
	  printf("  (TTL Expired)\n");
	}
	else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY)
	{
	  printf("  (ICMP Echo Reply)\n");
	}
	
	printf("   |-Code : %d\n",(unsigned int)(icmph->code));
	printf("   |-Checksum : %d\n",ntohs(icmph->checksum));
	//printf("   |-ID       : %d\n",ntohs(icmph->id));
	//printf( "   |-Sequence : %d\n",ntohs(icmph->sequence));
	printf("\n");
	
	printf("IP Header\n");
	PrintData(Buffer,iphdrlen);
	
	printf("UDP Header\n");
	PrintData(Buffer + iphdrlen , sizeof icmph);
	
	printf("Data Payload\n");    
	
	//Move the pointer ahead and reduce the size of string
	PrintData(Buffer + header_size , (Size - header_size) );
    }
  }
  
  
}

void PrintData (unsigned char* data , int Size)
{
  int i , j;
  for(i=0 ; i < Size ; i++)
  {
    if( i!=0 && i%16==0)   //if one line of hex printing is complete...
        {
	  printf("         ");
	  for(j=i-16 ; j<i ; j++)
	  {
	    if(isalpha(data[j]))
	    {   
	      printf("%c",(unsigned char)data[j]);//if its a number or alphabet
	    }
	    else 
	    {
	      printf(".");//otherwise print a dot
	    }
	  }
	  printf("\n");
	} 
	
	if(i%16==0) printf("   ");
	       printf(" %02X",(unsigned int)data[i]);
	
	if( i==Size-1)  //print the last spaces
        {
	  for(j=0;j<15-i%16;j++) 
	  {
	    printf("   "); //extra spaces
	  }
	  
	  printf("         ");
	  
	  for(j=i-i%16 ; j<=i ; j++)
	  {
	    if(isalpha(data[j])) 
	    {
	      printf("%c",(unsigned char)data[j]);
	    }
	    else
	    {
	      printf(".");
	    }
	  }
	  
	  printf("\n" );
	}
  }
}
