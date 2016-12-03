#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_CLIENTS 200
#define BUFFER_SIZE 250

int clients[MAX_CLIENTS];

size_t *client_push(int fd) {
	size_t i;

	for(i = 0; i < MAX_CLIENTS; i++) {
		if(!clients[i]) {
			clients[i] = fd;
			return (size_t *) &clients[i];
		}
	}

	return NULL;
}

void broadcast(char *buffer, size_t size) {
	size_t i;

	for(i = 0; i < MAX_CLIENTS; i++) {
		if(clients[i]) {
			send(clients[i], buffer, size, 0);
		}
	}
}

void *connection_handler(void *arg) {
	int fd = *((int *) arg);
	char *buffer = calloc(BUFFER_SIZE, 1);
	ssize_t n;

	while(1) {
		(n = recv(fd, buffer, BUFFER_SIZE, 0));
		printf("buf!\n");
		broadcast(buffer, n);
	}

	printf("no more bufs!\n");

	return NULL;
}

int main() {
	const int port_number = 3001;
	int sockfd;
	int clientfd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t client_len;
	pthread_t thread;
	int option = 1;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	if(sockfd < 0) {
		perror("Error opening socket");
		return 1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port_number);

	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		perror("Error binding to socket");
		return 1;
	}

	listen(sockfd, 5);

	client_len = sizeof(client_addr);

	while(1) {
		clientfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);

		if(clientfd < 0) {
			perror("Error on accept");
			break;
		}

		pthread_create(&thread, NULL, &connection_handler, client_push(clientfd));
	}
}
