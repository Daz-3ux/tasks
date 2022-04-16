#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
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

struct 
{
    char *argv[BUFFSIZE];
    char *in;
    char *out;
}cmd[16];

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
int command_with_OutRePlus(char *);
int parse_pipe(char *buf,int cmd_num);
int command_with_Pipe(char *);
int command_with_Back(char *);
void callCd(int );
int printHistory(char COMMAND[MAX_CMD][MAX_CMD_LEN]);

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景


int main()
{
    //my_signal();
    while(1){
        char place[BUFFSIZE];
        getcwd(place, BUFFSIZE);
        printf(BEGIN(36,36)"%s:"CLOSE, place);
        char *command = readline(BEGIN(33,33)"ypd-super-shell ￥$ "CLOSE);
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
/*
command 为用户输入的命令
*/
    //初始化argv与argc
    for(int i = 0; i < MAX_CMD; i++){
        argv[i] = NULL;
        for(int j = 0;j < MAX_CMD_LEN; j++){
            COMMAND[i][j] = '\0';
        }
    }
    argc = 0;//命令数计数器
    
    strcpy(backupCommand, command);//备份命令
    
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
    /*处理__内置命令__*/
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
    char buf[1024];
    ///识别管道命令
    for(int i = 0;i < BUFFSIZE; i++){
        if(backupCommand[i] == '|'){
            strcpy(buf,backupCommand);
            command_with_Pipe(buf);
            return;
        }
    }
    //识别输出重定向
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], ">") == 0){
            strcpy(buf,backupCommand);
            command_with_OutRe(buf);
            return;
        }
    }
    //识别输入重定向
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], "<") == 0){
            strcpy(buf,backupCommand);
            command_with_InRe(buf);
            return;
        }
    }
    //识别追加写重定向
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], ">>") == 0){
            strcpy(buf,backupCommand);
            command_with_OutRePlus(buf);
            return;
        }
    }
    //识别后台运行命令
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], "&") == 0){
            strcpy(buf,backupCommand);
            command_with_Back(buf);
            return;
        }
    }

    pid_t pid;
    //识别shell内置命令
    if(strcmp(COMMAND[0], "cd") == 0){
        callCd(argc);
    }else if(strcmp(COMMAND[0], "history") == 0){
        HIST_ENTRY **history = NULL;
        history = history_list();
        for(int i = 0; history[i] != NULL; i++){
            printf("%s\n",history[i] -> line);
        }
    }else if(strcmp(COMMAND[0], "exit") == 0){
        printf("--------------GoodBye---------------\n");
        exit(0);
    }else{
        switch(pid = fork()){
            case -1:
                my_error("fork",__LINE__);
            case 0://子进程执行任务
                execvp(argv[0],argv);
                my_error("execvp",__LINE__);
            default:{//父进程等待子进程结束
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

//buf实际为用户输入的command
int command_with_OutRe(char *buf)
{//command > file
    char OutFile[1024];
    memset(OutFile, 0, BUFFSIZE);
    int RedNum = 0;
    for(int i = 0; buf[i]; i++){
        if(buf[i] == '>'){
            RedNum++;
            break;
        }
    }
    if(RedNum != 1){
        my_error("error num of OutRe",__LINE__);
    }
    int fg = 0;
    for(int i =0;i < argc; i++){//与分割好的命令逐个比较，确定重定向文件
        if(strcmp(COMMAND[i], ">") == 0){
            if(i+1 < argc){//因为有argv[argc] == NULL,所以不用<=
                strcpy(OutFile,COMMAND[i+1]);
                fg = i-1;
            }else{
                my_error("missing output file",__LINE__);
            }
        }
    }
    for (int j = 0; j < strlen(buf); j++) {
        if (buf[j] == '>') {
            buf[j - 1] = '\0';
            break;
        }
    }
    
    parse(buf);//重定向符号后面的为文件，所以需要重新解析命令
    pid_t pid = fork();
    if(pid < 0){
        my_error("fork",__LINE__);
    }
    if(pid == 0){
        int fd;
        fd = open(OutFile, O_RDWR | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR);
        if(fd < 0){
            my_error("open",__LINE__);
        }
        dup2(fd,STDOUT_FILENO);//灵魂
        execvp(argv[0], argv);
        if(fd != STDOUT_FILENO){
            close(fd);
        }
        my_error("fault argu",__LINE__);
    }else{
        int status;
        waitpid(pid, &status, 0);
        int err = WEXITSTATUS(status);
        if(err){
            printf("Error:%s\n",strerror(err));
        }
    }
}

int command_with_InRe(char *buf)
{//command < file
    char InFile[1024];
    memset(InFile, 0, BUFFSIZE);
    int InNum = 0;
    for(int i = 0; buf[i]; i++){
        if(buf[i] == '<'){
            InNum++;
            break;
        }
    }
    if(InNum != 1){
        my_error("error num of InRe",__LINE__);
    }

    for(int i =0;i < argc; i++){//与分割好的命令逐个比较，确定重定向文件
        if(strcmp(COMMAND[i], "<") == 0){
            if(i+1 < argc){
                strcpy(InFile,COMMAND[i+1]);
            }else{
                my_error("missing output file",__LINE__);
            }
        }
    }
    for (int j = 0; j < strlen(buf); j++) {
        if (buf[j] == '<') {
            buf[j - 1] = '\0';
            break;
        }
    }
    
    parse(buf);//重定向符号后面的为文件，所以需要重新解析命令
    pid_t pid = fork();
    if(pid < 0){
        my_error("fork",__LINE__);
    }
    if(pid == 0){
        int fd;
        fd = open(InFile, O_RDONLY, S_IRUSR | S_IWUSR);
        if(fd < 0){
            my_error("open",__LINE__);
        }
        dup2(fd,STDIN_FILENO);//灵魂
        execvp(argv[0], argv);
        if(fd != STDIN_FILENO){
            close(fd);
        }
        my_error("fault argu",__LINE__);
    }else{
        int status;
        waitpid(pid, &status, 0);
        int err = WEXITSTATUS(status);
        if(err){
            printf("Error:%s\n",strerror(err));
        }
    }
}

int command_with_OutRePlus(char *buf)
{
    char OutFileP[1024];
    memset(OutFileP, 0, BUFFSIZE);
    int RedPNum = 0;
    for(int i = 0; buf[i]; i++){
        if(buf[i] == '>' && buf[i+1] == '>'){
            RedPNum++;
            break;
        }
    }
    if(RedPNum != 1){
        my_error("error num of OutRe",__LINE__);
    }

    for(int i =0;i < argc; i++){//与分割好的命令逐个比较，确定重定向文件
        if(strcmp(COMMAND[i], ">>") == 0){
            if(i+1 <= argc){
                strcpy(OutFileP,COMMAND[i+1]);
            }else{
                my_error("missing output fileP",__LINE__);
            }
        }
    }
    for (int j = 0; j < strlen(buf); j++) {
        if (buf[j] == '>' && buf[j+1] == '>') {
            buf[j - 1] = '\0';
            break;
        }
    }
    
    parse(buf);//重定向符号后面的为文件，所以需要重新解析命令
    pid_t pid = fork();
    if(pid < 0){
        my_error("fork",__LINE__);
    }
    if(pid == 0){
        int fd;
        fd = open(OutFileP,O_RDWR|O_APPEND|O_CREAT,S_IRUSR | S_IWUSR);
        if(fd < 0){
            my_error("open",__LINE__);
        }
        dup2(fd,STDOUT_FILENO);//灵魂
        execvp(argv[0], argv);
        if(fd != STDOUT_FILENO){
            close(fd);
        }
        my_error("fault argu",__LINE__);
    }else{
        int status;
        waitpid(pid, &status, 0);
        int err = WEXITSTATUS(status);
        if(err){
            printf("Error:%s\n",strerror(err));
        }
    }
}

int flag_out = 0;
int flag_in = 0 ;
int parse_pipe(char *buf,int cmd_num)
{
    int n = 0;
    char *p = buf;
    cmd[cmd_num].in = NULL;
    cmd[cmd_num].out = NULL;
    while(*p != '\0'){
        if(*p == ' '){
            *p++ = '\0';
            continue;
        }
        if(*p == '<'){
            *p = '\0';
            flag_in = 1;
            while(*(++p) == ' '){
                ;
            }
            cmd[cmd_num].in = p;
            continue;
        }
        if(*p == '>'){
            *p = '\0';
            flag_out = 1;
            while(*(++p) == ' ');
            cmd[cmd_num].out = p;
            continue;
        }
        if(*p != ' ' && ((p == buf) || *(p-1) == '\0')){
            if(n < MAX_CMD){
                cmd[cmd_num].argv[n++] = p++;
                continue;
            }else{
                return -1;
            }
        }
        ++p;
    }
    if(n == 0){
        return -1;
    }
    return 0;
}


int command_with_Pipe(char *buf)
{
    int i, j;
    int cmd_num = 0, pipe_num = 0;
    int fd[16][2];
    char *curcmd;
    char *nextcmd = buf;
    for (int k = 0; buf[k]; k++){
        if(buf[k] == '|'){
            pipe_num++;
        }
    }
    while ((curcmd = strsep(&nextcmd, "|"))){
        flag_out = 0;
        flag_in = 0;
        if(parse_pipe(curcmd, cmd_num++) < 0){
            cmd_num--;
            break;
        }
        if(cmd_num == 17)//16根管道最多支持17条命令
            break;
    }

    for (i = 0; i < pipe_num; i++){//创建管道
        if(pipe(fd[i])){
            my_error("pipe", __LINE__);
        }
    }

    pid_t pid;
    for (i = 0; i <= pipe_num; i++){ //管道数目决定创建子进程个数
        if((pid = fork()) == 0)
            break;
    }

    if(pid == 0){
        if(pipe_num != 0){
            if (i == 0){ //第一个创建的子进程
            //管道的输入为标准输入
                dup2(fd[0][1], STDOUT_FILENO);
                close(fd[0][0]);

                for (j = 1; j < pipe_num; j++){
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }else if (i == pipe_num){ //最后一个创建的子进程
            //管道的输出为标准输出
                dup2(fd[i-1][0], STDIN_FILENO);
                close(fd[i-1][1]);

                for (j = 0; j < pipe_num - 1; j++){
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }else{
                //重定中间进程的标准输入至管道读端
                dup2(fd[i-1][0], STDIN_FILENO); 
                close(fd[i-1][1]);
                //重定中间进程的标准输出至管道写端
                dup2(fd[i][1], STDOUT_FILENO);
                close(fd[i][0]);

                for (j = 0; j < pipe_num; j++){ //关闭不使用的管道读写两端
                    if (j != i || j != (i - 1)){
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                }
            }
        }
        if(flag_in){
            int file_fd = open(cmd[i].in, O_RDONLY);
            dup2(file_fd, STDIN_FILENO);
        }
        if(flag_out){
            int file_fd = open(cmd[i].out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(file_fd, STDOUT_FILENO);
        }
        // else{
        //     if(strcmp(COMMAND[0],"exit")){
        //         return 0;
        //     }
        // }
        execvp(cmd[i].argv[0], cmd[i].argv); //执行用户输入的命令
        my_error("execvp",__LINE__);
    }else{// parent
    //关闭父进程两侧管道
        for (i = 0; i < pipe_num; i++){
                close(fd[i][0]);
                close(fd[i][1]);
            }

        for (i = 0; i < cmd_num; i++){
            wait(NULL);//一父收一子
        }
    }
}

int command_with_Back(char *buf)
{
    char BackBuf[strlen(buf)];
    memset(BackBuf, 0, strlen(buf));
    //提取 & 前的命令
    for(int i = 0; i < strlen(buf); i++){
        BackBuf[i] = buf[i];
        if(buf[i] == '&'){
            BackBuf[i-1] = '\0';
            break;
        }
    }

    pid_t pid = fork();
    if(pid < 0){
        my_error("Fork",__LINE__);
    }

    if(pid == 0){
        //FILE *freopen(const chat*pathname, const char*mode, FILE *stream);
        freopen("/dev/null", "w", stdout); 
        freopen("/dev/null", "r", stdin);
        signal(SIGCHLD, SIG_IGN);
        parse(BackBuf);
        execvp(argv[0], argv);
        my_error("execvp",__LINE__);
    }else{
        exit(0);//父进程直接退出
    }
}


char oldPath[BUFFSIZE];
void callCd(int argc)
{
    int result = 1;
    if(argc == 1) {
        int ret = chdir("/home/yyn");
        return;
    }else{
        int ret = 0;
        int flag_gang = 0;
        int flag_piao = 0;
        int flag;
        for(int i = 0; COMMAND[1][i]; i++) {
            if(COMMAND[1][i] == '-'){
                flag_gang = 1;
            }
            if(COMMAND[1][i] == '~'){
                flag_piao = 1;
            }
        }
        if(flag_gang){
            if((ret = chdir(oldPath)) == -1){
                my_error("chdir",__LINE__);
            }
            getcwd(oldPath, BUFFSIZE);
        }else if(flag_piao){
            getcwd(oldPath, BUFFSIZE);
            if((ret = chdir("/home/yyn")) == -1){
                my_error("chdir",__LINE__);
            }
        }else{
            getcwd(oldPath, BUFFSIZE);
            ret = chdir(COMMAND[1]);
        }
        if(ret){
            return;
        }
    }
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
    fprintf(stderr, "Line:%d,error:\n", line);
    fprintf(stderr, "%s:%s\n", string, strerror(errno));
    printf("***********************\n");
}
