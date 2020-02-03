/*************************************************************************
	> File Name: 墙壁涂色.c
	> Author: 
	> Mail: 
	> Created Time: 2019年04月07日 星期日 20时20分01秒
 ************************************************************************/

#include<stdio.h>
#define max 1000

int result[max + 5][max + 5] = {0};

int main() {
    int n, m, k;
    scanf("%d%d", &n, &k); //n代表墙壁数量，ｋ代表颜色数
    result[0][0] = 1;
    result[1][1] = k;
    result[2][1] = k * (k - 1);
    result[3][1] = k * (k - 1) * (k - 2);
    for (m = 4; m <= n; m++) {
        for (int i = 1; i <= result[0][0]; i++) {
            result[m][i] =(k - 2) * result[m - 1][i] + (k - 1) * result[m - 2][i];
        }

        for (int j = 1; j <= result[0][0]; j++) {
            if (result[m][j] < 10) continue;
            result[m][j + 1] += result[m][j] / 10;
            result[m][j] %= 10;
            result[0][0] += (j == result[0][0]);
        }
    }

    for (int k = result[0][0]; k >= 1; k--) {
        printf("%d", result[n][k]);
    }
    printf("\n"); 

    return 0;
}
