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
#define SERV_TCP_PORT 8080
#define SA struct sockaddr
int main(){
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


int n,c=0;
double d;
printf("Enter number of frames to be sent:");
scanf("%d",&n);
int i;
char buff[100],data[100],s[n],str[100];
for(i=0;i<n;i++){
if(i%2==0)
s[i]='0';
else
s[i]='1';
}
write(sockfd,s,sizeof(s));

i=0;
int ms,max=0;
while(i<n){
printf("\nEnter data to be sent:");
scanf("%s",data);
str[0]=s[i];
strcat(data,str);
label:
ms=0;
write(sockfd,data,sizeof(data));
printf("%s\n",data);
printf("Frame %c sent!!\n",s[i]);
clock_t starttime,diff;
starttime=clock();
while(ms<max)
{
  diff = clock()-starttime;
  ms = diff * 1000 / CLOCKS_PER_SEC;
  read(sockfd,buff,sizeof(buff));
  if(strlen(buff)){
  c=1;
  break;
  }
}

if(c==1){
printf("acknowledgement received from server:%s\n",buff);
c=0;
i+=1;
}
else{
printf("acknowledgement not received from server so frame resent\n");
max=10; 
goto label;
}


}
close(sockfd);
return 0;
}

