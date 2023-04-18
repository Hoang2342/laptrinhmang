#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    FILE* fp = fopen("data.txt", "r");
    if (fp == NULL) {
        printf("Cannot open file.\n");
        exit(EXIT_FAILURE);
    }

    while (!feof(fp)) {
        size_t bytes_read = fread(buffer, sizeof
	char), BUFFER_SIZE, fp);
		if (bytes_read == 0) {
		break;
		}
    send(sock, buffer, bytes_read, 0);
    memset(buffer, 0, BUFFER_SIZE);
	}
	
	fclose(fp);
	shutdown(sock, SHUT_WR);
	
	return 0;
}

