/*************************************************************************
	> File Name: common.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月26日 星期三 18时58分41秒
 ************************************************************************/

#include "common.h"
/*
int socket_connect(int port, char *host, struct timeval tv) {
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
    unsigned long flag=1; 
    if (ioctl(sockfd, FIONBIO, &flag) != 0) { 
        close(sockfd); 
        exit(1); 
    } 
    fd_set wset;
    
	connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    
    FD_ZERO(&wset);
    FD_SET(sockfd, &wset);
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int retval;
    char buff[4096] = {0};
    retval = select(sockfd + 1, NULL, &wset, NULL, &tv);
     
    if (retval == -1) perror("select()");
    
    else if (retval) {
        printf("connect success, wait to recv :\n");
        int i;
        recv(sockfd, &i, sizeof(i), 0);
        printf("recv：%d\n", i);
    }

    else printf("not connect in five seconds!\n");
	return sockfd;
}
*/
int socket_create(int port) {
    int sockfd;
    struct sockaddr_in my_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        return -1;
    }
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t socklen = sizeof(struct sockaddr);
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind() error");
        return -1;
    }
    if (listen(sockfd, 20) < 0) {
        perror("listen() error");
        return -1;
    }
    return sockfd;
}

