#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>

void my_error(char *string, int line){
    printf("***********************\n");
    fprintf(stderr, "Line:%d,error:\n", line);
    fprintf(stderr, "%s:%s\n", string, strerror(errno));
    printf("***********************\n");
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    int thepipe[2],newfd;
    pid_t pid;

    {
        if(argc != 3){
            my_error("argc != 3",__LINE__);
        }
        if(pipe(thepipe) == -1){
            my_error("pipe",__LINE__);
        }

        if((pid = fork()) == -1){
            my_error("fork",__LINE__);
        }
    }

    if(pid > 0){//父进程接收输出
        close(thepipe[1]);
        if((dup2(thepipe[0], 0)) == -1){
            my_error("dup2",__LINE__);
        }
        close(thepipe[0]);
        execlp(argv[2],argv[2],NULL);
        my_error("execlp",__LINE__);
    }else if(pid == 0){//子进程接收输入
        close(thepipe[0]);
        if(dup2(thepipe[1],1) == -1){
            my_error("dup2",__LINE__);
        }
        close(thepipe[1]);
        execlp(argv[1],argv[1],NULL);
        my_error("execlp",__LINE__);
    }
}
