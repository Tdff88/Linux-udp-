#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>


#include <wiringPi.h>

#define SERV_PORT 9527  //端口号，在网络环境中唯一标识一个进程

void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd = 0, cfd = 0;
	int ret;
	char buf[4096], client_IP[1024];

	struct sockaddr_in serv_addr, clit_addr;
	socklen_t clit_addr_len;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);//创建socket
	if (sockfd == -1){//错误检查
		sys_err("socket error");
	}

	if (bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){//错误检查
		sys_err("bind error");
	}


	clit_addr_len = sizeof(clit_addr);


	while (1)
	{
		ret = recvfrom(sockfd, buf, sizeof(buf), 0, (sockaddr *)&clit_addr, &clit_addr_len);

		printf("client ip:%s port:%d\n",
			inet_ntop(AF_INET, (const void *)&clit_addr.sin_addr, client_IP, sizeof(client_IP)),
			ntohs(clit_addr.sin_port));

		write(STDOUT_FILENO, buf, ret);
		for (int i = 0; i < ret; i++)
		{
			buf[i] = toupper(buf[i]);
		}

		sendto(sockfd,buf,sizeof(buf),0, (sockaddr *)&clit_addr, clit_addr_len);

	}

	

	

	close(sockfd);


	return 0;
}
