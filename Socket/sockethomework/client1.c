/*************************************************************************
	> File Name: client1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月07日 星期日 10时10分47秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdlib.h>

int main() {
	int sockfd;
    int port = 9999;
    char host[20] = "192.168.2.58";
	struct sockaddr_in addr;
    struct timeval tv;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error");
        return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);
    unsigned long flag=1; 
    if (ioctl(sockfd, FIONBIO, &flag) != 0) { 
        close(sockfd); 
        exit(1); 
    } 
    fd_set wset;
    /*
    if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(0);
    }
    */

    connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    FD_ZERO(&wset);
    FD_SET(sockfd, &wset);

    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int retval;
    retval = select(sockfd + 1, NULL, &wset, NULL, &tv);
     
    if (retval == -1) {
        perror("select()");
    }

    else if (retval) {
        printf("already connect!\n");
        flag = 0;
        ioctl(sockfd, FIONBIO, &flag);  //设置为阻塞模式
        int i = 0; 
        if(FD_ISSET(sockfd, &wset)) {
            if ((recv(sockfd, &i, sizeof(i), 0)) < 0) {
                perror("recv() error");
            }
        }
        printf("recv：%d\n", i);
    }

    else printf("not connect in five seconds!\n");
    
    close(sockfd);
    return 0;
}
