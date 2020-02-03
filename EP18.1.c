/*************************************************************************
	> File Name: EP18.1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月24日 星期一 20时40分09秒
 ************************************************************************/
#include <stdio.h>
#include <inttypes.h>

int32_t f[22][22] = {0};
int32_t num[22][22] = {0};

int32_t main() {
    for (int32_t i = 0; i < 20; ++i) {
        for (int32_t j = 0; j <= i; ++j) {
            scanf("%d", &num[i][j]);
        } 
    }

    for (int32_t i = 0; i < 20; ++i) {
        f[19][i] = num[19][i];
    }

    for (int32_t i = 18; i >= 0; --i) {
        for (int32_t j = 0; j <= i; ++j) {
            f[i][j] = f[i + 1][j];
            if (f[i + 1][j + 1] > f[i][j]) 
                f[i][j] = f[i + 1][j + 1];
            f[i][j] += num[i][j];
        }
    }
    printf("%d\n", f[0][0]);
    return 0;

}
