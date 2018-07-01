#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define _BACKLOG_ 5
int main(int argc, char* argv[]){
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock < 0){
		perror("socket");
	}
	struct sockaddr_in server;
	struct sockaddr_in client;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2])); 
	server.sin_addr.s_addr = inet_addr(argv[1]);
	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server))<0){
		perror("bind");
		close(listen_sock);
		return 1;
	}
	if(listen(listen_sock, _BACKLOG_)< 0){
		perror("listen");
		close(listen_sock);
		return 2;
	}

	printf("listen...\n");
	
	for(;;){
		socklen_t len = 0;
		int sock = accept(listen_sock, (struct sockaddr*)&client, &len);
		if(sock < 0){
			perror("accept");
		}
		char buf[1024];
		inet_ntop(AF_INET, &client.sin_addr, buf, sizeof(buf));
		printf("get connect from %s:%d\n", buf, ntohs(client.sin_port));
		while(1){
			memset(buf, 0, sizeof(buf));
			read(sock, buf, sizeof(buf));
			printf("client:%s\n", buf);
			printf("server:");
			memset(buf, 0, sizeof(buf));
			fgets(buf, sizeof(buf)-1, stdin);
			buf[strlen(buf)] = 0;
			write(sock, buf, strlen(buf));
			printf("wait...\n");
		}

	}
	close(listen_sock);
}
