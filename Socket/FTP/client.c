/*************************************************************************
	> File Name: client.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月19日 星期三 20时19分23秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8888
#define MAXBUF 1024

int socket_connect(int port, char *host) {
    int sockfd;
    struct sockaddr_in dest_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        return -1;
    } 

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(host);

    if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("connect() error");
        return -1;
    }
    return sockfd;
}

int main(int argc, char *argv[]) {
    int socket_fd;
    int port = PORT;
    char ip_addr[20] = "192.168.2.58";
    int read_num;
    socket_fd = socket_connect(port, ip_addr);
    if (socket_fd < 0) {
        exit(1); 
    }
    char buffer[MAXBUF]; 
    bzero(buffer, MAXBUF); 
    if(recv(socket_fd, buffer, MAXBUF, 0) < 0) { 
      perror("Server Recieve Data Failed:"); 
    } 
    // 然后从buffer(缓冲区)拷贝到file_name中 
    char file_name[MAXBUF + 1]; 
    bzero(file_name, MAXBUF + 1); 
    strncpy(file_name, buffer, strlen(buffer)); 
    printf("%s\n", file_name); 
    // 打开文件并读取文件数据 
    FILE *fp = fopen(file_name, "r"); 
    if(NULL == fp) { 
        printf("File:%s Not Found\n", file_name);
        exit(1);
    } 
    memset(buffer, 0, sizeof(buffer));
    while  ((read_num = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        printf("%s\n", buffer);
        if (send(socket_fd, buffer, strlen(buffer), 0) < 0) {
		    printf("Send Failed\n");
            break;
	    }
        memset(buffer, 0, sizeof(buffer));
    }
    fclose(fp);
    close(socket_fd);
    return 0;
}
