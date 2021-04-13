#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <jsoncpp/json/json.h>
#include "PCA9685.h"
#include "servo.h"
#include "client.h"

int sendtomyserver(char bufdata[]);

int main(int argc, char *argv[])
{
	int server_sockfd;
	int len;
	
	struct sockaddr_in my_addr;   //服务器网络地址结构体
    struct sockaddr_in remote_addr; //客户端网络地址结构体
	socklen_t sin_size;
	char buf[BUFSIZ];  //数据传送的缓冲区
	memset(&my_addr,0,sizeof(my_addr)); //数据初始化--清零
	my_addr.sin_family=AF_INET; //设置为IP通信
	my_addr.sin_addr.s_addr=INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
	my_addr.sin_port=htons(9999); //服务器端口号
	
	/*创建服务器端套接字--IPv4协议，面向无连接通信，UDP协议*/
	if((server_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
	{  
		perror("socket error");
		return 1;
	}
	
	/*将套接字绑定到服务器的网络地址上*/
	if (bind(server_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
	{
		perror("bind error");
		return 1;
	}
	sin_size=sizeof(struct sockaddr_in);
	printf("waiting for a packet.../n");
	
	/*接收客户端的数据并将其发送给客户端--recvfrom是无连接的*/
	if((len=recvfrom(server_sockfd,buf,BUFSIZ,0,(struct sockaddr *)&remote_addr,&sin_size))<0)
	{
		perror("recvfrom error"); 
		return 1;
	}

	printf("received packet from %s:/n",inet_ntoa(remote_addr.sin_addr));
	buf[len]='/0';

	//读取json转化字符串,将json内容赋值给变量
	Json::Reader reader;  
    Json::Value value; 
    reader.parse(buf, value);
    std::string gettype = value["keyword"].asString();  
    std::cout << gettype << std::endl;

	/* 例子
	result["score"] = 0.8;
    result["root"] = "商品-容器";
    result["keyword"] = "瓶子";*/
    
	//商品类型<->数字类型 info定义一个字典
	/*
		0:可回收
		1:有害
		2:厨余
		3:其他
		4无效数据
	*/
	Json::Value typeinfo;
	typeinfo["瓶子"] = 0;
	typeinfo["纸"] = 0;
	typeinfo["罐子"] = 0;
	typeinfo["药品"] = 1;
	typeinfo["电池"] = 1;
	
	int typenum = 4;//初始化typenum
	if(typeinfo[gettype].isString())
	{
		std::string typenumber = typeinfo[gettype].asString();
		typenum = std::stoi(typenumber);
	}
	else 
		typenum = 3;//未找到数据 返回为其他垃圾

	//调用周姐函数操作舵机
	if(typenum < 4 && typenum > -1)
	{
		servo::servo s1 = servo();
		s1.set_servo_open(typenum);
		s1.set_servo_close(typenum);
	}

	//TCP传给服务器 o
	sendtomyserver(buf);

	/*关闭套接字*/
	close(server_sockfd);

    return 0;
}
