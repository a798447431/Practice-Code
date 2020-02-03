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

#define MAX_SIZE 20000
#define MAX_LENGTH 1000

int is_dir(char *name) {
    DIR *dir;
    if((dir = opendir(name)) == NULL) 
        return 0;
    else 
    {
        closedir(dir); 
        return 1;
    }
}
/*
int is_dir(char *name) {
    struct stat *buf = malloc(sizeof(struct stat));
    if (stat(name, buf) == -1) {
        perror("stat");
        return -1;
    }
    if (S_IFDIR & buf->st_mode) {
        return ISDIR;
    }
    else {
        return ISFILE;
    }
}
*/

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
        printf("error: %s 文件创建失败!\n", filename2);
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
        strcat(temp_dirname, split);
    }
    strcat(temp_dirname, temp_filename);
    return cp_file_to_file(filename, temp_dirname);
}

//创建目录这块没有思路，这个函数是在网上找到看懂后改的
//不要直接向strtok函数传入路径，strtok会永久破坏传入的字符串
int creat_dir(char *pathname) {
    const char *split = "/"; 
    char temp_pathname[MAX_LENGTH];
    sprintf(temp_pathname, "%s", pathname);
	char *temp; 
    //使用strtok函数对路径的每一个目录进行分割
    temp = strtok(temp_pathname, split);
	char dirname[MAX_SIZE];
	while(temp != NULL) {
		sprintf(dirname, "%s%s/", dirname, temp); //把每一个分割后的目录名添加到dirname后面
		if(access(dirname, F_OK) < 0) {//判断目录是否存在，没有则使用mkdir函数进行创建
            if(mkdir(dirname, 0755) < 0) {
			    perror("mkdir");
			    return -1;
		    }
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
        temp_start_dirname[0] = '\0';
        temp_finish_dirname[0] = '\0';
	    //bzero(temp_finish_dirname, sizeof(temp_finish_dirname)); 也可以用bzero函数初始化字符数组    
        sprintf(temp_start_dirname,"%s/%s", start_dirname, dp->d_name);
		sprintf(temp_finish_dirname,"%s/%s", finish_dirname, dp->d_name);
        if (is_dir(temp_start_dirname)) {
            if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue; //屏蔽.和..;
            cp_dirfile_to_dir(temp_start_dirname, temp_finish_dirname);
        } 
    
        else {
            cp_file_to_file(temp_start_dirname, temp_finish_dirname);
        }
    }
    closedir(dir);
    return 0;
} 

//将stat_dir目录名追加到finish_dir后就可以将整个目录一起复制过去
int cp_dir_to_dir(char *start_dirname, char *finish_dirname) {
    char temp_array[MAX_LENGTH] = ""; 
    char temp_start_dirname[MAX_LENGTH] = "";
    char temp_finish_dirname[MAX_LENGTH] = "";
    sprintf(temp_array, "%s", start_dirname);
    const char *split = "/";
    char *p;
    p = strtok(temp_array, split);
    while (p != NULL) {
        temp_start_dirname[0] = '\0';
        sprintf(temp_start_dirname, "%s", p);
        p = strtok(NULL, split);
    }
    //拼接目录名和文件名
    strcpy(temp_finish_dirname, finish_dirname);
    //如果目录路径末尾没有/，则补个/与文件名进行拼接
    if(temp_finish_dirname[strlen(temp_finish_dirname) - 1] != '/') {
        strcat(temp_finish_dirname, split);
    }
    strcat(temp_finish_dirname, temp_start_dirname);
    return cp_dirfile_to_dir(start_dirname, temp_finish_dirname);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("error: 缺少参数！\n");
        return -1;
    }
    else if (argc == 3) {
        if(is_dir(argv[1]) && is_dir(argv[2])) {//指定目录->指定目录
            cp_dir_to_dir(argv[1], argv[2]);
        }
        else if(is_dir(argv[1]) && !is_dir(argv[2])) {//将当前目录内文件->新建文件夹中
            cp_dirfile_to_dir(argv[1], argv[2]); 
        }
        else if(is_dir(argv[2])) {
            cp_file_to_dir(argv[1], argv[2]);//指定文件->指定目录
        }
        else if(!is_dir(argv[2])) {
            cp_file_to_file(argv[1], argv[2]);//指定文件->指定文件
        }
    } else {//大于两个参数的情况，最后一个参数一定是目录，否则出错;
        if (!is_dir(argv[argc - 1])) {
                printf("error: 最后一个参数 %s 不是目录!\n", argv[argc - 1]);
        }
        for (int i = 1; i < argc - 1; i++) {
            if (is_dir(argv[i])) {
                cp_dir_to_dir(argv[i], argv[argc - 1]);
            } 
            else {
                cp_file_to_dir(argv[i], argv[argc - 1]);
            }
        }
    }
    return 0;
}
