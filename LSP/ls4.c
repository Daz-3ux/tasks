#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<string.h>
#include<unistd.h>

void do_ls(char[]);//执行ls基本操作
void dostat(char *);//获取文件信息
void show_file_info(char *, struct stat *);//展示从stat获取的信息
void mode_to_letters(int ,char[]);//将模式字段转化为字符
char * uid_to_name(uid_t);//将用户ID转化为字符串
char * gid_to_name(gid_t);//将组ID转化为字符串
void getArgs(int argc,char *argv[]);//获取命令行参数
void restored_ls(struct dirent *);//存储文件名
/*快排 排序字符串*/
void swap(char** s1,char** s2);
int compare(char* s1,char* s2);
int partition(char** filenames,int start,int end);
void sort(char** filenames,int start,int end);

int file_cnt = 0;//ls命令四个一行
int has_l = 0;
int has_a = 0;

char *filename[4096];
int filenums = 0;


int main(int argc, char *argv[])
{
    getArgs(argc,argv);
    if(argc == 1){
        do_ls(".");
    }else{
        if(has_a == 1){
            --argc;
            ++argv;
        }
        if(has_l == 1){
            --argc;
            ++argv;
        }
        if(argc == 1){
            do_ls(".");
        }else{
            while(--argc){
                //printf("%s:\n", * ++argv);
                do_ls(*++argv);
            }
        }
    }


    return 0;
}

void do_ls(char dirname[])
//list files in directory called dirname
{
    DIR *dir_ptr;
    struct dirent *direntp;
    
    if((dir_ptr = opendir(dirname)) == NULL){
        perror("打开目录失败");
        exit(0);
    }else{

        while((direntp = readdir(dir_ptr)) != NULL){
            restored_ls(direntp);
        }
        sort(filename,0,filenums-1);
        
        if(has_l == 1){
            for(int i = 0; i < filenums; i++){
                //filename[filenums++] = dir -> d_name;
                dostat(filename[i]);
            }
        }else{
            for (int i = 0; i < filenums; i++){
                printf("%-10s", filename[i]);
                file_cnt++;
                if (file_cnt % 5 == 0){
                    printf("\n");
                }
            }
        }

        // for(int i = 0;i < filenums;i++){
        //     free(filename[i]);
        // }

        int flag;
        if((flag = closedir(dir_ptr)) == -1){
            perror("关闭目录失败");
            exit(0);
        }
    }
}

void dostat(char *filename)
{
    struct stat info;//存放stat结构体数据的地址
    if(stat(filename, &info) == -1){//解析filename，将得到的信息放在info
        perror("获取信息失败");
    }else{
        show_file_info(filename,&info);//调用下一函数
    }
}

void show_file_info(char *filename, struct stat *info_p)//此处的info_p就是stat放东西的info位
//display the info about filename
{
    char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letters();
    char modestr[11];//一类型位+九权限位+'\0'

    mode_to_letters(info_p -> st_mode, modestr);

    printf("%s ", modestr);//权限字符串
    printf("%d ",(int)info_p -> st_nlink);//链接数
    printf("%-8s",uid_to_name(info_p -> st_uid));//所属用户
    printf("%-8s",gid_to_name(info_p -> st_gid));//所属用户组
    printf("%8ld ",(long)info_p -> st_size);//内存大小
    printf("%.12s ",4+ctime(&info_p -> st_mtime));//最后修改时间
    printf("%s\n",filename);//文件名

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

void getArgs(int argc, char *argv[])
{
    int opt;
    if((opt = getopt(argc, argv,"la")) != -1){//选项字母后面带一个冒号表示这个选项带有一个参数
        switch(opt){
            case 'a':
                has_a = 1;
                break;
            case 'l':
                has_l = 1;
                break;
            case ':':
                perror("Missing argument");
                exit(0);
                break;
            case '?':
                perror("Unrecognized option");
                exit(0);
                break;
            default:
                printf("ERROR ARGV");
                exit(EXIT_FAILURE);
        }
    }

    //if(optind >= argc){
    //optind:The variable optind is the index of the next element to be processed in argv
    //     perror("参数过少");
    //     exit(EXIT_FAILURE);
    //}
}

void restored_ls(struct dirent* dir)
{
    char *result = dir -> d_name;
    if(has_a == 0 && *result == '.'){
        return;
    }
    filename[filenums++] = dir -> d_name;
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

int partition(char** filenames,int start,int end)
{
	if(!filenames){
        return -1;
    }
	char* privot = filenames[start];
	while(start < end){
		while(start < end && compare(privot,filenames[end]) < 0)
			--end;
		swap(&filenames[start],&filenames[end]);
		while(start < end && compare(privot,filenames[start]) >= 0)
			++start;
		swap(&filenames[start],&filenames[end]);
	}
	return start;
}

void sort(char ** filenames,int start,int end)
{
    if(start < end){
        int position = partition(filename, start, end);
        sort(filename, start, position - 1);
        sort(filename, position + 1, end);
    }
}