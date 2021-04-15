#include "client.h"
#include "config.h"
#include "Console.h"

// Socket fd this client use.
int sock;
// Server address.
struct sockaddr_in server;
socklen_t addr_len;
int blocksize = DATA_SIZE;

void help(){
    Console::writeLine("Usage: cmd  arg0[,arg1,arg2...]");
    Console::writeLine("Please input your ip and port");
}

const int SERV_PORT = 6000;

const int MAXLINE = 2048;

void dg_cli(char buff[],int lenth , int sockfd , const struct sockaddr *pservaddr , socklen_t servlen)
{
	int n;

		if(sendto(sockfd , buff , lenth , 0 , pservaddr ,  servlen) < 0)
		{
			perror("sendto error");
			exit(1);
		}// if
}

int send_local_udp(char buff[],int lenth)
{
	static int sockfd , t;
	static struct sockaddr_in servaddr;
	static int is_init=0;
	
	if(is_init==0)
	{
		is_init=1;
    	bzero(&servaddr , sizeof(servaddr));
    	servaddr.sin_family = AF_INET;
    	servaddr.sin_port = htons(9999);
    	if((t = inet_pton(AF_INET , "255.255.255.255", &servaddr.sin_addr)) <= 0)
    	{
    		perror("inet_pton error");
    		exit(1);
    	}// if
    	
    	if((sockfd = socket(AF_INET , SOCK_DGRAM , 0)) < 0)
    	{
    		perror("socket error");
    		exit(1);
    	}// if
		 int on=1;
       setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR | SO_BROADCAST,&on,sizeof(on));
	}
	dg_cli(buff,lenth , sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr));
} 


/* handle the message */
void *recv_message(void *fd)
{
	int sockfd = *(int *)fd;
	while(1)
	{
		char buf[MAX_LINE];
		memset(buf , 0 , MAX_LINE);
		int n;
		if((n = recv(sockfd , buf , MAX_LINE , 0)) == -1)
		{
			perror("recv error.\n");
			exit(1);
		}// if
		buf[n] = '\0';
		
		// exit
		if(strcmp(buf , "byebye.") == 0)
		{
			printf("Server is closed.\n");
			close(sockfd);
			exit(0);
		}// if
        send_local_udp(buf,n);
		printf("\nServer: %s\n", buf);
	}// while
}

