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
/*修改restored*/
/*加入 -r*/
void do_ls(const char[]);//执行ls基本操作
void dostat(char *);//获取文件信息
void show_file_info(char *, struct stat *);//展示从stat获取的信息
void mode_to_letters(int ,char[]);//将模式字段转化为字符
char * uid_to_name(uid_t);//将用户ID转化为字符串
char * gid_to_name(gid_t);//将组ID转化为字符串
void getArgs(int, char **);//获取命令行参数
void restored_ls(struct dirent *, const char *);//存储文件名
void color_print(char*,int);//染色
void show_without_l_with_r(char **);
void show_without_l(char **);//整合重复函数
void do_R(char *);//递归实现
void take_out(char *,char *);

ino_t get_inode(const char *);
mode_t get_mode(const char *);
unsigned long get_st_blocks(char *);

/*快排 排序字符串*/
void swap(char** s1,char** s2);
int compare(char* s1,char* s2);
int partition(char** filenames,int start,int end);
void sort(char** filenames,int start,int end);
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

char **filename;
int filenums = 0;
char **dirname;
int dirnums = 0;
mode_t *filetime;
mode_t temp_time;

int main(int argc, char *argv[])
{
    filename = (char **)malloc(sizeof(char*) * 20000);
    dirname = (char **)malloc(sizeof(char*) * 20000);
    for(int i = 0;i < 20000;i++){
        filename[i] = (char*)malloc(256);
        dirname[i] = (char*)malloc(256);
    }
    filetime = (mode_t *)malloc(sizeof(mode_t) * 20000);
    
    getArgs(argc, argv);
    if(dirnums == 0){
        dirname[dirnums++] = ".";
    }

    sort(dirname, 0, dirnums-1);

    if(has_R){
        do_R(dirname[0]);
    }else{
        if(has_r == 0){
            for(int i = 0; i < dirnums; i++){
                do_ls(dirname[i]);
            }
        }else if(has_r == 1){
            for(int i = dirnums-1; i >= 0; i--){
                do_ls(dirname[i]);
            }
        }
    }

    free(filename);
    free(dirname);
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
            dirname[dirnums++] = *argv;
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
            restored_ls(direntp,dirname);
        }

        if(has_t == 0){
            sort(filename,0,filenums-1);
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

        // for(int i = 0;i < filenums;i++){
        //     free(filename[i]);
        // }

        int flag;
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
    printf("%s ", modestr);//权限字符串
    printf("%d ",(int)info_p -> st_nlink);//链接数
    printf("%-8s",uid_to_name(info_p -> st_uid));//所属用户
    printf("%-8s",gid_to_name(info_p -> st_gid));//所属用户组
    printf("%8ld ",(long)info_p -> st_size);//内存大小
    printf("%.12s ",4+ctime(&info_p -> st_mtime));//最后修改时间
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

void restored_ls(struct dirent* dir,const char* dirname)
{
    char *result = dir -> d_name;
    if(has_a == 0 && *result == '.'){
        return;
    }
    filename[filenums++] = dir -> d_name;
}

void sort(char ** filenames,int start,int end)
{
    if(start < end){
        int position = partition(filename, start, end);
        sort(filename, start, position - 1);
        sort(filename, position + 1, end);
    }
}

void swap(char **s1, char **s2)
{
    char *temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}

int compare(char *s1, char *s2)
{
    while(*s1 && *s2 && *s1 == *s2){
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

int partition(char** filename,int start,int end)
{
	if(!filename){
        return -1;
    }
	char* privot = filename[start];
	while(start < end){
		while(start < end && compare(privot,filename[end]) < 0)
			--end;
		swap(&filename[start],&filename[end]);
		while(start < end && compare(privot,filename[start]) >= 0)
			++start;
		swap(&filename[start],&filename[end]);
	}
	return start;
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

void do_R(char *name)
{
    DIR *dir;
    struct dirent *ptr;
    int i, count = 0;
    int f_maxlen = 0;
    struct stat buf;
    char name_dir[10000];

    if (chdir(name) < 0){ //将输入的目录改为当前目录
        perror("chdir");
    }
    if (getcwd(name_dir, 10000) < 0)
    {
        perror("getcwd"); //获取当前目录的绝对路径
    }
    printf("%s:\n", name_dir);

    if ((dir = opendir(name_dir)) == NULL)//用新获得路径打开目录
    {
        perror("opendir");
    }
    while ((ptr = readdir(dir)) != NULL)
    {
        count++;
    }
    int flag_close = 0;
    if((flag_close = closedir(dir)) == -1){
        perror("closedir");
    }

    //动态数组
    char **filenames = (char **)malloc(count * sizeof(char *)); //要进行初始化
    memset(filenames, 0, sizeof(char *) * count);

    for (i = 0; i < count; i++)
    {
        filenames[i] = (char *)malloc(256 * sizeof(char));
        memset(filenames[i], 0, sizeof(char) * 256);
    }

    dir = opendir(name_dir);
    for (i = 0; i < count; i++)
    {
        if ((ptr = readdir(dir)) == NULL){
            perror("readdir");
        }

        strcat(filenames[i], ptr->d_name);
    }
    for (i = 0; i < count; i++){
        do_ls(filenames[i]);
    }
    printf("\n");

    for (i = 0; i < count; i++)
    {
        if (lstat(filenames[i], &buf) == -1){
            perror("stat");
        }
        if (strcmp(filenames[i], "..") == 0){
            continue;
        }
        if (strcmp(filenames[i], ".") == 0){
            continue;
        }

        if (S_ISDIR(buf.st_mode))//是目录
        {
            do_ls(filenames[i]);
        }
        else if (!S_ISDIR(buf.st_mode))//不是目录
        {
            mode_t file_mode = get_mode(filename[i]);
            color_print(filename[i], file_mode);
        }
        chdir("../"); //处理完一个目录后返回上一层
    }

    for (i = 0; i < count; i++)
    {
        free(filenames[i]);
    }
    free(filenames);
    
    if((flag_close = closedir(dir)) == -1){
        perror("closedir");
    }
}
