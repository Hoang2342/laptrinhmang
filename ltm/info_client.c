#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main(int argc, char const *argv[]) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Khong the tao socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Khong the ket noi den server");
        exit(EXIT_FAILURE);
    }

    char computer_name[256];
    printf("Nhap ten may tinh: ");
    fgets(computer_name, sizeof(computer_name), stdin);
    computer_name[strcspn(computer_name, "\n")] = 0;

    int num_drives;
    printf("Nhap so o dia: ");
    scanf("%d", &num_drives);
    getchar();  // loai bo ky tu \n trong buffer

    char drives[1000] = "";
    char drive_letter, drive_size[20];
    for (int i = 0; i < num_drives; i++) {
        printf("Nhap o dia thu %d (chu cai va kich thuoc, cach nhau bang khoang trang): ", i+1);
        scanf("%c %s", &drive_letter, drive_size);
        getchar();  // loai bo ky tu \n trong buffer

        char drive_info[50];
        sprintf(drive_info, "%c-%s ", drive_letter, drive_size);
        strcat(drives, drive_info);
    }

    // Dong goi du lieu va gui den server
    char data[1500];
    sprintf(data, "%s|%s", computer_name, drives);
    send(socket_fd, data, strlen(data), 0);

    close(socket_fd);
    return 0;
}