int main(int argc, char **argv){
	char cmd_line[LINE_BUF_SIZE];
	char *buf;
	char *arg;
	int i;
	char *local_file;
		/* socket and server */
    int sockfd;
	pthread_t recv_tid , send_tid;
    struct sockaddr_in servaddr;

	int done = 0;	// Server exit.
	char *server_ip;
	unsigned short port = 69;

	addr_len = sizeof(struct sockaddr_in);	
	
	if(argc < 2){
		printf("Usage: %s server_ip [server_port]\n", argv[0]);
		help();
		return 0;
	}

	/*(1) set socket */
    if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
    {
        perror("socket error");
        exit(1);
    }

	/*(2) set struct */
    bzero(&servaddr , sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if(inet_pton(AF_INET , argv[1] , &servaddr.sin_addr) < 0)
    {
        printf("inet_pton error for %s\n",argv[1]);
        exit(1);
    }

	/*(3) set connect request*/
    if( connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
    {
        perror("connect error");
        exit(1);
    }//if	
	
	
	/* creating thread for recerving */
	if(pthread_create(&recv_tid , NULL , recv_message, &sockfd) == -1)
	{
		perror("pthread create error.\n");
		exit(1);
	}
	
	/* handle messages */
	char msg[MAX_LINE];
	memset(msg , 0 , MAX_LINE);
	
	server_ip = argv[1];
	if(argc > 2){
		port = (unsigned short)69;
	}
	printf("Connect to server at %s:%d", server_ip, port);
	
	if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
		printf("Server socket could not be created.\n");
		return 0;
	}
	
	// Initialize server address
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	inet_pton(AF_INET, server_ip, &(server.sin_addr.s_addr));
	
	while(1){	
		sleep(5);
        // start the camera
		system("fswebcam --no-banner -r 640*480 test.jpg");
		if(strcmp("put", "put") == 0){
				do_put("test.jpg");
	  	  if(send(sockfd , "send one pci" , strlen("send one pci") , 0) == -1)
	  	  {
		  	perror("send error.\n");
		  	exit(1);
		  }
		}	
	}
	return 0;
}

// Download a file from the server.
void do_get(char *remote_file, char *local_file){
	struct tftpx_packet snd_packet, rcv_packet;
	int next_block = 1;
	int recv_n;
	int total_bytes = 0;
	struct tftpx_packet ack;	
	struct sockaddr_in sender;
		
	int r_size = 0;
	int time_wait_data;
	ushort block = 1;
	
	// Send request.
	snd_packet.cmd = htons(CMD_RRQ);
	sprintf(snd_packet.filename, "%s%c%s%c%d%c", remote_file, 0, "octet", 0, blocksize, 0);
	sendto(sock, &snd_packet, sizeof(struct tftpx_packet), 0, (struct sockaddr*)&server, addr_len);
	
	FILE *fp = fopen(local_file, "w");
	if(fp == NULL){
		printf("Create file \"%s\" error.\n", local_file);
		return;
	}
	
	// Receive data.
	snd_packet.cmd = htons(CMD_ACK);
	do{
		for(time_wait_data = 0; time_wait_data < PKT_RCV_TIMEOUT * PKT_MAX_RXMT; time_wait_data += 10000){
			// Try receive(Nonblock receive).
			r_size = recvfrom(sock, &rcv_packet, sizeof(struct tftpx_packet), MSG_DONTWAIT,
					(struct sockaddr *)&sender,
					&addr_len);
			if(r_size > 0 && r_size < 4){
				printf("Bad packet: r_size=%d\n", r_size);
			}
			if(r_size >= 4 && rcv_packet.cmd == htons(CMD_DATA) && rcv_packet.block == htons(block)){
				printf("DATA: block=%d, data_size=%d\n", ntohs(rcv_packet.block), r_size - 4);
				// Send ACK.
				snd_packet.block = rcv_packet.block;
				sendto(sock, &snd_packet, sizeof(struct tftpx_packet), 0, (struct sockaddr*)&sender, addr_len);
				fwrite(rcv_packet.data, 1, r_size - 4, fp);
				break;
			}
			usleep(10000);
		}
		if(time_wait_data >= PKT_RCV_TIMEOUT * PKT_MAX_RXMT){
			printf("Wait for DATA #%d timeout.\n", block);
			goto do_get_error;
		}
		block ++;
	}while(r_size == blocksize + 4);
	//printf("\nReceived %d bytes.\n", total_bytes);
	
do_get_error:
	fclose(fp);
}


// Upload a file to the server.
void do_put(char *filename){
	struct sockaddr_in sender;
	struct tftpx_packet rcv_packet, snd_packet;
	int r_size = 0;
	int time_wait_ack;
	
	// Send request and wait for ACK#0.
	snd_packet.cmd = htons(CMD_WRQ);
	sprintf(snd_packet.filename, "%s%c%s%c%d%c", filename, 0, "octet", 0, blocksize, 0);	
	sendto(sock, &snd_packet, sizeof(struct tftpx_packet), 0, (struct sockaddr*)&server, addr_len);	
	for(time_wait_ack = 0; time_wait_ack < PKT_RCV_TIMEOUT; time_wait_ack += 20000){
		// Try receive(Nonblock receive).
		r_size = recvfrom(sock, &rcv_packet, sizeof(struct tftpx_packet), MSG_DONTWAIT,
				(struct sockaddr *)&sender,
				&addr_len);
		if(r_size > 0 && r_size < 4){
			printf("Bad packet: r_size=%d\n", r_size);
		}
		if(r_size >= 4 && rcv_packet.cmd == htons(CMD_ACK) && rcv_packet.block == htons(0)){
			break;
		}
		usleep(20000);
	}
	if(time_wait_ack >= PKT_RCV_TIMEOUT){
		printf("Could not receive from server.\n");
		return;
	}
	
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("File not exists!\n");
		return;
	}
	
	int s_size = 0;
	int rxmt;
	ushort block = 1;
	snd_packet.cmd = htons(CMD_DATA);
	// Send data.
	do{
		memset(snd_packet.data, 0, sizeof(snd_packet.data));
		snd_packet.block = htons(block);
		s_size = fread(snd_packet.data, 1, blocksize, fp);
		
		for(rxmt = 0; rxmt < PKT_MAX_RXMT; rxmt ++){
			sendto(sock, &snd_packet, s_size + 4, 0, (struct sockaddr*)&sender, addr_len);
			printf("Send %d\n", block);
			// Wait for ACK.
			for(time_wait_ack = 0; time_wait_ack < PKT_RCV_TIMEOUT; time_wait_ack += 20000){
				// Try receive(Nonblock receive).
				r_size = recvfrom(sock, &rcv_packet, sizeof(struct tftpx_packet), MSG_DONTWAIT,
						(struct sockaddr *)&sender,
						&addr_len);
				if(r_size > 0 && r_size < 4){
					printf("Bad packet: r_size=%d\n", r_size);
				}
				if(r_size >= 4 && rcv_packet.cmd == htons(CMD_ACK) && rcv_packet.block == htons(block)){
					break;
				}
				usleep(20000);
			}
			if(time_wait_ack < PKT_RCV_TIMEOUT){
				// Send success.
				break;
			}else{
				// Retransmission.
				continue;
			}
		}
		if(rxmt >= PKT_MAX_RXMT){
			printf("Could not receive from server.\n");
			return;
		}
		
		block ++;
	}while(s_size == blocksize);
	
	printf("\nSend file end.\n");
	
do_put_error:
	fclose(fp);
	
	return;
}


