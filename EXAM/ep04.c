/*************************************************************************
	> File Name: ep04.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 09时47分35秒
 ************************************************************************/

#include <stdio.h>

int is_result(int n) {
    int temp = 0, old_num = n;
    while (n) {
        temp = temp * 10 + n % 10;
        n /= 10;
    }
    if (temp == old_num) {
        return 1;
    } else {
        return -1;
    }
    return -1;
}

int main() {
    int ans = 0;
    for (int i = 100; i < 1000; i++) {
        for (int j = i; j < 1000; j++) {
            if (is_result(i * j) == 1) {
                printf("%d\n", i * j);
                if (i * j > ans) ans = i * j;
            }
        }
    }
    printf("%d\n", ans);
    return 0;
}
