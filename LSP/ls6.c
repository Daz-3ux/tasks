/*
内存泄露检测：
1. 
gcc -fsanitize=address -g ls8.c -o ls8
2.
gcc -g ls8.c -o ls8
valgrind --tool=memcheck --leak-check=full --show-reachable=yes -s ./ls8
*/

#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<limits.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<assert.h>
/*修改restored*/
/*加入 -r*/
void do_ls(const char[]);//执行ls基本操作
void dostat(char *);//获取文件信息
void show_file_info(char *, struct stat *);//展示从stat获取的信息
void mode_to_letters(int ,char[]);//将模式字段转化为字符
char * uid_to_name(uid_t);//将用户ID转化为字符串
char * gid_to_name(gid_t);//将组ID转化为字符串
void getArgs(int, char **);//获取命令行参数
void restored_ls(struct dirent *);//存储文件名
void color_print(char*,int);//染色
void show_without_l_with_r(char **);
void show_without_l(char **);//整合重复函数
void do_R(char *);//递归实现
void take_out(char *,char *);

ino_t get_inode(const char *);
mode_t get_mode(const char *);
unsigned long get_st_blocks(char *);

void sort(char** filenames,int filenums);
/*按照最后修改时间排序*/
void swap_M(char *s1,char *s2);
void sort_by_ModificationTime(char **, int);


int file_cnt = 0;
/*定义来自man手册*/
int has_l = 0;//use a long listing format
int has_a = 0;//do not ignore entries starting with .
int has_i = 0;//print the index number of each file
int has_r = 0;//reverse older while sorting
int has_t = 0;//sort by modification time,newest first
int has_s = 0;//print the allocated size of each file, in blocks
int has_R = 0;//list subdirectories recursively
int flag = 1;
int flag_have_dir = 0;

char *filename[4096];
int filenums = 0;
char *dirname[2];
mode_t *filetime;
mode_t temp_time;

int cmp(const void *s1, const void *s2)
{
    //(s1 && s2);
    return strcmp(*(char**)s1, *(char**)s2);
}

int main(int argc, char *argv[])
{
    filetime = (mode_t *)malloc(sizeof(mode_t) * 20000);
    
    getArgs(argc, argv);

    if(argc == 1){
        do_ls(".");
    }else{
        if(!has_R && !flag_have_dir){
            do_ls(".");
        }else if(!has_R && flag_have_dir){
            do_ls(dirname[0]);
        }else if(has_R && !flag_have_dir){
            do_R(".");
        }else if(has_R && flag_have_dir){
            do_R(dirname[0]);
        }
    }

    free(filetime);
    return 0;
}


void getArgs(int argc, char *argv[]){
    while(--argc){
        ++argv;
        if(*argv[0] == '-'){
            while(*(++(*argv))){
                if(**argv == 'a')   has_a = 1;
                if(**argv == 'l')   has_l = 1;
                if(**argv == 'i')   has_i = 1;
                if(**argv == 'r')   has_r = 1;
                if(**argv == 't')   has_t = 1;
                if(**argv == 'r')   has_r = 1;
                if(**argv == 's')   has_s = 1;
                if(**argv == 'R')   has_R = 1;
            }
        }else{
            flag_have_dir = 1;
            dirname[0] = *argv;
        }
    }
}

void do_ls(const char dirname[])
//list files in directory called dirname
{
    DIR *dir_ptr;
    struct dirent *direntp;

    if((dir_ptr = opendir(dirname)) == NULL){
        perror("打开目录失败");
        exit(1);
    }else{
        while((direntp = readdir(dir_ptr)) != NULL){
            restored_ls(direntp);
        }

        if(has_t == 0){
            qsort(filename,filenums,sizeof(filename[0]),cmp);
        }else if(has_t == 1){
            sort_by_ModificationTime(filename,filenums);
        }
        
        if(has_l){
            if(has_r == 1){
                for(int i = filenums-1; i >= 0; i--){
                    dostat(filename[i]);
                }
            }else {
                for(int i = 0; i < filenums; i++){
                    //filename[filenums++] = dir -> d_name;
                    dostat(filename[i]);
                }
            }
        }else{
            if (has_r == 0){
                show_without_l(filename);
            }else if (has_r == 1){
                show_without_l_with_r(filename);
            }
        }

        if((flag = closedir(dir_ptr)) == -1){
            perror("关闭目录失败");
            exit(1);
        }
    }
}

