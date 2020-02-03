/*************************************************************************
	> File Name: test_cp.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月10日 星期五 09时40分56秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

#define ISDIR  1
#define ISFILE 2
#define MAX_SIZE 20000
#define MAX_LENGTH 1000

int is_file_or_dir(char *name) {
    struct stat *buf = malloc(sizeof(struct stat));
    if (stat(name, buf) == -1) {
        perror("stat");
        return -1;
    }
    if (S_IFDIR & buf->st_mode) {
       return ISDIR;
    }
    else if (S_IFREG & buf->st_mode) {
       return ISFILE; 
    }
}

int cp_file_to_file(char *filename1, char *filename2) {
    char buf[MAX_SIZE] = {0};
    int length;
    FILE *open = NULL, *out = NULL;
    
    if ((open = fopen(filename1, "r")) == NULL) {
        printf("error: %s 文件不存在!\n", filename1);
        return -1;
    }

    if ((out = fopen(filename2, "w")) == NULL) {
        fclose(open);
        printf("error: 文件创建失败!\n");
        return -2;
    }
  
    while ((length = fread(buf, 1, MAX_SIZE, open)) > 0) {
        fwrite(buf, 1, length, out);
    }

    fclose(open);
    fclose(out);
    return 0;
}

//根据下面创建目录函数的思路改写的文件复制到目录函数
int cp_file_to_dir(char *filename, char *dirname) {
    char temp_array[MAX_LENGTH] = "";
    char temp_filename[MAX_LENGTH] = "";
    char temp_dirname[MAX_LENGTH] = "";
    sprintf(temp_array, "%s", filename);
    const char *split = "/";
    char *p;
    p = strtok(temp_array, split);
    while (p != NULL) {
        temp_filename[0] = '\0';
        sprintf(temp_filename, "%s", p);
        p = strtok(NULL, split);
    }
    //拼接目录名和文件名
    strcpy(temp_dirname, dirname);
    //如果目录路径末尾没有/，则补个/与文件名进行拼接
    if(temp_dirname[strlen(temp_dirname) - 1] != '/') {
        strcat(temp_dirname, "/");
    }
    strcat(temp_dirname, temp_filename);
    int i = cp_file_to_file(filename, temp_dirname);
    return i;
}

//创建目录这块没有思路，这个函数是在网上找到看懂后改的
int creat_dir(char *pathname) {
    const char *split = "/"; 
	char *temp; 
    //使用strtok函数对路径的每一个目录进行分割
    temp = strtok(pathname, split);
	char dirname[MAX_SIZE];
	while(temp != NULL) {
		sprintf(dirname, "%s%s/", dirname, temp); //把每一个分割后的目录名添加到dirname后面
		if(access(dirname, F_OK) < 0) {//判断目录是否存在，没有则使用mkdir函数进行创建
            if(mkdir(dirname, 0755) < 0) {
			    perror("mkdir");
			    return -1;
		    }
        } else{
            printf("error: 目录已存在！");
            return -2;
        }
		temp = strtok(NULL, split);
	}
    return 0;
}

int cp_dirfile_to_dir(char *start_dirname, char *finish_dirname) {
    DIR *dir;
    struct dirent *dp;
    const char *split = "/"; 
    char temp_start_dirname[MAX_LENGTH];
    char temp_finish_dirname[MAX_LENGTH];
    dir = opendir(start_dirname);
    if (dir == NULL) {
        perror("opendir");
        return -1;
    }

    if(creat_dir(finish_dirname) != 0) {
        return -2;
    }
    
    while ((dp = readdir(dir)) != NULL) {
        strcpy(temp_start_dirname, start_dirname);
        strcat(temp_start_dirname, split);
        strcat(temp_start_dirname, dp->d_name);
        strcpy(temp_finish_dirname, finish_dirname);
        strcat(temp_finish_dirname, split);
        strcat(temp_finish_dirname, dp->d_name);
        
        if (is_file_or_dir(temp_start_dirname) == ISDIR) {
            if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
            cp_dirfile_to_dir(temp_start_dirname, temp_finish_dirname);
        } 
    
        else if (is_file_or_dir(temp_start_dirname) == ISFILE) {
            cp_file_to_file(temp_start_dirname, temp_finish_dirname);
        }
    }
    closedir(dir);
    return 0;
} 

int main(int argc, char *argv[]) {
    if (argc == 3) {
        cp_dirfile_to_dir(argv[1], argv[2]);
    }
 // creat_dir(argv[1]);
    return 0;
}
