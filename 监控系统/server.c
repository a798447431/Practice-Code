/*************************************************************************
	> File Name: server.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月20日 星期六 15时16分36秒
 ************************************************************************/

#include "common.h"

LinkList *l = init_linklist();
char *value, *ip;
int errno = -1;

void init_list() {
    char file[50], key[50], val[50];
    int sip, eip;
    strcpy(key, "_IP");
    get_conf_value(file, key, value);
    strcpy(ip, value);
    memset(value, 0, sizeof(value));
    strcpy(key, "SIP");
    get_conf_value(file, key, value);
    sip = atoi(value);
    memset(value, 0, sizeof(value));
    strcpy(key, "EIP");
    get_conf_value(file, key, value);
    eip = atoi(value);
    memset(value, 0, sizeof(value));
    for (int i = sip; i < eip; i++) {
        char tem_buf[50];
        sprintf(tmp_buf, "%s%d", ip, i);
        Node *node = *init_listnode(tmp_buf);
        l = insert(l, node);
    }
}

void *heartbeat(void *arg){
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 500000;
    int n = 0;
    fd_set fds;
    while(1){
        FD_ZERO(&fds);
        int maxFd = 0;
        for(int i = 0; i < l->length; i++) {
            ListNode *p = &(l->head);
            while(p && p->next){
                char *tmp_ip = p->c_info.ip;
                int t_fd = socket_noblock_connect(tmp_ip, port);
                if(t_fd == -1){
                    perror("socket_noblock_connect() error");
                    p = p->next;
                    close(t_fd);
                    continue;
                }
                p = p->next;
                maxFd = maxFd > t_fd ? maxFd : t_fd;
                FD_SET(fds, t_fd);
            }
        }
        int ret = select(maxFd + 1, &fds, NULL, NULL, &tv);
        if (ret != 0) {
            for(i = 0; i < l->length; i++) {
                while(p->next) {
                    if(FD_ISSET(t_fd, &fds)){
                        getsockopt(a_fd, SOL_SOCKET, SO_ERROR, &errno, (socklen_t*)&len);
                        if (errno != 0){
                            erase(l, i);
                        } else{
                            p = p->next;
                        }
                    }else{
                        erase(l, i);
                    }
                    close(t_fd);
                }
            }
        }
    }
}

int main() {
    //读取配置文件内容
    //将范围内所有主机初始化到链表
    init_list();
    int slisten_fd = server_socket_create(listen_port, 20);
    struct sockaddr_in peer_addr;
    socklen_t socklen = sizeof(struct sockaddr);
    pthread_t pth_heart;
    pthread_create(&pth_heart, NULL, heartbeat, NULL);
    while (1) {
        int sockfd;
        if ((sockfd = accept(sockfd, (struct sockaddr *)&peer_addr, &socklen)) == -1) {
            //获取clinet的ip，将信息加入链表
            char *tmp_ip;
            tmp_ip = inet_ntoa(peer_addr.sin_addr);
            Node *node = *init_listnode(tmp_ip);
            l = insert(node);
        }        
    }
    
    return 0;
}
