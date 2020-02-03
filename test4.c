/*************************************************************************
	> File Name: test4.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月08日 星期三 23时57分50秒
 ************************************************************************/
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>      
#include <sys/ioctl.h>
#include "errno.h"
 
static int get_screen_width(int *pColum)                         
{//get terminal's width
	struct winsize size;
 
	if(isatty(STDOUT_FILENO)==0)
	{
		printf("not a tty\n");
		return -1;  
	}
 
	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size)<0)
	{
		printf("get win size failed: %s\n", strerror(errno));
		return -1;  
	}
 
	*pColum = size.ws_col;
 
	return 0;
}
 
int printf_list(int col_width, char **pTabl)
{//list info with specified indent
	int max_len = 0;
	int i = 0;
	int one_line_items = 0;
 
	if (pTabl == NULL)
	{
		return -1;	
	}
 
	while (pTabl[i] != NULL)
	{
		if (max_len < strlen(pTabl[i]))
		{
			max_len = strlen(pTabl[i]);	
		}
		i++;
	}
 
	one_line_items  = col_width/(max_len+4);
 
	i = 0;
 
	while (pTabl[i] != NULL)
	{
		printf("%-*s", max_len+4, pTabl[i]);
		if ((i+1) % one_line_items == 0)
		{
			printf("\n");
		}
 
		i++;
	}
	printf("\n");
 
	return 0;
}
 
int main()
{
	int col_width = 0;
	char *buf[] = {
		"a", 	
		"aa",
		"aaa",
		"aaaa",
		"aaaaa",
		"aaaaaa",
		"aaaaa",
		"aaaa",
		"aaa",
		"aa",
		"a",
		NULL
	};
 
	get_screen_width(&col_width);
 
	printf_list(col_width, buf);
 
	return 0;
}
