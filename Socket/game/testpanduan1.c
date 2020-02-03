/*************************************************************************
	> File Name: testpanduan1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月16日 星期二 14时17分17秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h> 



int main() {
    bool response;
    response = get_response(ASK);
    printf("\n response = %d\n", response);
    return 0;
}

bool get_response(char *ask) {
    printf("%s (y/n)?", ask);
    while (1) {
        switch(getchar()) {
            case 'Y':
            case 'y': return true; break;
            case 'N':
            case 'n':
            case 'EOF': return false;
        }
    }
}
