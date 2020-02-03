/*************************************************************************
	> File Name: linux4.14.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年04月14日 星期日 21时32分39秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h> //getpwuid的头文件
#include <unistd.h> // getuid,gethostname,getcwd的头文件
/*void my_cd(const char *cd_to) {
    if(chdir(cd_to) == -1){
        perror("chdir");
        exit(1);
    }
}
*/
int main(){
    struct passwd *pwd; // passwd结构体描述了/etc/passwd的文件记录行 包括用户名 密码 用户 用户组ID 真实姓名 用户主目录 和默认Shell 
    pwd = getpwuid(getuid()); 
    // 根据传入的用户ID返回指向passwd的结构体 该结构体初始化了里面的所有成员
    // getuid()用来取得执行目前进程的用户识别码。
    char hostname[200];
    char workdir[1000];
    gethostname(hostname, 200);
    getcwd(workdir, 1000);
    printf("\033[1;35m%s@%s\033[0m:\033[1;36m%s\033[0m ", pwd->pw_name, hostname, workdir); 
    char dir[1000];
    char commands[256];
    while (scanf("%s", commands) != EOF) {
        if (strcmp(commands, "cd") == 0) {
            scanf("%s", &dir);
            if (chdir(dir) == -1) {
                perror("chdir");
                exit(1);
            }
            getcwd(dir, 1000);
            printf("\033[1;35m%s@%s\033[0m:\033[1;36m%s\033[0m ", pwd->pw_name, hostname, dir);
        } else if (strcmp(commands, "exit") == 0) {
            break;
        } else {
            printf("error command\n");
            printf("\033[1;35m%s@%s\033[0m:\033[1;36m%s\033[0m ", pwd->pw_name, hostname, workdir); 
        } 
    }
    return 0;
}
