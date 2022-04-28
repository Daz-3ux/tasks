/*
获取当前目录inode号
在上一级目录通过此inode号获取当前目录文件名
递归
当满足终止条件或发生错误时程序运行结束
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

ino_t	get_inode(char *);
void    printpathto(ino_t);
void    inum_to_name(ino_t , char *, int );

int main()
{
	printpathto(get_inode( "." ));//打印路径
	putchar('\n');//换行
	return 0;
}

void printpathto(ino_t this_inode)
//利用inode号去递归，保存每一次记录到的文件夹名，从而得到路径
{
	ino_t	my_inode;
	char	its_name[BUFSIZ];

	if (get_inode("..") != this_inode)//this_inode的初始值参见12行
	{
		chdir("..");//利用chdir()函数跳转至上一个目录

		inum_to_name(this_inode,its_name,BUFSIZ);//利用inode号获取当前目录名称

		my_inode = get_inode( "." );//获取上一级目录inode号，更新my_node
		printpathto( my_inode );//递归
		printf("/%s", its_name );//递归至顶层，所以打印从初始目录的根目录开始，符合逻辑
	}
}

void inum_to_name(ino_t inode_to_find , char *namebuf, int buflen)
/*
在当前文件夹中找到inode号与记录值this_inode匹配的文件夹
其就是下一级目录
保存其名字到namebuf数组
*/
{
	DIR	*dir_ptr;//DIR结构体
	struct dirent *direntp;//dirent结构体

	dir_ptr = opendir(".");
	if (dir_ptr == NULL){//处理失败情况
		perror(".");
		exit(1);
	}

    //在文件夹中搜索指定的inode号
	while ((direntp = readdir(dir_ptr)) != NULL){
		if ( direntp->d_ino == inode_to_find ){
			strncpy( namebuf, direntp->d_name, buflen);
			namebuf[buflen-1] = '\0';   /* just in case */
			closedir( dir_ptr );
			return;
		}
    }
    //处理失败情况
    fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
    exit(1);//程序异常终止信号
}

ino_t get_inode( char *fname )
//返回inode号
{
	struct stat info;

	if (stat(fname, &info ) == -1){
		fprintf(stderr, "Cannot stat ");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}