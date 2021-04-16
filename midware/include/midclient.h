#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <jsoncpp/json/json.h>

/*
most of the socket client step is similiar to the socket server:
	Server network address structure
	The buffer for data transfer
	Data initialization-set zero
	Set to IP communication
	Server IP address
	Server port number
*/

int sendtomyserver(char bufdata[])
{
	int client_sockfd;
	int len;
	struct sockaddr_in remote_addr; 
	int sin_size;
	char buf[BUFSIZ]; 
	memset(&remote_addr,0,sizeof(remote_addr)); 
	remote_addr.sin_family=AF_INET; 
	remote_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	remote_addr.sin_port=htons(8000); 

    /*Create a client socket-IPv4 protocol for connectionless communication, UDP protocol*/
	if((client_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
	{  
		perror("socket error");
		return 1;
	}
	/* The content sent */
	strcpy(buf, bufdata); 
	printf("sending: '%s'/n",buf); 
	sin_size=sizeof(struct sockaddr_in);
	
	/*Send packets to the server*/
	if((len=sendto(client_sockfd,buf,strlen(buf),0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
	{
		perror("recvfrom"); 
		return 1;
	}

	/*close socket*/
	close(client_sockfd);

	return 0;
}