/*************************************************************************
	> File Name: p1290.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年04月22日 星期一 18时54分42秒
 ************************************************************************/

#include<stdio.h>

void swap(int *m, int *n) {
    int temp = *m;
    *m = *n;
    *n = temp;
}

int main() {
    int length;
    int m, n;
    scanf("%d", &length);
    while(length--) {       
        
        scanf("%d%d", &m, &n);
        
        if (m < n) {
            swap(&m, &n);
        }

        int flag = 1; //判断当前情况是谁在操作 1为Stan 0为Ollie

        while (m % n != 0 && m / n == 1) {
            m -= n;
            if (m < n) {
                swap(&m, &n);
            }
            flag = !flag; //交换操作者
        }
        if (flag) {
            printf("Stan wins\n");
        } else {
            printf("Ollie wins\n");
        }
    }

    return 0;
}

