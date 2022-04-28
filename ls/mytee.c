/*
 * =====================================================================================
 *
 *       Filename:  mytee.c
 *
 *    Description:  
    tee 命令是从标准输入中读取数据,直至文件结尾,随后将数据写入标准输出和命令行参数所指定的文件。
    请使用I/O 系统调用实现 tee 命令。默认情况下,若已存在与命令行参数指定文件同名的文件,tee 命令会将其覆盖。如文件已存在,请实现-a 命令行选项(tee-a file)在文件结尾处追加数据。
(请参考附录 B 中对 getopt()函数的描述来解析命令行选项)
 *
 *        Version:  1.0
 *        Created:  2022年03月07日 18时45分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lin3ux 
 *   Organization:  
 *
 * =====================================================================================
 */
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

struct teeConfigStruct
{
    int a;
    char *outputPath;
    int outputFd;
}teeConfig;


void getArgs(int argc, char *argv[])
{
    int opt;
    if( (opt = getopt(argc,argv,"a") )!= -1 ) {
        teeConfig.a = 0;
        switch(opt){
        case 'a':
                printf("追加模式\n");
                teeConfig.a = 1;
                break;
            default:
                printf("参数错误\n");
                _exit(0);
                break;
            }
        }

    if(argc - optind < 1){//optined:argv的当前索引值
        printf("参数太少\n");
        _exit(0);
    }

    teeConfig.outputPath = argv[optind];
    printf("输出路径 %s \n",teeConfig.outputPath);
}

int main(int argc, char* argv[])
{
    getArgs(argc, argv);
    int outputFileFlag;
    if(teeConfig.a == 1){
        outputFileFlag = O_RDWR | O_CREAT | O_APPEND;
    }else{
        outputFileFlag = O_RDWR | O_CREAT | O_TRUNC;//O_TRUNC若文件存在，则长度被截为0，属性不变 
    }

    teeConfig.outputFd = open(teeConfig.outputPath,outputFileFlag,0777);
    if(teeConfig.outputFd == -1){
        printf("无法打开文件!\n");
        _exit(0);
    }

    char buff;
    while(read(STDIN_FILENO,&buff,1) >0){//STDIN_FILENO就是标准输入设备（一般是键盘）的文件描述符
        printf("%c",buff);
        if(write(teeConfig.outputFd,&buff,1) == -1){
            printf("无法写入!\n");
            _exit(0);
        }
    }

    if(close(teeConfig.outputFd) == -1){
        printf("无法关闭文件!\n");
    }
}











