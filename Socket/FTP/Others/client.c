#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
 
 
int main() {
    char filename[100]; //保存文件名
    printf("input the file name: ");
    gets(filename);
    //scanf("Input file name to save:%s ", filename);
    FILE *fp = fopen(filename, "wb");
    if(fp == NULL) {
        printf("open file failed!!\r\n");
        exit(1);
    }
    int sock_cli = socket(PF_INET,SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = PF_INET;
    serveraddr.sin_port = htons(8888);
    serveraddr.sin_addr.s_addr =inet_addr("192.168.2.58");
 
    if (connect(sock_cli,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) <0) {
	    perror("connect failed!\r\n");
	    exit(1);
    }
    
    int ncount = 0;
    char recvbuf[1024] = {0};
    while( (ncount = recv(sock_cli, recvbuf, 1024, 0)) >0) {
        fwrite(recvbuf, 1, ncount, fp);
    }
    puts("file transfer succeed!!\r\n");
    memset(recvbuf, 0, 1024);
    fclose(fp);
    close(sock_cli);
    return 0;
}
