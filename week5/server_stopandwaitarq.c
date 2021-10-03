#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
#include <unistd.h>
#include<stdlib.h>
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

char data[100],s[100],c[10];
int i=0,l;
read(newsockfd,s,sizeof(s));

while(i<strlen(s)){
while(strlen(data)==0){
read(newsockfd,data,sizeof(data));
}
l=strlen(data);
if(data[l-1]==s[i])
{
printf("\nFrame received from the client:%s\n",data);
bzero(data,sizeof(data));
if(i<strlen(s)){
char buff[100]="ACK ";
c[0]=s[i+1];
strcat(buff,c);
write(newsockfd,buff,sizeof(buff));
printf("acknowledgement sent\n");
}
i+=1;
}
else{
printf("\nDuplicate Frame received so discarded!!\n");
}
bzero(data,sizeof(data));
}

close(sockfd);
return 0;
}
