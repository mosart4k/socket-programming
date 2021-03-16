#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h> 

void error(const char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc, char *argv[]) 
{
	if(argc <3) 
	{
		printf("Error!.Port not provided.\n");
		exit(1);
	}
	int sockfd,newsockfd,portno,n;
	char buffer[255];
	
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	portno= atoi(argv[2]);
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd <0 )
	{
		error("Error opening socket\n");
	}
	
	server = gethostbyname(argv[1]);
	if (server == NULL )
	{
		printf("Error!No such host exists!\n");
	}
	
	bzero((char *)&serv_addr,sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *) &serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port =  htons(portno);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
		error("Connection Failed!\n");
		
	
	int len;
	len=sizeof(serv_addr);
	
	while (1)
	{
		bzero(buffer,255);
		fgets(buffer,255,stdin);
		
		if (strncmp(buffer,"bye",1)==0)
			close(sockfd);
			
		n=sendto(sockfd,(char *)buffer,strlen(buffer),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
		
		printf("You :%s\n",buffer);
			
		if (n<0)
			error("Error on sending!\n");
			
		bzero(buffer,255);
		
		n=recvfrom(sockfd,(char *)buffer,255,0,(struct sockaddr *)&serv_addr,&len);
		
		if (n<0)
			error("Error on revieve!\n");
		
		printf("server :%s\n",buffer);
		char *ip=inet_ntoa(serv_addr.sin_addr);
		printf("IP: %s\n",ip);
		printf("Port No: %d\n",portno);
		
	}
	
	close(sockfd);
	return 0;
		
}