// Directory listing.
void do_list(int sock, char *dir){
	struct tftpx_packet packet;	
	int next_block = 1;
	int recv_n;
	struct tftpx_packet ack;	
	struct sockaddr_in sender;
	
	ack.cmd = htons(CMD_ACK);
	
	int r_size = 0;
	int time_wait_data;
	ushort block = 1;
	
	// Send request.
	packet.cmd = htons(CMD_LIST);
	strcpy(packet.data, dir);
	sendto(sock, &packet, sizeof(struct tftpx_packet), 0, (struct sockaddr*)&server, addr_len);
	
	printf("type\tsize\tname\n");
    Console::writeLine("-------------------------------------------------");
	
	// Receive data.
	do{
		for(time_wait_data = 0; time_wait_data < PKT_RCV_TIMEOUT * PKT_MAX_RXMT; time_wait_data += 20000){
			// Try receive(Nonblock receive).
			r_size = recvfrom(sock, &packet, sizeof(packet), MSG_DONTWAIT,
					(struct sockaddr *)&sender,
					&addr_len);
			if(r_size > 0 && r_size < 4){
				printf("Bad packet: r_size=%d\n", r_size);
			}
			if(r_size >= 4 && packet.cmd == htons(CMD_DATA) && packet.block == htons(block)){
				block ++;
				ack.block = packet.block;
				sendto(sock, &ack, sizeof(struct tftpx_packet), 0, (struct sockaddr*)&sender, addr_len);
				fwrite(packet.data, 1, r_size - 4, stdout);
				break;
			}
			usleep(20000);
		}
		if(time_wait_data >= PKT_RCV_TIMEOUT * PKT_MAX_RXMT){
			printf("Wait for DATA #%d timeout.\n", block);
			return;
		}
	}while(r_size == blocksize + 4);
}
