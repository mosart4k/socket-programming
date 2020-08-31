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
	int choice;
	int buffer;
	
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	portno= atoi(argv[2]);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
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
	
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
		error("Connection Failed!\n");
	
	while (1)
	{
		printf("\nEnter 1 to input dataword or 0 to exit::");
		
		scanf("%d",&choice);
		
		if (choice == 1)
		{
		
			printf("\nEnter the dataword::");
			
			scanf("%d",&buffer);
			
			
			n= write(sockfd,&buffer,sizeof(int));
			if (n<0)
				error("Error on write!\n");
			
			buffer=0;
			
			n=read(sockfd,&buffer,sizeof(int));
			if (n<0)
				error("Error on read\n");	
			
			printf("\nCodeword::%d",buffer);
			
		
		}
		else
			break;
		
		
		
	}
	
	close(sockfd);
	return 0;
		
}
