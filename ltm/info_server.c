#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 8888

int main(int argc, char const *argv[]) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Khong the tao socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Khong the bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) == -1) {
        perror("Khong the lang nghe ket noi den socket");
        exit(EXIT_FAILURE
	}

	printf("Dang cho ket noi tu client...\n");
	
	int client_fd = accept(server_fd, NULL, NULL);
	if (client_fd == -1) {
	    perror("Khong the chap nhan ket noi tu client");
	    exit(EXIT_FAILURE);
	}

	char data[1500];
	recv(client_fd, data, sizeof(data), 0);

// Tach thong tin ten may tinh va cac o dia
	char *token;
	char computer_name[256], drives[1000];
	token = strtok(data, "|");
	strcpy(computer_name, token);
	token = strtok(NULL, "|");
	strcpy(drives, token);
	
	printf("Thong tin may tinh: %s\n", computer_name);
	printf("Danh sach o dia:\n");
	token = strtok(drives, " ");
	while (token != NULL) {
	    char drive_letter = token[0];
	    char *drive_size = token+2;
	    printf("%c - %s\n", drive_letter, drive_size);
	    token = strtok(NULL, " ");
	}

	close(client_fd);
	close(server_fd);
	return 0;
}
