#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define _BACKLOG_ 5

int main(int argc, char* argv[])
{

	char buf[1024];

	struct sockaddr_in server;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);

	int ret = connect(sock, (struct sockaddr*)&server, sizeof(server));
	if(ret < 0){
		perror("connect");
	}
	printf("connect success\n");
	while(1){
		printf("client:");
		fgets(buf, sizeof(buf)-1, stdin);
		buf[strlen(buf)] = 0;
		write(sock, buf, sizeof(buf));
		printf("wait..\n");
		memset(buf, 0, sizeof(buf));
		read(sock, buf, sizeof(buf));
		printf("server:%s\n", buf);
	}
	close(sock);
}
