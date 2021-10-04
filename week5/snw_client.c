// Used in Connection-oriented communication.
// It offers error and flow control
// It is used in Data Link and Transport Layers
// Stop and Wait ARQ mainly implements Sliding Window Protocol concept with Window Size 1

#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<stdlib.h>
#include<time.h>
#define MAX 80
#define SERV_TCP_PORT 8080
#define SA struct sockaddr
int main()
{
int sockfd,connfd;
struct sockaddr_in serv_addr;
sockfd=socket(AF_INET,SOCK_STREAM,0);
if (sockfd == -1) 
{
printf("socket creation failed...\n");
exit(0);
}
else
printf("Socket successfully created..\n");
bzero(&serv_addr, sizeof(serv_addr));
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
serv_addr.sin_port=htons(SERV_TCP_PORT);
if (connect(sockfd, (SA*)&serv_addr, sizeof(serv_addr)) != 0)
{
printf("connection with the server failed...\n");
exit(0);
}
else
printf("connected to the server..\n");
char buf[1000]={'\0'};
char k[1000]={'\0'};
int ack=0,flag=0;
time_t start,end;
while(1)
{
	if(ack==0)
	{	
		*buf='\0';
		printf("\nEnter data to be sent...\n");
		scanf("%s",buf);
		int l=strlen(buf);
		buf[l]='0';
		write(sockfd,buf,sizeof(buf));
		printf("\nFrame %d is sent successfully...\n",ack);
		start=time(NULL);
		*buf='\0';
	}
	else
	{
		*buf='\0';
		printf("Enter data to be sent...\n");
		scanf("%s",buf);
		int l=strlen(buf);
		buf[l]='1';
		write(sockfd,buf,sizeof(buf));
		printf("\nFrame %d is sent successfully...\n",ack);
		start=time(NULL);
		*buf='\0';
	}
	end=time(NULL);
	while(end-start<2)
	{
		*buf='\0';
		end=time(NULL);
		int rec_size=read(sockfd,k,sizeof(k));
		if(rec_size<0)
		{
			printf("Waiting for the acknowledgement...\n");
			sleep(3);
		}
		else
		{
			flag=1;
			break;
		}
	}
	if(flag==0)
	printf("Timeout...sending again!\n");
	else
	{
		if(k[strlen(k)-1]=='0')
    	{
         	ack=0;
         	printf("Sending the frame: %d \n",ack);
    	}
    	else
   		{
         	ack=1;
         	printf("Sending the frame: %d \n",ack);
   		}
   }
   flag=0;
   *buf='\0';
   *k='\0';
}
close(sockfd);
}

