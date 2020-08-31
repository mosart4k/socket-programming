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


int main(int argc, char *argv[]) 
{
	if(argc <2) 
	{
		printf("Error!.Port not provided.\n");
		exit(1);
	}
	int sockfd,newsockfd,portno,n;
	int buffer;
	
	
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t clilen; 
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd <0 )
	{
		error("Error opening socket\n");
	}
	
	bzero((char *)&serv_addr,sizeof(serv_addr));
	portno = atoi(argv[1]);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port = htons(portno); 
	
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
		
		n = read(newsockfd,&buffer,sizeof(int));
		if (n <0)
			error("Error!Could not read!\n");
			
		printf("Client-->DataWord:: %d\n",buffer);
		
		int x=buffer;
		int ec;
		
		while(x != 0)
		{
			int p=x%10;
			if (p==1)
				ec++;
			x=x/10;
		}
		
		if (ec%2!=0)
			buffer=buffer*10+1;
		else
			buffer=buffer*10;
			
		n=write(newsockfd,&buffer,sizeof(int));
		if (n<0)
			error("Error on writing!\n");
	}
	
	close(newsockfd);
	close(sockfd);
	return 0;
	
}
