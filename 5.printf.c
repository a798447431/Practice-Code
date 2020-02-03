/*************************************************************************
	> File Name: 5.printf.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年04月30日 星期二 18时13分02秒
 ************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#define printf(frm, args...) ;

int output_dnum(int p, int digit2) {
    int flag = (x >= 0 ? 0 : 1); 
    int temp = 0;
    if (x < 0) {
        temp = -x;
    } else {
        temp = x;
    }    
    int digit = 0;
    if (flag) {
        putchar('-');
    }
    do {
        temp = temp * 10 + x % 10;
        digit += 1;
        x /= 10;
    } while(x);
    if (digit != -1) digit = digit2;
    while (digit--) {
        putchar(temp % 10 ^ 48);
        temp /= 10;
    }
}


int output_d(int x) {
    #define BASE 100000
    #define BASE_D 5
    unsigned int temp = (x >= 0 ? x : -x);
    int digit = floor(log10(temp)) + 1;
    int p1 = x / BASE. p2 = x % BASE;
    if (digit <= 5) {
        output_dnum(p2, -1);
    } else {
        output_dnum(p1, -1);
        if (p2 < 0) p2 = -p2;
        output_dnum(p2, BASE_D);
    }
    #undef BASE
    return 0;
}

int my_printf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    int ret = 0;
                        unsigned int temp2 = 0; 
                        int flag = (x >= 0 ? 0 : 1);
                        if (x < 0) {
                            temp1 = -x;
                        } else {
                            temp1 = x;
                        }
                        int digit = 0;
                        if (flag) {
                            putchar('-');
                        }
                        do {
                            temp2 = temp2 * 10 + temp1 % 10;
                            digit += 1;
                            temp1 /= 10;
                        } while(temp1); 
                        while (digit--) {
                            putchar(temp2 % 10 ^ 48);
                            temp2 /= 10;
                        }
                    } break;
                }
            } break;
            default:
                putchar(str[0]), ret++; break;
        }
        str++;
    }
    return ret;

}

int main() {
    my_printf("hello world : %d\n", 146548644);
    my_printf("hello world : %d\n", 100000);
    my_printf("hello world : %d\n", -5);
    my_printf("hello world : %d\n", 0);
    return 0;
}
