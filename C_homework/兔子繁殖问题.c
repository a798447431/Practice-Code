/*************************************************************************
	> File Name: 兔子繁殖问题.c
	> Author: 
	> Mail: 
	> Created Time: 2019年04月07日 星期日 16时10分57秒
 ************************************************************************/

#include<stdio.h>
#define max 100
#define max_n 1000

int array[max + 5][max_n + 5] = {0};

int main() {
    int n;
    scanf("%d", &n);
    array[1][0] = array[1][1] = 1;
    array[2][1] = 2;

    for (int m = 3; m <= n; m++) {
        for (int i = 1; i <= array[1][0]; i++) {
            array[m][i] = array[m - 1][i] + array[m - 2][i];
        }

        for (int j = 1; j <= array[1][0]; j++) {
            if (array[m][j] < 10) continue;
                array[m][j + 1] += array[m][j] / 10;
                array[m][j] %= 10;
                array[1][0] += (j == array[1][0]);
        }
    }
    
    for (int k = array[1][0]; k >= 1; k--) {
        printf("%d", array[n][k]); 
    }

    printf("\n");
    return 0;
}

