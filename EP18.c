/*************************************************************************
	> File Name: EP18.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月24日 星期一 19时34分33秒
 ************************************************************************/

#include <stdio.h>

int num[22][22] = {0};
int mamory[22][22] = {0};

int GetMaxSum(int i, int j, int maxnum) {
    if (i + 1 == maxnum) return num[i][j];
    if (mamory[i][j] != 0) return mamory[i][j];
    int ans1, ans2;
    ans1 = GetMaxSum(i + 1, j, maxnum) + num[i][j];
    ans2 = GetMaxSum(i + 1, j + 1, maxnum) + num[i][j];
    mamory[i][j] =  ans1 > ans2 ? ans1 : ans2;
    return mamory[i][j];
}

int main() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j <= i; j++) {
            scanf("%d", &num[i][j]);
        }
    }
    printf("%d\n", GetMaxSum(0, 0, 20));
    return 0;
}
