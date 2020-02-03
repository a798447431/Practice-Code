/*************************************************************************
	> File Name: oj156.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月01日 星期一 22时06分14秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_N 9999

int prime[MAX_N + 5] = {0};

int is_value(int n) {
    if (n < 1000 || n >= 10000) return -1;
    int ans = n;
    n /= 100;
    ans %= 100;
    int temp1 = sqrt(n);
    int temp2 = sqrt(ans);
    if (temp1 * temp1 == n && temp2 * temp2 == ans) return 1;
    return -1;
}


int main() {
    int a, b;
    scanf("%d%d", &a, &b);
    for (int i = 2; i <= MAX_N; i++) {
        if (!prime[i]) prime[++prime[0]] = i;
        for (int j = 1; j <= prime[0]; j++) {
            if (i * prime[j] > MAX_N) break;
            prime[i * prime[j]] = 1;
            if (i % prime[j] == 0) break;
        }
    }
    
    int flag = 0;
    int index = 0;

    for (int i = 1; i <= prime[0]; i++) {
        if (prime[i] < a || prime[i] > b) continue;
        if (is_value(prime[i]) == 1) {
            if (index == 1) printf(" ");
            if (!index) index = 1;
            printf("%d", prime[i]);
            flag++;
        }
    }
    printf("\n");
    printf("%d\n", flag);
    return 0;
}
