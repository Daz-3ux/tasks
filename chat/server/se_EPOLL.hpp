#ifndef _SE_EPOLL_H
#define _SE_EPOLL_H
#include "../include/SOCK.hpp"
#include "../include/error.h"
#include "se_JSON.hpp"
#include "../include/IO.hpp"
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
  // printf("accept connection: %ld", pthread_self());
  socketInfo *info = (socketInfo *)arg;
  int cfd = accept(info->fd, NULL, NULL);

  int flag = fcntl(cfd, F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(cfd, F_SETFL, flag);

  // 将客户连接加入至epoll
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLET || EPOLLRDHUP || EPOLLERR;
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
  // printf("communication: %ld", pthread_self());
  socketInfo *info = (socketInfo *)arg;
  int fd = info->fd;
  int epfd = info->epfd;
  char buf[4096];
  memset(buf, 0, sizeof(buf));
  my_recv(fd, buf, sizeof(buf));

  std::string s(buf,strlen(buf));

  std::cout << buf << std::endl;
  std::cout << s << std::endl;

  json js = json::parse(s);
  int status = js["loginStatus"];

  if(status == 1) {
    std::cout << "让我们来注册吧" << std::endl;
    std::cin.get();
  }



  free(info);
  return NULL;
}

#endif