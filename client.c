#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char *argv[]) {
	struct sockaddr_in server_addr;
	struct hostent *server;
	char *hostname = argv[1];
	int port = atoi(argv[2]);

	server = gethostbyname(hostname);

	if(hostname == NULL || port == 0) {
		printf("Usage: %s [hostname] [port]\n", argv[0]);
		return 1;
	}

	server_addr.sin_family = AF_INET;
	inet_aton(server, &server_addr.sin_addr.s_addr);
	bcopy((char *)server->h_addr,
	  (char *)&server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port = htons(port);
}
