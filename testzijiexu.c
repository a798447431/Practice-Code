/*************************************************************************
	> File Name: testzijiexu.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月26日 星期日 19时50分45秒
 ************************************************************************/
#include <stdio.h>
union
{
    char ch;
    int i;
}un;
int main(void)
{
    un.i = 0x12345678;
    if(un.ch == 0x12)
    {
        printf("big endian\n");
    }
    else
    {
        printf("small endain\n");
    }
    return 0;
}
