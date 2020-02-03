/*************************************************************************
	> File Name: EP20.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月24日 星期一 21时01分33秒
 ************************************************************************/

#include<stdio.h>

int ans[500] = {0};

int main() {
    ans[0] = ans[1] = 1;
    for (int i = 2; i <= 100; i++) {
        for (int j = 1; j <= ans[0]; j++) {
            ans[j] *= i;
        }
        for (int j = 1; j <= ans[0]; j++) {
            if (ans[j] < 10) continue;
            ans[j + 1] += ans[j] / 10;
            ans[j] %= 10;
            if (j == ans[0]) ++ans[0];
        }
    }
    int sum = 0;
    for (int j = 1; j <= ans[0]; ++j) {
        sum += ans[j];
    }
    printf("%d\n", sum);
    return 0;
}
