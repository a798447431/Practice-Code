/*************************************************************************
	> File Name: testEP07.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月15日 星期六 15时07分18秒
 ************************************************************************/

#include <stdio.h>

#define MAX_N 10000
int is_prime[MAX_N + 5];

int main() {
    for (int i = 2; i * i <= MAX_N; i +=  1 + (i % 2)) {
        if (is_prime[i]) continue;
        is_prime[i] = i;
        for (int j = i * i; j <= MAX_N; j += i) {
            if (is_prime[j] == 0) is_prime[j] = i;
        }
    }
    int n;
    while (~scanf("%d", &n)) {
        printf("is_prime[%d] = %d\n", n, is_prime[n]);
    }
    for (int i = 2; i <= MAX_N; i++) {
        if (is_prime[i]) continue;
        is_prime[++is_prime[0]] = i;
    }
    return 0;
}

