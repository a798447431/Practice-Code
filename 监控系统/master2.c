/*************************************************************************
	> File Name: master2.c
	> Author: szt
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月25日 星期四 19时51分12秒
 ************************************************************************/

#include "master2.h"

int main() {
    char *config = "./pihealthd.conf";
    int server_listen;
    int Port, Ins, CHPort, CTLPort; //监听端口，心跳端口，并发度
    long TimeOut; //心跳超时时间
    char FromIp[20] = {0}; //起始ip地址
    char ToIp[20] = {0}; //终止ip地址
    pthread_t HeartTid, t[Ins];
    
    //提取配置文件信息
    char tmp[20] = {0};
    
    get_conf_value(config, "INS", tmp);
    Ins = atoi(tmp);
    memset(tmp, 0, sizeof(tmp));
    
    get_conf_value(config, "From", tmp);
    strcpy(FromIp, tmp);
    memset(tmp, 0, sizeof(tmp));
    
    get_conf_value(config, "To", tmp);
    strcpy(ToIp, tmp);
    memset(tmp, 0, sizeof(tmp));
    
    get_conf_value(config, "ClientHeart", tmp);
    CHPort = atoi(tmp);
    memset(tmp, 0, sizeof(tmp));
    
    get_conf_value(config, "MasterPort", tmp);
    Port = atoi(tmp);
    memset(tmp, 0, sizeof(tmp));
    
    get_conf_value(config, "DataPort", tmp);
    CTLPort = atoi(tmp);
    memset(tmp, 0, sizeof(tmp));
    
    get_conf_value(config, "TimeOut", tmp);
    TimeOut = atol(tmp);
    memset(tmp, 0, sizeof(tmp));
    
    unsigned int begin, end; //将ip地址转化为无符号32位整形，用来遍历
    begin = ntohl(inet_addr(FromIp));
    end = ntohl(inet_addr(ToIp));
    
    //初始化链表指针
    LinkedList *linkedlist = (LinkedList *)malloc(Ins * sizeof(LinkedList));
    int *sum = (int *)malloc(Ins * sizeof(int));
    memset(sum, 0, Ins * sizeof(int));
    
    //传入心跳端口
    struct sockaddr_in initaddr;
    initaddr.sin_family = AF_INET;
    initaddr.sin_port = htons(CHPort);
    initaddr.sin_addr.s_addr = inet_addr("0.0.0.0");

    //初始化链表头结点
    for (int i = 0; i < Ins; i++) {
        Node *p = (Node *)malloc(sizeof(Node));
        p->client_addr = initaddr;
        p->node_sock = -1;
        p->next = NULL;
        linkedlist[i] = p;
    }
    
    //将存储不同ip地址信息的节点尾插入链表中
    for (unsigned int i = begin; i <= end; i++) {
        if (i % 256 == 0 || ((i + 1) % 256 == 0)) {
            continue;
        }
        initaddr.sin_addr.s_addr = htonl(i);
        Node *p = (Node *)malloc(sizeof(Node));
        p->client_addr = initaddr;
        p->next = NULL;
        p->node_sock = -1;
        int sub = find_min(sum, Ins);
        insert(linkedlist[sub], p);
        sum[sub]++;
    }
    
    //打印链表存储信息
    for (int i = 0; i < Ins; i++) {
        printf("Ins <%d>:\n", i);
        print_linkedlist(linkedlist[i]);
    }
    
    struct HEART heartarg;
    heartarg.sum = sum;
    heartarg.head = linkedlist;
    heartarg.ins = Ins;
    heartarg.timeout = TimeOut;
    
    if (pthread_create(&HeartTid, NULL, heartbeat_all, (void *)&heartarg) < 0) {
        perror("pthread_create");
        exit(1);
    }
    
    struct WORK workarg[Ins];
    
    sleep(5);

    /*
    for (int i = 0; i < Ins; i++) {
        workarg[i].head = linkedlist[i];
        workarg[i].index = i;
        if (pthread_create(&t[i], NULL, work, (void *)&workarg[i]) < 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    */
     
    for (int i = 0; i < Ins; i++) {
        workarg[i].head = linkedlist[i];
        workarg[i].index = i;
        workarg[i].ctlport = CTLPort;
        if (pthread_create(&t[i], NULL, do_work, (void *)&workarg[i]) < 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    
    
    if ((server_listen = socket_create(Port)) < 0) {
        perror("server_listen");
        exit(1);
    }
    
    do_epoll(server_listen, linkedlist, sum, Ins, CHPort);

    for (int i = 0; i < Ins; i++) {
        pthread_join(t[i], NULL);
    }

    pthread_join(HeartTid, NULL);
   
    sleep(1000);
    return 0;
}

