#include "../include/SOCK.hpp"
#include "se_UI.hpp"
#include "se_EPOLL.hpp"

#define MAX_CLIENT_NUM 32


int main(int argc, char **argv)
{
  if(argc != 3) {
    std::cout << "Usage: " << basename(argv[0]) << " ip_address port_number" << std::endl;
    std::cout << "程序即将退出, 下次请正确输入" <<std::endl;
    sleep(0.5);
    exit(EXIT_FAILURE);
  }else{
    strncpy(IP, argv[1], sizeof(IP));
    PORT = atoi(argv[2]);
    std::cout << "ip_address: " << IP << " port_number: " << PORT << std::endl;
  }

  int lfd = Sock::Socket();
  Sock::Setsock(lfd);
  Sock::Bind(IP, lfd, PORT);
  Sock::Listen(lfd, MAX_CLIENT_NUM+1);

  se_baseUi();

  int epfd = Epoll::Create();
  
  struct epoll_event ev;
  ev.events = EPOLLIN || EPOLLET;
  ev.data.fd = lfd;
  int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
  if (ret == -1) {
    perror("epoll_ctl");
    exit(1);
  }

  struct epoll_event evs[1024];
  int size = sizeof(evs) / sizeof(evs[0]);

  while (1) {
    int num = epoll_wait(epfd, evs, size, -1);
    pthread_t tid;
    for (int i = 0; i < num; i++) {
      int fd = evs[i].data.fd;
      socketInfo *info = (socketInfo *) malloc(sizeof(socketInfo));
      info->fd = fd;
      info->epfd = epfd;
      // epoll系列函数为线程安全的,无需加锁
      if (fd == lfd) { // 监听套接字
        pthread_create(&tid, NULL, acceptConn, info);
        pthread_detach(tid);
      } else if(evs[i].events & EPOLLIN) { // 通信
        pthread_create(&tid, NULL, communication, info);
        pthread_detach(tid);
      }
    }
  }

  close(lfd);

  return 0;

}
