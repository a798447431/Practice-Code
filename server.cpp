/*************************************************************************
	> File Name: server.cpp
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月06日 星期六 21时49分52秒
 ************************************************************************/

#include "common_n.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/select.h>
#include <iostream>
using namespace std;
int sock[200] = {0};

int get_max(int n) {
    int max = 0;
    for (int i = 0; i < n; i++) {
        if (sock[i] > max) {
            max = sock[i];
        }
    }
    return max;
}

int main() {
    int port = 8888;
    int listen_socket;
    if((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    } 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listen_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    listen(listen_socket, 100);

    fd_set set; 
    struct timeval tm; 
    FD_ZERO(&set);
    FD_SET(listen_socket, &set);
    unsigned long ul = 1;
    int index = 0;  //总人数
    int error = -1;
    int len = sizeof(int);
    
    while (1) {
        tm.tv_sec = 10;
        tm.tv_usec = 0;
        int max_fd = get_max(index);
        if (max_fd < listen_socket) max_fd = listen_socket;
        int reval = select(max_fd+1, &set, NULL, NULL, &tm);
        if (reval == -1) {
            perror("select");
            break;
        } else if (reval == 0) {
            cout << "--本轮无通信--" <<endl;
            continue;
        }

        if (FD_ISSET(listen_socket, &set)) {
           int new_socket; 
            if( ( new_socket = accept(listen_socket, NULL, NULL))<0){
                perror("accept");
            }
            ioctl(new_socket, FIONBIO, &ul);
            FD_SET(new_socket, &set);
            sock[index++] = new_socket;
            cout << "加入成功 :" << new_socket <<endl;
        }else{
            for (int i = 0; i < index; i++) {
                if (!FD_ISSET(sock[i], &set)) continue;
                cout << sock[i] << endl;
                int k , t;
                if((t = recv(sock[i], &k, sizeof(k), 0)) < 0) {
                    perror("recv");
                }if(t > 0)
                cout << i << "-->" << k<<endl;
                k = 9999;
                if(send(sock[i], &k, sizeof(k), 0) < 0){
                    perror("send");
                }
                if (t == 0){
                    FD_CLR(sock[i], &set);
                    close(sock[i]);
                    sock[i] = -1;
                    cout << "对方退出" <<endl; 
                }
            } 
        }
    }
    close(listen_socket);
    return 0;
}
