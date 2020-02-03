/*************************************************************************
	> File Name: linux.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年04月14日 星期日 21时32分39秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h> 
#include <signal.h>  
#define MAX_PATH_LEN 1000
#define MIN_PATH_LEN 256

struct passwd *pwd; 

char hostname[MAX_PATH_LEN];
char commands[MIN_PATH_LEN];

char *strrpc(char *str,char *oldstr,char *newstr){
    char temp_str[strlen(str)];
    memset(temp_str,0,sizeof(temp_str));
 
    for(int i = 0;i < strlen(str);i++){
        if(!strncmp(str+i,oldstr,strlen(oldstr))){
            strcat(temp_str,newstr);
            i += strlen(oldstr) - 1;
        }else{
        	strncat(temp_str, str + i, 1);
	    }
    }

    strcpy(str,temp_str);
    return str;
}

void my_pwd(char *workdir){ 
    gethostname(hostname, MAX_PATH_LEN);
    getcwd(workdir, MAX_PATH_LEN);
    workdir = strrpc(workdir, pwd->pw_dir, "~");
    printf("\033[1;35m%s@%s\033[0m:\033[1;36m%s\033[0m ", pwd->pw_name, hostname, workdir); 
}

void my_cd(char *dir) {
    if (chdir(dir) == -1) {
        printf("\033[1;31mmy_cd: no this directery!\033[0m\n");
    }
}


int main(){
    pwd = getpwuid(getuid());
    signal(SIGINT, SIG_IGN);
    char workdir[MAX_PATH_LEN];
    char dir[MAX_PATH_LEN];
    my_pwd(workdir);
    while (scanf("%s", commands) != EOF) {
        if (strcmp(commands, "cd") == 0) {
            scanf("%s", &dir);
            my_cd(dir);
            my_pwd(dir);
        } else if (strcmp(commands, "exit") == 0) {
            break;
        } else {
            printf("\033[1;31mcommand not found!\033[0m\n");
            my_pwd(workdir);
        }
    }
    return 0;
}