void show_without_l(char** filename)
{
    for (int i = 0; i < filenums; i++){
        if (has_i && !has_s){
            ino_t file_ino = get_inode(filename[i]);
            mode_t file_mode = get_mode(filename[i]);
            printf("%-5ld", file_ino);
            color_print(filename[i], file_mode);
            file_cnt++;
                if (file_cnt % 2 == 0){
                    printf("\n");
                }
            }else if(!has_i && has_s){
                unsigned long file_blo = get_st_blocks(filename[i]);
                mode_t file_mode = get_mode(filename[i]);
                printf("%-5ld", file_blo);
                color_print(filename[i], file_mode);
                file_cnt++;
                    if (file_cnt % 2 == 0){
                        printf("\n");
                    }
            }else if(has_i && has_s){
                ino_t file_ino = get_inode(filename[i]);
                mode_t file_mode = get_mode(filename[i]);
                unsigned long blo = get_st_blocks(filename[i]);
                printf("%-5ld", file_ino);
                printf("%-5ld", blo);
                color_print(filename[i], file_mode);
                file_cnt++;
                if (file_cnt % 2 == 0){
                    printf("\n");
                }
            }else{
                mode_t file_mode = get_mode(filename[i]);
                color_print(filename[i], file_mode);
                file_cnt++;
                if (file_cnt % 5 == 0){
                    printf("\n");
                }
            }
        }
}

void show_without_l_with_r(char ** filename)
{
    for (int i = filenums-1; i >= 0; i--){
        if (has_i && !has_s){
            ino_t file_ino = get_inode(filename[i]);
            mode_t file_mode = get_mode(filename[i]);
            printf("%-5ld", file_ino);
            color_print(filename[i], file_mode);
            file_cnt++;
                if (file_cnt % 2 == 0){
                    printf("\n");
                }
            }else if(!has_i && has_s){
                unsigned long file_blo = get_st_blocks(filename[i]);
                mode_t file_mode = get_mode(filename[i]);
                printf("%-5ld", file_blo);
                color_print(filename[i], file_mode);
                file_cnt++;
                    if (file_cnt % 2 == 0){
                        printf("\n");
                    }
            }else if(has_i && has_s){
                ino_t file_ino = get_inode(filename[i]);
                mode_t file_mode = get_mode(filename[i]);
                unsigned long blo = get_st_blocks(filename[i]);
                printf("%-5ld", file_ino);
                printf("%-5ld", blo);
                color_print(filename[i], file_mode);
                file_cnt++;
                if (file_cnt % 2 == 0){
                    printf("\n");
                }
            }else{
                mode_t file_mode = get_mode(filename[i]);
                color_print(filename[i], file_mode);
                file_cnt++;
                if (file_cnt % 5 == 0){
                    printf("\n");
                }
            }
        }
}

ino_t get_inode(const char *this_name)
{
    struct stat info;
    while(stat (this_name, &info) == -1){
        chdir("..");
        if(get_inode(".") == get_inode("..")){
            break;
        }
    }

    return info.st_ino;
}

mode_t get_mode(const char *this_name)
{
    struct stat info;
    while(stat (this_name, &info) == -1){
        chdir("..");
        if(get_inode(".") == get_inode("..")){
            break;
        }
    }

    return info.st_mode;
}

unsigned long get_st_blocks(char *this_name)
{
    struct stat info;
    while(stat (this_name, &info) == -1){
        chdir("..");
        if(get_inode(".") == get_inode("..")){
            break;
        }
    }

    return info.st_blocks;
}

void dostat(char *filename)
{
    struct stat info;//存放stat结构体数据的地址
    while(stat(filename, &info) == -1){//解析filename，将得到的信息放在info
        chdir("..");
        if(get_inode(".") == get_inode("..")){
            break;
        }
        //perror("在dostat获取信息失败");
    }

    show_file_info(filename,&info);//调用下一函数
    
}

void show_file_info(char *filename, struct stat *info_p)//此处的info_p就是stat放东西的info位
{
    char *ctime();
    char modestr[11];//一类型位+九权限位+'\0'

    mode_to_letters(info_p -> st_mode, modestr);

    if(has_i == 1){
        printf("%-5ld",info_p -> st_ino);
    }
    if(has_s == 1){
        printf("%-5ld",info_p -> st_blocks);
    }
    printf("%s  ", modestr);//权限字符串
    printf("%d  ",(int)info_p -> st_nlink);//链接数
    printf("%-8s",uid_to_name(info_p -> st_uid));//所属用户
    printf("%-8s",gid_to_name(info_p -> st_gid));//所属用户组
    printf("%8ld    ",(long)info_p -> st_size);//内存大小
    printf("%.12s   ",4+ctime(&info_p -> st_mtime));//最后修改时间
    //printf("%s\n",filename);//文件名
    color_print(filename, info_p -> st_mode);
    putchar('\n');
}

