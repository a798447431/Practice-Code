/*************************************************************************
	> File Name: myinet_ntoa.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月18日 星期四 21时02分19秒
 ************************************************************************/

#include <stdio.h>
#include <arpa/inet.h>

char *my_inet_ntoa(struct in_addr in) {
    static char ip[20] = {0};
    char *p;
    p = (char *)&in.s_addr;
    sprintf(ip, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return ip;
}

int main(int argc, char **argv) {
    char *ip = argv[1];
    struct in_addr in;
    in.s_addr = inet_addr(ip);
    printf("%d\n", in.s_addr);
    printf("%s\n", my_inet_ntoa(in)); 
    return 0;
}
