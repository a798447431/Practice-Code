/*************************************************************************
	> File Name: jinchengtest.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月04日 星期四 18时06分09秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>
#define MAX_N 10000

char sum_file[] = "./sum.txt";
char now_file[] = "./now.txt";

void get_num(char *filename, int *num, FILE *fp);
void set_num(char *filename, int num, FILE *fp);

int main() {
    FILE *fp_num, *fp_sum, *fp_lock;
    int num = 0, sum = 0;
    set_num(sum_file, sum, fp_sum);
    set_num(now_file, num, fp_num);
    fclose(fp_sum);
    fclose(fp_num);
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
            fp_num = fopen(now_file, "r");
            fp_sum = fopen(sum_file, "r");
            fp_lock = fopen(sum_file, "r");
            flock(fp_lock->_fileno, LOCK_EX);
            get_num(now_file, &num, fp_num);
            get_num(sum_file, &sum, fp_sum);
            while (sum >= MAX_N) break;
            sum += num;
            fp_num = fopen(now_file, "w");
            set_num(now_file, num + 1, fp_num);
            fp_sum = fopen(sum_file, "w");
            set_num(sum_file, sum, fp_sum);
            fclose(fp_num);
            fclose(fp_sum);
            fclose(fp_lock);
            printf("%d\n", sum);
        }
        exit(0);
    }
    int n = 0;
    while (n < 10) {
        waitpid(-1, NULL, 0);
        n++;
    }

    return 0;
}

void get_num(char *filename, int *num, FILE *fp) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    getline(&line, &len, fp);
    *num = atoi(line);
    free(line);
    return ;
}

void set_num(char *filename, int num, FILE *fp) {
    fprintf(fp, "%d", num);
    return ;
}






