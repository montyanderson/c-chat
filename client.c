#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void dieOnError(int res) {
	if(res < 0) {
		perror("");
		exit(1);
	} 
}

int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in addr;
	ssize_t n;

	dieOnError(sock = socket(AF_INET, SOCK_STREAM, 0));

	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3001);

	dieOnError(connect(sock, (struct sockaddr *) &addr, sizeof(addr)));

	char message[] = "Hello, World!";

	send(sock, message, strlen(message), 0);

	char buffer[200];

	while((n = recv(sock, buffer, sizeof(buffer - 1), 0)) > 0) {
		buffer[n] = 0;
		printf("%s\n", buffer);
	}
}
