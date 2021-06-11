#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
#include <unistd.h>
#include<time.h>
#include<stdlib.h>
#define MAX 80
#define SERV_TCP_PORT 8080
#define SA struct sockaddr
int main()
{
int sockfd,newsockfd,clength;
struct sockaddr_in serv_addr,cli_addr;
sockfd=socket(AF_INET,SOCK_STREAM,0);

if (sockfd == -1) {
printf("socket creation failed...\n");
exit(0);
}
else
printf("Socket successfully created..\n");

serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=INADDR_ANY;
serv_addr.sin_port=htons(SERV_TCP_PORT);

if ((bind(sockfd, (SA*)&serv_addr, sizeof(serv_addr))) != 0) {
printf("socket bind failed...\n");
exit(0);
}
else
printf("Socket successfully binded..\n");

if ((listen(sockfd, 5)) != 0) {
printf("Listen failed...\n");
exit(0);
}
else
printf("Server listening..\n");

clength=sizeof(cli_addr);
newsockfd=accept(sockfd, (SA*)&cli_addr, &clength);

if (newsockfd< 0) {
printf("server acccept failed...\n");
exit(0);
}
else
printf("server acccept the client...\n");

char buf[1000]={'\0'};
char send[1000]={'\0'};
int ack=0;
while(1)
{
	int rec=read(newsockfd,buf,sizeof(buf));
	if(rec>0)
	{
		if(buf[strlen(buf)-1]=='0')
		{
		if(ack==0)
		{
			printf("\nFrame %d recieved successfully...\n",ack);
			ack=1;
			int len=strlen(buf);
			printf("length of the data: %d\n",len-1);
			printf("Frame recieved along with seq_no is: %s\n",buf);
			*buf='\0';
			*send='\0';
			printf("Sending acknowledgement for frame: %d\n",ack);
			strcpy(send,"Ack recieved 1");
			write(newsockfd,send,sizeof(send));
			*send='\0';
		}
		else
		{
			*send='\0';
			strcpy(send,"Duplicate");
			printf("Duplicate frame recieved,discarding...\n");
			write(newsockfd,send,sizeof(send));
			*send='\0';
		}
		}
		else
		{
			if(ack==1)
			{
			printf("\nFrame %d recieved successfully \n",ack);
			ack=0;
			int len=strlen(buf);
			printf("length of the data: %d\n",len-1);
			printf("Frame recieved along with seq_no is: %s\n",buf);
			*buf='\0';
			*send='\0';
			strcpy(send,"ACK RECEIVED 0");
			printf("Sending Acknowledgement for frame: %d\n",ack);
			write(newsockfd,send,sizeof(send));
			*send='\0';
			}
			else
			{
			*send='\0';
			strcpy(send,"Duplicate");
			printf("Duplicate frame recieved,discarding...\n");
			write(newsockfd,send,sizeof(send));
			*send='\0';
			}
		}
		
	}
	*buf='\0';
	*send='\0';
}
close(sockfd);
return 0;
}
