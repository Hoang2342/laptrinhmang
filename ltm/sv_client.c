#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8080 
#define MAX_LEN 100

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAX_LEN];
    char *client_ip;
    char *mssv, *ho_ten, *ngay_sinh, *diem_tb;
    char IP_sv[225];
    printf("Nhap dia chi server\n");
    gets(IP_sv);

    if (argc != 3) {
        printf("Usage: %s <IP_sv> <PORT>\n", argv[0]);
        return 1;
    }

    // Tao socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Thiet lap dia chi server
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(PORT);

    // ket noi toi server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("connect");
        return 1;
    }

    // Nhap thông tin sinh viên
    printf("Nhap MSSV: ");
    fgets(buffer, MAX_LEN, stdin);
    mssv = strtok(buffer, "\n");

    printf("Nhap ho ten: ");
    fgets(buffer, MAX_LEN, stdin);
    ho_ten = strtok(buffer, "\n");

    printf("Nhap ngay sinh (yyyy-mm-dd): ");
    fgets(buffer, MAX_LEN, stdin);
    ngay_sinh = strtok(buffer, "\n");

    printf("Nhap diem trung binh: ");
    fgets(buffer, MAX_LEN, stdin);
    diem_tb = strtok(buffer, "\n");

    // Gui thong tin toi server
    client_ip = inet_ntoa(servaddr.sin_addr);
    sprintf(buffer, "%s %s %s %s %s", client_ip, mssv, ho_ten, ngay_sinh, diem_tb);
    write(sockfd, buffer, strlen(buffer));

    // Ðóng ket noi
    close(sockfd);

    return 0;
}

