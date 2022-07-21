#ifndef _SE_EPOLL_H
#define _SE_EPOLL_H
#include "../include/SOCK.hpp"
#include "../include/error.h"
#include <sys/epoll.h>

typedef struct socketinfo {
  int fd;
  int epfd;
} socketInfo;

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
    perror("ctl error");
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
  char buf[5];
  char temp[4096];
  bzero(temp, sizeof(temp));
  while (1) {
    int len = recv(fd, buf, sizeof(buf), 0);
    if (len == -1) {
      if (errno == EAGAIN) {
        printf("数据接受完毕\n");
        int ret = send(fd, temp, strlen(temp)+1, 0);
        if(ret == -1) {
          perror("send error");
        }
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
    printf("origin buf: %s\n", buf);
    for (int i = 0; i < len; i++) {
      buf[i] = toupper(buf[i]);
    }
    strncat(temp+strlen(temp), buf, len);
    write(STDOUT_FILENO, temp, len);
  }

  free(info);
  return NULL;
}

#endif