#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <iostream>
#include "error.h"

char IP[15];
int PORT;

class Sock
{
public:
  static int Socket()
  {
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0) {
      my_error(std::string("socket"), __LINE__);
    }
    return socketfd;
  }

  static void Bind(char *ip, int sockfd, int port)
  {
    struct sockaddr_in this_addr;
    memset(&this_addr, 0, sizeof(this_addr));
    this_addr.sin_family = AF_INET;
    this_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &this_addr);

    if(bind(sockfd, (struct sockaddr*)&this_addr, sizeof(this_addr)) < 0) {
      my_error(std::string("bind"), __LINE__);
    }    
  }

  static void Listen(int sockfd, int limits) 
  {
    if(listen(sockfd, limits) < 0) {
      my_error(std::string("listen"), __LINE__);
    }
  }

  static void Accept(int sockfd) 
  {
    if(accept(sockfd, NULL, NULL) < 0) {
      my_error(std::string("accept"), __LINE__);
    }
  }

  static void Connect(int sockfd, char *ip, int port)
  {
    struct sockaddr_in this_addr;
    memset(&this_addr, 0 ,sizeof(this_addr));
    this_addr.sin_family = AF_INET;
    this_addr.sin_port = port;
    inet_pton(AF_INET, ip, &this_addr);
    
    if(connect(sockfd, (struct sockaddr*)&this_addr, sizeof(this_addr)) < 0) {
      my_error(std::string("connect"), __LINE__);
    }

  }

  static void Setsock(int sockfd)
  {
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, NULL, 0);
  }
};

