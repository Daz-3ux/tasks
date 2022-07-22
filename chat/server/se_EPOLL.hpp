#ifndef _SE_EPOLL_H
#define _SE_EPOLL_H
#include "../include/SOCK.hpp"
#include "../include/error.h"
#include <sys/epoll.h>

typedef struct socketinfo {
  int fd;
  int epfd;
} socketInfo;

class User
{

};

class Epoll
{
public:
  static int Create()
  {
    int epfd = epoll_create(6);
    if(epfd < 0) {
      my_error("epoll_create", __FILE__, __LINE__);
    }

    return epfd;
  }
};

void *acceptConn(void *arg) {
  printf("accept connection: %ld", pthread_self());
  socketInfo *info = (socketInfo *)arg;
  int cfd = accept(info->fd, NULL, NULL);

  int flag = fcntl(cfd, F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(cfd, F_SETFL, flag);

  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = cfd;
  int ret = epoll_ctl(info->epfd, EPOLL_CTL_ADD, cfd, &ev);
  if (ret == -1) {
    my_error("ctl error", __FILE__,__LINE__);
    exit(1);
  }

  free(info);
  return NULL;
}

void *communication(void *arg) {
  printf("communication: %ld", pthread_self());
  socketInfo *info = (socketInfo *)arg;
  int fd = info->fd;
  int epfd = info->epfd;
  char buf[4096];
  memset(buf, 0, sizeof(buf));
  while (1) {
    int len = recv(fd, buf, sizeof(buf), 0);
    if (len == -1) {
      if (errno == EAGAIN) {
        printf("数据接受完毕\n");
        break;
      }
      perror("recv error");
      // exit(1);
      break;
    } else if (len == 0) {
      printf("客户端断开连接!\n");
      epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
      break;
    }
    write(STDOUT_FILENO, buf, len);
  }

  free(info);
  return NULL;
}

#endif