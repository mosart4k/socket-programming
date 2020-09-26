#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>


void error(char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc,char *argv[])
{
	int sockfd,portno,n,length,fromlen;
	char buffer[255];
	
	struct sockaddr_in serv_addr,cli_addr;
	
	
	if(argc <2) 
	{
		printf("Error!.Port not provided.\n");
		exit(1);
	}

	
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd <0 )
	{
		error("Error opening socket\n");
	}
	
	length= sizeof(serv_addr);
	
	bzero((char *)&serv_addr,length);
	
	portno = atoi(argv[1]);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	if(bind(sockfd,(struct sockaddr *) &serv_addr,length) <0)
	{
		error("Binding failed!\n");
	}
	
	
	fromlen = sizeof(cli_addr);
	
	char cmd[][1]={"L","R","F"};
	char mng[][10]={"Left","Right","Forward"};
		
	while(1)
	{
		bzero(buffer,255);
		
		n=recvfrom(sockfd,(char *)buffer,255,0,(struct sockaddr *)&cli_addr,&fromlen);
		
		if (n <0)
			error("Error!Could no recieve!\n");
		
		write(1,"Recevied a datagram: ",21);
		write(1,buffer,n);
		
		if (strncmp(buffer,"0",1)==0)
		{
			printf("Terminated!\n");
			break;
		}
		
		int flag=0;
		
		for(int i=0;i<3;i++)
		{
			if(strncmp(buffer,cmd[i],1)==0)
			{
				strcpy(buffer,mng[i]);
				flag=1;
			}
				
		}
		
		if(flag == 0)
		{
			strcpy(buffer,"Wrong Input!");
		}
		
		
		
		
		n=sendto(sockfd,(char *)buffer,strlen(buffer),0,(struct sockaddr *)&cli_addr,fromlen);
			
		
		if (n<0)
			error("Error on sending!\n");
	}
	
	close(sockfd);
	return 0;
	
}

