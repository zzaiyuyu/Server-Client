#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

void Usage()
{
	printf("Usage: ./server [ip] [port] \n");
}

void ProcessRequest(int sock, struct sockaddr_in* client){
	char buf[1024*10];
	printf("client: %s login\n", inet_ntoa(client->sin_addr));
	for(;;){
		ssize_t read_size = read(sock , buf, sizeof(buf));
		if(read_size<0){
			perror("read");
			return;
		}
		if(read_size == 0){
			printf("client: %s say bye!\n", inet_ntoa(client->sin_addr));
			close(sock);
			return;
		}
		buf[read_size] = '\0';
		printf("client %s say: %s\n", inet_ntoa(client->sin_addr), buf);
		const char* hello = "HTTP/1.0 200 OK\r\nContent-Length:5\r\n\r\nhello";
		write(sock, hello, strlen(hello));
	}
	return;
}

void CreateWorker(int sock, struct sockaddr_in* client){
	pid_t pid = fork();
	if(pid < 0){
		perror("fork");
		return;
	}else if(pid == 0){
		if(fork() == 0){
			ProcessRequest(sock, client);
		}
		exit(0);
	}else{
		close(sock);
		waitpid(pid, NULL, 0);
	}
}
int main(int argc, char* argv[]){
	if(argc <3){
		Usage();
		return 1;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);

	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_socket < 0){
		perror("listen_socket");
		return 1;
	}

	int ret = bind(listen_socket,(struct sockaddr*)&server, sizeof(server));
	if(ret < 0){
		perror("bind");
		return 1;
	}
	ret = listen(listen_socket, 10);
	if(ret < 0){
		perror("listen");
		return 1;
	}
	printf("listen...\n");
	for(;;){
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		int sock = accept(listen_socket, (struct sockaddr*)&client, &len);
		if(sock <0){
			perror("accept");
			continue;
		}
		CreateWorker(sock ,&client);
	}

}
