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


int main(int argc, char *argv[]) //specifiying number of arguments that will be input
{
	if(argc <3) //if input does not contain the port number
	{
		printf("Error!.Port not provided.\n");
		exit(1);
	}
	int sockfd,newsockfd,portno,n;
	char buffer[255];
	
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
		
		
	int v;
	
	while (1)
	{
		bzero(buffer,255);
		fgets(buffer,255,stdin);
		if (strncmp(buffer,"0",1)==0)
			break;
		n= write(sockfd,buffer,strlen(buffer));
		if (n<0)
			error("Error on write!\n");
			
		bzero(buffer,255);
		n=read(sockfd,&v,sizeof(int));
		if (n<0)
			error("Error on read\n");
		
		printf("server :No. of vowels in the string =%d\n",v);
		
	}
	
	close(sockfd);
	return 0;
		
}
