/*************************************************************************
	> File Name: test3.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月07日 星期二 20时16分41秒
 ************************************************************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h> 
int main(void) 
{
    struct winsize size;  
    if (isatty(STDOUT_FILENO) == 0)  
        exit(1);  
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size)<0) 
    {
        perror("ioctl TIOCGWINSZ error");
        exit(1);
    } 

    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);  
    return 0; 
}
