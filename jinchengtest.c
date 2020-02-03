/*************************************************************************
	> File Name: jinchengtest.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月04日 星期四 18时06分09秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_N 1000000

char sum_file[] = "./sum.txt";
char now_file[] = "./now.txt";

void  get_num(char *filename, int *num);

void set_num(char *filename, int num);

int count = 0;
int sum = 0;

int main() {
    int x = 0;
    set_num(sum_file, 0);
    set_num(now_file, 0);
    pid_t pid;
    for (int i = 0; i < 10; i++) {
        pid = fork();
        if (pid == -1) {
            printf("error!\n");
        } else if (pid == 0) {
            break;
        }
    }
    if (pid == 0) {
        while (1) {
            int tmp_n = 0;
            int tmp_s = 0;
            get_num(now_file, &tmp_n);
            get_num(sum_file, &tmp_s);
            if (tmp_n > MAX_N) break;
            tmp_s += tmp_n;
            set_num(now_file, tmp_n + 1);
            set_num(sum_file, tmp_s);
        }
        exit(0);
    }
    
    int n = 0;
    while (n < 10) {
        waitpid(-1, NULL, 0);
        n++;
    }
        
    get_num(sum_file, &sum);
    printf("%d\n", sum);
    return 0;
}

void get_num(char *filename, int *num) {
    FILE *out = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    if ((out = fopen(filename, "r")) < 0) {
        printf("open error!\n");
        exit(1);
    }

    getline(&line, &len, out);
    *num = atoi(line);
    free(line);
    fclose(out);
    return ;
}

void set_num(char *filename, int num) {
    FILE *out = NULL;
    if ((out = fopen(filename, "w")) < 0) {
        printf("open error!\n");
        exit(1);
    }
    fprintf(out, "%d", num);
    fclose(out);
    return ;
}






