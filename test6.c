/*************************************************************************
	> File Name: test6.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月10日 星期五 15时16分16秒
 ************************************************************************/
#include<string.h>
#include<stdio.h>

int main() {
    const char *split = "/";
    char input[100]="home/szt/linux.c";
    char filename[100] = "";
    char *p;
    p = strtok(input, split);
    while (p != NULL) {
        filename[0] = '\0';
        sprintf(filename, "%s", p);
        p = strtok(NULL, split);
    }
    printf("%s\n", filename);
    return 0;

}

