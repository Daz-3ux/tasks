#define _GNU_SOURCE 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define BUFFER_SIZE 64

int main(int argc, char *argv[])
{
    //由用户指定通信地址与端口
    if (argc <= 2) {
      //basename为UNIX下系统调用:当向basename传递一个路径名时，它会删除任何前缀，直到最后一个斜线字符，然后返回结果
      printf("usage: %s ip_address port_number\n", basename(argv[0]));
      return 1;
    }

    // 获取ip地址 与 端口号
    const char *ip = argv[1];
    int port = atoi(argv[2]);// atoi:将字符串转换为整数

    // 客户端连接配置
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);

    // 通信fd
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("connection failed\n");
        close(sockfd);
        return 1;
    }

    #if 0
    pollfd fds[2];
    /* 注册文件描述符0(标准输入)和文件描述符sockfd上的可读事件 */
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    //  POLLRDHUP用于判断对端是否关闭
    fds[1].fd = sockfd;
    fds[1].events = POLLIN | POLLRDHUP;
    fds[1].revents = 0;
    #endif

    #if 1
    // 创建epoll实例
    int epfd = epoll_create(1);
    if(epfd < 0) {
      perror("epoll_create");
      exit(1);
    }

    int ret = 0;
    struct epoll_event ev;
    // 为标准输入注册epoll
    // 检测可读事件, 边缘触发
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = 0;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &ev);
    assert(ret != -1);
    // 为套接字注册epoll
    // 检测可读事件, 边缘触发, 检测对端关闭
    ev.events = EPOLLOUT | EPOLLET | EPOLLRDHUP;
    ev.data.fd = sockfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    assert(ret != -1);
    #endif

    // 创建管道和缓冲区用于零拷贝
    char read_buf[BUFFER_SIZE];
    int pipefd[2];
    ret = pipe(pipefd);
    assert(ret != -1);

    #if 1
    // evs用于存放就绪事件
    struct epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(struct epoll_event);

    #endif

    while (1)
    {
      #if 1
      int num = epoll_wait(epfd, evs, size, -1);
      // 
      for(int i = 0; i < num; i++) {
        //
        int curfd = evs[i].data.fd;

        // 处理连接
        if(curfd == sockfd) {
          if(evs[i].events & EPOLLRDHUP) {
            printf("server close the connection\n");
            break;
          }else if(evs[i].events & EPOLLIN) {
            memset(read_buf, 0, sizeof(read_buf));
            recv(sockfd, read_buf, BUFFER_SIZE-1, 0);
            printf("%s\n", read_buf);
          }
        }

        // 处理标准输入
        if(curfd == 0) {
          if(evs[i].events & EPOLLIN) {
            splice(0, NULL, pipefd[1], NULL, 32768, 
            SPLICE_F_MORE | SPLICE_F_MOVE);
            splice(pipefd[0], NULL, sockfd, NULL, 32768,
            SPLICE_F_MORE | SPLICE_F_MOVE);

            // SPLICE_F_MORE:More data will be coming in a subsequent splice. This is a helpful hint when the fd_out refers to a socket
            // SPLICE_F_MOVE:Attempt to move pages instead of copying
          }

        }
      }

      #endif

      #if 0 
        // nfds:第一个参数数组中最后一个有效元素的下标 + 1
        ret = poll(fds, 2, -1);
        if (ret < 0)
        {
            printf("poll failure\n");
            break;
        }

        if (fds[1].revents & POLLRDHUP)
        {
            printf("server close the connection\n");
            break;
        }
        else if (fds[1].revents & POLLIN)
        {
            memset(read_buf, '\0', BUFFER_SIZE);
            recv(fds[1].fd, read_buf, BUFFER_SIZE - 1, 0);
            printf("%s\n", read_buf);
        }

        if (fds[0].revents & POLLIN)
        {
          // splice:从fdin处传递len个字节到fdout(零拷贝)
            /* 使用splice将用户输入的数据直接写到sockfd上(零拷贝) */
          // 从标准输入处零拷贝传递32768(最大)个字节到管道写端
            ret = splice(0, NULL, pipefd[1], NULL, 32768,
                    SPLICE_F_MORE | SPLICE_F_MOVE);
          // 从管道读端获得数据并将其传递到服务器
            ret = splice(pipefd[0], NULL, sockfd, NULL, 32768,
                    SPLICE_F_MORE | SPLICE_F_MOVE);
        }
      #endif
    }

    close(sockfd);

    return 0;
}