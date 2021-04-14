#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <jsoncpp/json/json.h>

int sendtomyserver(char bufdata[])
{
	int client_sockfd;
	int len;
	struct sockaddr_in remote_addr; //Server network address structure
	int sin_size;
	char buf[BUFSIZ];  //The buffer for data transfer
	memset(&remote_addr,0,sizeof(remote_addr)); //Data initialization-set zero
	remote_addr.sin_family=AF_INET; //Set to IP communication
	remote_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//Server IP address
	remote_addr.sin_port=htons(8000); //Server port number

    /*Create a client socket-IPv4 protocol for connectionless communication, UDP protocol*/
	if((client_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
	{  
		perror("socket error");
		return 1;
	}
	strcpy(buf, bufdata); // The content sent
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