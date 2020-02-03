/*************************************************************************
	> File Name: ep25.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 15时02分35秒
 ************************************************************************/

#include <stdio.h>

int fib[3][1005] = {0};

int main() {
    fib[1][0] = fib[1][1] = 1;
    fib[2][0] = fib[2][1] = 1;
    int many = 2, ind1, ind2, ind3;
    while (fib[many % 3][0] < 1000) {
        ++many;
        ind1 = (many - 2) % 3;
        ind2 = (many - 1) % 3;
        ind3 = many % 3;
        for (int i = 1; i <= fib[ind2][0]; i++) {
            fib[ind3][i] = fib[ind1][i] + fib[ind2][i];
        }
        fib[ind3][0] = fib[ind1][0];
        for (int i = 1; i <= fib[ind3][0]; i++) {
            if (fib[ind3][i] >= 10) {
                fib[ind3][i + 1] += fib[ind3][i] / 10;
                fib[ind3][i] %= 10;
                if (i == fib[ind3][0]) ++fib[ind3][0];
            }
        }
    }
    printf("%d\n", many);
    return 0;
}
