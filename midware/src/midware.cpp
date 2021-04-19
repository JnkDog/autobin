#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <jsoncpp/json/json.h>
#include "PCA9685.h"
#include "servo.h"
#include "client.h"

/*	set some const variable and info fist of the program*/
Json::Value typeinfo;
typeinfo["瓶子"] = "0";
typeinfo["纸"] = "0";
typeinfo["书"] = "0";
typeinfo["罐子"] = "0";
typeinfo["药品"] = "1";
typeinfo["电池"] = "1";
typeinfo["果"] = "2";

const char *name[] = {"瓶子", "书", "纸", "罐子", "药品", "电池", "果"};
int namelen = 7;

/*	socket initialization:
	Server network address structure
	Client network address structure
	define a buff
	Data initialization-set zero
	Set to IP communication
	Server IP address-allow connection to all local addresses
	Server port number
*/

/* 	the example json result we get from camera:
	json:
	{	
		result:
		{
			one: 
			{
				"score" = 0.8;
				"root" = "商品-容器";
				"keyword" = "瓶子";
			}
		}
	}
*/

/*	Product Types<->number 
	define info as a dictionary
	number:
		0:recyclable trash
		1:Hazardous waste
		2:Kitchen waste 
		3:other garbage
		4:Invalid data
*/

/*	set a function achieve to send infomation to the server	*/
int sendtomyserver(char bufdata[]);

int main(int argc, char *argv[])
{
	int server_sockfd;
	int len;

	struct sockaddr_in my_addr;   
    struct sockaddr_in remote_addr; 
	socklen_t sin_size;
	char buf[BUFSIZ];  
	memset(&my_addr,0,sizeof(my_addr)); 
	my_addr.sin_family=AF_INET; 
	my_addr.sin_addr.s_addr=INADDR_ANY;
	my_addr.sin_port=htons(9999); 
	
	/*Create server-side socket-IPv4 protocol, for connectionless communication, UDP protocol*/
	if((server_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
	{  
		perror("socket error");
		return 1;
	}
	
		/*Bind the socket to the server's network address*/
		if (bind(server_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
		{
			perror("bind error");
			return 1;
		}
		sin_size=sizeof(struct sockaddr_in);
		printf("waiting for a packet.../n");
		
	while(1)
	{	
		/*Receive data from the client and send it to the client - recvfrom is connectionless*/
		if((len=recvfrom(server_sockfd,buf,BUFSIZ,0,(struct sockaddr *)&remote_addr,&sin_size))<0)
		{
			perror("recvfrom error"); 
			return 1;
		}

		printf("received packet from %s:\n",inet_ntoa(remote_addr.sin_addr));
		buf[len]='/0';

		/*read json and transform to string, Assign json content to variable*/
		Json::Reader reader;  
		Json::Value value; 
		reader.parse(buf, value);
		std::string gettype = value["result"][0u]["keyword"].asString();  
		std::cout << gettype << std::endl;
		
		for(int i=0; i<namelen; i++){
			if(gettype.find(name[i]) != -1)
				{
					gettype = name[i];
					break;
				}
		}

		/*	init typenum as a invalid data
			if can't find the info in dict. return as other garbage
		*/
		int typenum = 4;
		if(typeinfo[gettype].isString())
		{
			std::string typenumber = typeinfo[gettype].asString();
			typenum = std::stoi(typenumber);
		}
		else
			typenum = 3;

		/*Call the function to operate the rudder*/
		if(typenum < 4 && typenum > -1)
		{
			servo s1 = servo();
			s1.set_servo_open(typenum);
			s1.set_servo_close(typenum);
		}

		/*send info to the server*/
		sendtomyserver(buf);
	}

	/*close socket*/
	close(server_sockfd);

    return 0;
}
