/*************************************************************************
	> File Name: play_again1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月20日 星期六 09时50分58秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h>

#define ASK "Do you want another try"

bool get_reponse(char *);

int main() {
    bool response;
    response = get_reponse(ASK);
    printf("\n response = %d\n", response);
    return 0;
}


bool get_reponse(char *ask) {
    printf("%s (y/n)?", ask);
    while (1) {
        switch(getchar()) {
            case 'Y':
            case 'y': return true;
            case 'N': 
            case 'n':
            case EOF: return false;
        }
    }
}
