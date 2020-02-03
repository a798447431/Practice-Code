/************************************************************************
	> File Name: ep07.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 11时10分27秒
 ************************************************************************/

#include <stdio.h>
#define MAX_N 1000000

int prime[MAX_N + 5];

int main() {
    for (int i = 2; i * i <= MAX_N; i++) {
        if (!prime[i]) {
            for (int j = 2 * i; j <= MAX_N; j += i) {
                prime[j] = 1;
            }
        }
    }
    
    for (int i = 2; i <= MAX_N; i++) {
        if (prime[i]) continue;
        prime[++prime[0]] = i;
    }

    printf("%d\n", prime[10001]);
    return 0;
}
