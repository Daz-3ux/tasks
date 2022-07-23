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

  static void reset_oneshot(int epfd, int fd)
  {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLERR | EPOLLONESHOT;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);
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
  ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLERR | EPOLLONESHOT;
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
  while(1) {

    int ret = recv(fd, buf, sizeof(buf), 0);
    if(ret == 0) {
      close(fd);
      std::cout << "有一个客户端离线啦" << std::endl;
      break;
    }else if(ret < 0) {
      if(errno == EAGAIN) {
        Epoll::reset_oneshot(epfd, fd);
        break;
      }
    }
  
    std::string s(buf,sizeof(buf));

    json js = json::parse(s);

    int status = js["loginStatus"];

    if(status == 1) { // 注册
      std::cout << "让我们来注册吧" << std::endl;
      
    }else if(status == 2) { // 登陆
      std::cout << "让我们来登陆吧" << std::endl;

    }else if(status == 3) { // 修改密码
      std::cout << "让我们来修改密码吧" << std::endl;

    }else if(status == 4) { // 修改昵称
      std::cout << "让我们来修改昵称吧" << std::endl;

    }else if(status == 5) { // 找回密码
      std::cout << "让我们来找回密码吧" << std::endl;

    }else if(status == 6) { // 退出登陆
      std::cout << "让我们来退出登陆吧" << std::endl;

    }
  }

  



  free(info);
  return NULL;
}

#endif