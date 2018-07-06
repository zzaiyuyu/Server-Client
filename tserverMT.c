#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

typedef struct Arg_{
	int sock;
	struct sockaddr_in client;	
}Arg;


void Usage()
{
	printf("Usage: ./server [ip] [port] \n");
}

void ProcessRequest(int sock, struct sockaddr_in* client){
	char buf[1024];
	printf("client: %s login\n", inet_ntoa(client->sin_addr));
	for(;;){
		memset(buf, 0, sizeof(buf));
		printf("waiting...\n");
		ssize_t read_size = read(sock , buf, sizeof(buf));
		if(read_size<0){
			perror("read");
			return;
		}
		if(read_size == 0){
			printf("client:%s say bye!\n", inet_ntoa(client->sin_addr));
			close(sock);
			return;
		}
		buf[read_size] = '\0';
		printf("client %s say:%s\n", inet_ntoa(client->sin_addr), buf);
		write(sock, buf, strlen(buf));
	}
	return;
}

void CreateWorker(void* ptr){
	Arg* arg = (Arg*)ptr;
	ProcessRequest(arg->sock, &arg->client);	
	free(arg);
	return NULL;
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
		pthread_t tid;	
		Arg* args = (Arg*)malloc(sizeof(Arg));
		args->sock = sock;
		args->client = client;
		pthread_create(&tid, NULL, CreateWorker, (void*)args);
		pthread_detach(tid);
	}

}
