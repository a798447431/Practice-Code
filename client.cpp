/*************************************************************************
	> File Name: client.cpp
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月07日 星期日 16时22分37秒
 ************************************************************************/

#include "common_n.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main() {
    int port = 8888;
    char ip[20] = "192.168.2.62";
    int socket = socket_connect(port, ip);
    //char buff[100] = "hello\n";
    int k=1 ;
    int num = 1;
    while(cin >> num && num != 0){

    k = num;
    if (send(socket, &k, sizeof(k), 0) < 0) {
        perror("send");
    }
    if (recv(socket, &k,sizeof(k), 0) < 0) {
        perror("recv");
    }
    //memset(buff, 0, sizeof(buff));
    cout << k << endl;
    }
    close(socket);
    return 0;
}