#define S_ISEXEC(m) (((m)&(0000111))!=(0))//检测是否为可执行文件
void color_print(char *filename,int filemode)
{
    if(S_ISDIR(filemode)){
        printf("\033[01;34m%-10s\033[0m", filename);
    }else if(S_ISLNK(filemode)){
        printf("\033[01;36m%-10s\033[0m", filename);
	}
	else if(S_ISFIFO(filemode)){
		printf("\033[40;33m%-10s\033[0m", filename);
	}
	else if(S_ISSOCK(filemode)){
		printf("\033[01;35m%-10s\033[0m", filename);
	}
	else if(S_ISBLK(filemode)){
		printf("\033[40;33;01m%-10s\033[0m", filename);
	}
	else if(S_ISCHR(filemode)){
		printf("\033[40;33;01m%-10s\033[0m", filename);
	}
	else if(S_ISREG(filemode)){
		if(S_ISEXEC(filemode)){
			printf("\033[01;32m%-10s\033[0m", filename);
		}
		else{
			printf("%-10s", filename);
		}
	}
	else{
		printf("%-10s", filename);
	}
}

void mode_to_letters(int mode,char str[])
{
    strcpy(str,"----------");//十个 default = no permission

    /*掩码*/

    //1bit for types
    if(S_ISDIR(mode)) str[0] = 'd';//directory
    if(S_ISCHR(mode)) str[0] = 'c';//char device
    if(S_ISBLK(mode)) str[0] = 'b';//block device
    //三者都不是，那就显示默认的'-'

    //3bit for user
    if(mode & S_IRUSR) str[1] = 'r';
    if(mode & S_IWUSR) str[2] = 'w';
    if(mode & S_IXUSR) str[3] = 'x';

    //3bit for group
    if(mode & S_IRGRP) str[4] = 'r';
    if(mode & S_IWGRP) str[5] = 'w';
    if(mode & S_IXGRP) str[6] = 'x';

    //3bit for other
    if(mode & S_IROTH) str[7] = 'r';
    if(mode & S_IWOTH) str[8] = 'w';
    if(mode & S_IXOTH) str[9] = 'x';

}

//#include<pwd.h>
char * uid_to_name(uid_t uid)//此处的uid值即为stat函数所读到的st_uid值
{
    struct passwd *getpwuid();
    struct passwd *pw_ptr;
    static char numstr[10];

    if((pw_ptr = getpwuid(uid)) == NULL){
        sprintf(numstr, "%d", uid);
        return numstr;
    }else{
        return pw_ptr -> pw_name;
    }
}

//#include<grp.h>
char * gid_to_name(gid_t gid)//此处的gid值即为stat函数所读到的st_gid值
{
    struct group *getgrgid();
    struct group *grp_ptr;
    static char numstr[10];

    if((grp_ptr = getgrgid(gid)) == NULL){
        sprintf(numstr, "%d", gid);
        return numstr;
    }else{
        return grp_ptr -> gr_name;
    }
}

void restored_ls(struct dirent* dir)
{
    char *result = dir -> d_name;
    if(has_a == 0 && *result == '.'){
        return;
    }
    filename[filenums++] = dir -> d_name;
}

void sort_by_ModificationTime(char **filename, int nums)
{
    if(!filename){
        perror("文件名错误");
        exit(1);
    }
    struct stat info;
    for(int i=0; i<nums; i++){
        while(stat(filename[i],&info) == -1){
            //perror("在sort_BY_M执行stat失败");
            chdir("..");
        }
        filetime[i] = info.st_mtime;
    }
    for(int i = 0; i < nums-1; i++){
        for(int j = 0; j < nums-1-i; j++){
            if(filetime[j] < filetime[j+1]){
                temp_time = filetime[j];
                filetime[j] = filetime[j+1];
                filetime[j+1] = temp_time;//时间交换
                char *temp = (char*)malloc(sizeof(char) * 4096);
                strcpy(temp,filename[j]);  
                strcpy(filename[j],filename[j+1]);
                strcpy(filename[j+1],temp);
                free(temp);
            }
        }
    }
}

void do_R(char path[])
{
    printf("%s:\n", path);
    DIR *dir_ptr;
    struct dirent *direntp;
    if ((dir_ptr = opendir(path)) == NULL) //打开目录
        fprintf(stderr, "cannot open %s\n", path);
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL) //读取当前目录文件
        {
            restored_ls(direntp);
        }
        qsort(filename,filenums,sizeof(filename[0]),cmp);

        for (int j = 0; j < filenums; j++)
        {
            struct stat info;
            if (stat(path, &info) == -1){
                perror(path);
            }
            color_print(filename[j], info.st_mode);
        }
    }

    printf("\n");
    if((flag = closedir(dir_ptr)) == -1){
        perror("fault:closedir in R");
    }

    if ((dir_ptr = opendir(path)) == NULL)//目录递归
        fprintf(stderr, "cannot open %s\n", path);
    else{
        while ((direntp = readdir(dir_ptr)) != NULL){
            if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
                continue;
            struct stat info;
            char temp[4096];
            sprintf(temp, "%s/%s", path, direntp->d_name);
            if (stat(temp, &info) == -1){
                perror("cannot stst in recursion");
            }
            if (S_ISDIR(info.st_mode)){ //判断是否为目录，如果是目录就进入递归
                do_R(temp);
            }
        }
    }
}