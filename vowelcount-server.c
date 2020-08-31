#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

void error(const char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc, char *argv[]) //specifiying number of arguments that will be input
{
	if(argc <2) //if input does not contain the port number
	{
		printf("Error!.Port not provided.\n");
		exit(1);
	}
	int sockfd,newsockfd,portno,n;
	char buffer[255];
	
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t clilen; //socklen_t is data type in socket.h,32 bit
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd <0 )
	{
		error("Error opening socket\n");
	}
	
	bzero((char *)&serv_addr,sizeof(serv_addr));
	portno = atoi(argv[1]);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port = htons(portno); //htons- Host to network short 
	
	if(bind(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) <0)
	{
		error("Binding failed!\n");
	}
	
	listen(sockfd,5);
	clilen= sizeof(cli_addr);
	newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);
	
	if (newsockfd <0)
		error("Error on accept!\n");
	
	while(1)
	{
		bzero(buffer , 255);
		n = read(newsockfd,buffer,255);
		if (n <0)
			error("Error!Could not read!\n");
		printf("Client : %s\n",buffer);
		
		
		if (strncmp(buffer,"0",1)==0)
			break;
		
		int x;
		int v=0;
		char ch;
		for (int i=0;buffer[i] != '\0';i++)
		{
			ch=buffer[i];
			if(ch == 'a' || ch == 'e'
			|| ch == 'i' || ch == 'o' 
			|| ch == 'u' || ch == 'A' 
			|| ch == 'E' || ch == 'I' 
			|| ch == 'O' || ch == 'U')
			v++;
			else if (ch == ' ')
			continue;
		
		}
		
			
		n=write(newsockfd,&v,sizeof(int));
		if (n<0)
			error("Error on writing!\n");
	}
	
	close(newsockfd);
	close(sockfd);
	return 0;
	
}
