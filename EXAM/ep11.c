/*************************************************************************
	> File Name: ep11.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 13时57分42秒
 ************************************************************************/

#include <stdio.h>

int nums[35][35] = {0};
int direct[4][2] = {0, 1, 1, 1, 1, 0, 1, -1};

int main() {
    for (int i = 5; i <= 24; i++) {
        for (int j = 5; j <= 24; j++) { 
            scanf("%d", &nums[i][j]);
        }
    }
    int p, ma = 0;
    for (int i = 5; i <= 24; i++) {
        for (int j = 5; j <= 24; j++) {
            for (int k = 0; k < 4; k++) {
                p = 1;
                for (int l = 0; l < 4; l++) {
                    p *= nums[i + l * direct[k][0]][j + l * direct[k][1]];
                }
                if (p > ma) ma = p;
            }
        }
    }
    printf("%d\n", ma);
    return 0;
}
