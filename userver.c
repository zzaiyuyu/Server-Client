#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){
	//创建socket,返回值是文件描述符，进程通过这个文件描述符读写网卡
	//AF_INET是ipv4的选项，SOCK_DGRAM面向数据报
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket");
		return 2;
	}

	//构建socket接口,类型端口地址
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(atoi(argv[2]));
	local.sin_addr.s_addr = inet_addr(argv[1]);
	//进程绑定端口
	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
		perror("bind");
		return 3;
	}
	//开始通信
	//UDP面向数据报，只能通过接口读写
	char buf[1024];
	struct sockaddr_in client;
	while(1){
		socklen_t len = sizeof(client);
		ssize_t s = recvfrom(sock, buf, sizeof(buf -1), 0, \
				(struct sockaddr*)&client, &len);
		if(s > 0){
			buf[s] = 0;
			printf("[%s:%d]: %s\n", inet_ntoa(client.sin_addr), \
					ntohs(client.sin_port), buf);
			sendto(sock, buf, strlen(buf), 0, \
					(struct sockaddr*)&client, sizeof(client));
		}

	}
}
