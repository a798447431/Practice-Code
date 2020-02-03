/*************************************************************************
	> File Name: qiuhe_1.cpp
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月04日 星期四 19时35分47秒
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>

using namespace std;
char sum_file[] = "./sum.txt";
char now_file[] = "./now.txt";
void get_num(char *filename, long long *num, FILE *fp){
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, fp);
    *num = atoll(line);
    free(line);
}

int main() {
    int x = 0;
    long long num = 0;
    long long sum = 0;
    FILE *fp, *fp2;
    fp = fopen(now_file, "w");
    fp2 = fopen(sum_file, "w");
    fprintf(fp,"%d", num);
    fprintf(fp2,"%d", sum);
    fclose(fp);
    fclose(fp2);
    pid_t pid;
    for(int i = 1; i <= 10; i++) {
        pid = fork();
        if (pid == 0) {
            x = i;
            break;
        }
    }
    if (pid == 0) {
        FILE *fp3, *fp4, *fp_tmp;
        long long tnum, tsum;
        int cnt = 1000;
        while (cnt--) {
            fp_tmp = fopen(now_file, "r");
            flock(fp_tmp->_fileno, LOCK_EX);

            fp3 = fopen(now_file, "r");
            fp4 = fopen(sum_file, "r");
            get_num(now_file, &tnum, fp3);
            get_num(sum_file, &tsum, fp4);
            fclose(fp4);
            fclose(fp3);
            tnum ++;
            tsum += tnum;
            fp3 = fopen(now_file, "w");
            fp4 = fopen(sum_file, "w");
            fprintf(fp3,"%d", tnum);
            fprintf(fp4,"%d", tsum);
            //cout << "进程：" << x << " -> " << tsum << endl;   
            fclose(fp4);
            fclose(fp3);

            flock(fp_tmp->_fileno, LOCK_UN);
            fclose(fp_tmp);
        }
        cout << "进程：" << x << " -> " << tsum << endl;   
        exit(0);
    }
    int n = 0;
    while (n < 10) {
        waitpid(-1, NULL, 0);
        n++;
    }
    return 0;
}


