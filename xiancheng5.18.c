/*************************************************************************
	> File Name: xiancheng5.18.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月20日 星期四 13时20分54秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <pthread.h>

struct Person{
    char name[20];
    int height;
};

void *echoinfo(void *arg) {
    struct Person *per = (struct Person *)arg;
    printf("%s\n", per->name);
    printf("%d\n", per->height);
}

int main() {
    struct Person person;
    scanf("%s%d", person.name, &person.height);
    pthread_t pth;
    pthread_create (&pth, NULL, echoinfo, &person);
    pthread_join (pth, NULL);
    return 0;
}
