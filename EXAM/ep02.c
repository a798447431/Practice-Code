/*************************************************************************
	> File Name: ep02.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月01日 星期一 20时34分18秒
 ************************************************************************/

#include <stdio.h>
#define MAX_N 4000000

int fib[55] = {0};
int n = 2;

int main() {
    fib[0] = 1; 
    fib[1] = 2;
    while (fib[n - 1] + fib[n - 2] < MAX_N) {
        fib[n] = fib[n - 1] + fib[n - 2];
        n++;
    }
    
    int ans = 0;

    for (int i = 0; i < n; i++) {
        if (fib[i] % 2 == 0) {
            ans += fib[i];
        }
    }
    printf("%d\n", ans);
    return 0;
}
