#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#define PORT 8080 
#define MAX_LEN 100

int main(int argc, char *argv[]) {
int listenfd, connfd;
struct sockaddr_in servaddr, cliaddr;
char buffer[MAX_LEN];
char log_file_name[MAX_LEN];
FILE *log_file;
time_t raw_time;
struct tm *time_info;
char time_str[MAX_LEN];
char *client_ip;
char *mssv, *ho_ten, *ngay_sinh, *diem_tb;

if (argc != 3) {
    printf("Usage: %s <PORT> <log_file_name>\n", argv[0]);
    return 1;
}

// Tao socket
listenfd = socket(AF_INET, SOCK_STREAM, 0);
if (listenfd == -1) {
    perror("socket");
    return 1;
}

// Thiet lap dia chi server
memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
servaddr.sin_port = htons(PORT);

// Gan socket vào dia chi
if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
    perror("bind");
    return 1;
}

// Lang nghe ket noi tu client
if (listen(listenfd, 5) == -1) {
    perror("listen");
    return 1;
}

// Mo file log
strcpy(log_file_name, argv[2]);
log_file = fopen(log_file_name, "a");
if (log_file == NULL) {
    perror("fopen");
    return 1;
}

// chap nhan ket noi va xu ly
while (1) {
    // Chap nhan ket noi tu client
    socklen_t cliaddr_len = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
    if (connfd == -1) {
        perror("accept");
        return 1;
    }

    // Nhan thông tin tu client
    read(connfd, buffer, MAX_LEN);
    client_ip = inet_ntoa(cliaddr.sin_addr);
    sscanf(buffer, "%s %s %s %s %s", client_ip, mssv, ho_ten, ngay_sinh, diem_tb);

    // Ghi vào file log
    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_str, MAX_LEN, "%Y-%m-%d %H:%M:%S", time_info);
    fprintf(log_file, "%s %s %s %s %s %s\n", client_ip, time_str, mssv, ho_ten, ngay_sinh, diem_tb);

    // In thông tin ra màn hình
    printf("%s %s %s %s %s %s\n", client_ip, time_str, mssv, ho_ten, ngay_sinh, diem_tb);

    // Ðóng ket noi
    close(connfd);
}

// Ðóng file log và socket
fclose(log_file);
close(listenfd);

return 0;
}
