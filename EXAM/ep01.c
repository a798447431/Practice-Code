/*************************************************************************
	> File Name: ep01.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月01日 星期一 20时23分13秒
 ************************************************************************/

#include <stdio.h>

int main() {
    int sum3, sum5, sum15;
    sum3 = ((3 + 999) * 333) / 2;
    sum5 = ((5 + 995) * 199) / 2;
    sum15 = ((15 + 990) * 66) / 2;
    printf("%d\n", sum3 + sum5 - sum15);
    return 0;
}
