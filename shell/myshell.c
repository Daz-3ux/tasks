#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<signal.h>
#include<errno.h>
#include <readline/history.h>
#include <readline/readline.h>

#define OUT 0
#define IN 1
#define MAX_CMD 10
#define MAX_CMD_LEN 100
#define BUFFSIZE 100

int argc;
char *argv[MAX_CMD];
char COMMAND[MAX_CMD][MAX_CMD_LEN];
char backupCommand[BUFFSIZE];

void my_signal();
void my_error();
void parse(char *);
void do_cmd(int, char **);
int command_with_OutRe(char *);
int command_with_InRe(char *); 
int command_with_ReOutInRed(char *);
int command_with_Pipe(char *);
int command_with_Back(char *);

int callCd(int );
int printHistory(char COMMAND[MAX_CMD][MAX_CMD_LEN]);

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景


int main()
{
    my_signal();
    while(1){
        char *command = readline(BEGIN(49,34)"ypd-super-shell ￥$ "CLOSE);
        if(!command){
            my_error("readline",__LINE__);
        }
        add_history(command);
        write_history(NULL);
        parse(command);
        do_cmd(argc,argv);
        argc = 0;
        free(command);
    }
}




void parse(char *command)
{
    //初始化argv与argc
    for(int i = 0; i < MAX_CMD; i++){
        argv[i] = NULL;
        for(int j = 0;j < MAX_CMD_LEN; j++){
            COMMAND[i][j] = '\0';
        }
    }
    argc = 0;//命令数计数器
    
    strcpy(backupCommand, command);//备份命令
    //下列操作会改变command数组
    int j = 0;
    int len = strlen(command);
    for(int i = 0; i < len; i++){
        if(command[i] != ' '){
            COMMAND[argc][j++] = command[i];
        }else{//command[i] == ' '
            if(j != 0){//j为0则为连续空格情况
                COMMAND[argc][j] = '\0';
                argc++;
                j = 0;
            }
        }
    }
    if(j != 0){//处理命令行末尾
        COMMAND[argc][j] = '\0';
    }

    /*处理解析内置命令
    若输入ls -a则存为
    argv[0] = "ls"
    argv[1] = "-a"
    */
    //OUT 0 IN 1
    argc = 0;
    int flg = OUT;
    for(int i = 0; command[i] != '\0'; i++){
        if(flg == OUT && !isspace(command[i])){
            flg = IN;
            argv[argc++] = command + i;
        }else if(flg == IN && isspace(command[i])){
            flg = OUT;
            command[i] = '\0';
        }
    }
    argv[argc] = NULL;
}

void do_cmd(int argc, char **argv)
{
    char *buf;
    //识别输出重定向
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], ">") == 0){
            strcpy(buf,backupCommand);
            int sample = command_with_OutRe(buf);
            return;
        }
    }
    //识别输入重定向
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], "<") == 0){
            strcpy(buf,backupCommand);
            int sample = command_with_InRe(buf);
            return;
        }
    }
    //识别追加写重定向
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], ">>") == 0){
            strcpy(buf,backupCommand);
            int sample = command_with_ReOutInRed(buf);
            return;
        }
    }
    //识别管道命令
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], "|") == 0){
            strcpy(buf,backupCommand);
            int sample = command_with_Pipe(buf);
            return;
        }
    }
    //识别后台运行命令
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], "&") == 0){
            strcpy(buf,backupCommand);
            int sample = command_with_Back(buf);
        }
    }

    pid_t pid;
    //识别shell内置命令
    if(strcmp(COMMAND[0], "cd") == 0){
        int res = callCd(argc);
        if(!res){
            my_error("cd",__LINE__);
        }
    }else if(strcmp(COMMAND[0], "history") == 0){
        printHistory(COMMAND);
    }else if(strcmp(COMMAND[0], "exit") == 0){
        printf("--------------GoodBye---------------\n");
        exit(0);
    }else{
        switch(pid = fork()){
            case -1:
                my_error("fork",__LINE__);
            case 0:
                execvp(argv[0],argv);
                my_error("execvp",__LINE__);
            default:{
                int status;
                waitpid(pid, &status, 0);//等待任何组进程
                int err_num = WEXITSTATUS(status);//宏用来指出子进程是否正常退出
                if(err_num){
                    printf("Error: %s\n", strerror(err_num));
                }
            }
        }
    }
}

int command_with_OutRe(char *buf)
{

}
int command_with_InRe(char *buf)
{

}
int command_with_ReOutInRed(char *buf)
{

}
int command_with_Pipe(char *buf)
{

}
int command_with_Back(char *buf)
{

}

int callCd(int argc)
{

}
int printHistory(char COMMAND[MAX_CMD][MAX_CMD_LEN])
{

}

void my_signal()
{
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}

void my_error(char *string, int line)
{//myerror("malloc", __LINE__);
    printf("***********************");
    fprintf(stderr, "Line:%d,error:\n", line);
    fprintf(stderr, "%s:%s\n", string, strerror(errno));
    exit(EXIT_FAILURE);
}