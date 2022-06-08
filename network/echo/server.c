#include "head.h"

int main(int argc, char **argv)
{
    int serverFd, connfd;
    int ret;
    socklen_t len;
    // 定义服务器结构体与客户端结构体
    struct sockaddr_in serveraddr, clientaddr;
    // 将缓冲区全部置0
    char readBuf[MAXBUFFER] = {0};
    // 存放ip地址
    char ip[40] = {0};
    // 使用socket()创建一个新的套接字
    (serverFd = socket(AF_INET, SOCK_STREAM, 0));
    // 重置服务器结构体
    bzero(&serveraddr, sizeof(serveraddr));
    // 设置结构体参数
    serveraddr.sin_family = AF_INET;//IPv4协议
    serveraddr.sin_port = htons(1234);//端口号
    // 将点分十进制ip地址转换为机器可读的二进制格式
    inet_pton(AF_INET, "127.0.0.1" , &serveraddr.sin_addr);
    /* 
    将socket绑定到指定地址
    SA*是为了将IPv4的结构体格式转换为通用的结构体格式
    */
    bind(serverFd, (struct sockaddr *)&serveraddr,sizeof(serveraddr));
    // 开始监听地址,允许其他socket接入连接
    listen(serverFd, 5);
    // 清空客户端结构体
    bzero(&clientaddr, sizeof(clientaddr));

    len = sizeof(clientaddr);
    while(1){//循环,一直处理客户端连接
        // 建立与其他socket的连接
        connfd = accept(serverFd, (struct sockaddr *)&clientaddr, &len);
        printf("%s 连接到服务器\n", inet_ntop(AF_INET, &clientaddr.sin_addr,ip, sizeof(ip)));
        // 打印客户端输入
        while((ret = read(connfd, readBuf, MAXBUFFER))){
            printf("%s\n", readBuf);
        }
        // 一个客户端退出后关闭与其的连接
        close(connfd);
    }
    close(serverFd);
    return 0;
}