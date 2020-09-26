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
	
	
	printf("Commnads accepted-\n");
	printf("F: Foraward\n");
	printf("L: Left\n");
	printf("R: Right\n");
	
	while (1)
	{
		bzero(buffer,255);
		fgets(buffer,255,stdin);
		
		
			
		n=sendto(sockfd,(char *)buffer,strlen(buffer),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
			
		if (n<0)
			error("Error on sending!\n");
		
		if (strncmp(buffer,"0",1)==0)
		{
			printf("Terminated!\n");
			break;
		}
	
		bzero(buffer,255);
		
		n=recvfrom(sockfd,(char *)buffer,255,0,(struct sockaddr *)&serv_addr,&len);
		
		if (n<0)
			error("Error on revieve!\n");
		
		
		
		printf("Robot::%s\n",buffer);
		
	}
	
	close(sockfd);
	return 0;
		
}
