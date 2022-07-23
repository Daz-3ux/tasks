#ifndef SE_IO_H
#define SE_IO_H

#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include "../include/error.h"

int my_recv(int fd, char *buf, int len)
{
  char *temp = buf;
  while (len > 0){
    ssize_t ret = recv(fd, temp, len, 0);
    if(ret < 0) {
      if(errno == EAGAIN) {
        break;
      }
      my_error("recv", __FILE__, __LINE__);
    }else if(ret == 0) {
      my_error("接受的数据包大小为零", __FILE__, __LINE__);
    }else {
      temp += ret;
      len -= ret;
    }
  }
  return len;
}

void mygets(char *str, int num)
{
  char *ret;
  int i = 0;
  ret = fgets(str, num, stdin);
  if(ret) {
    while(str[i] != '\n' && str[i] != '\0') {
      i++;
    }
    if(str[i] == '\n') {
      str[i] = '\0';
    }else { // 丢弃过长字符
      while(getchar() != '\n') {
        continue;
      }
    }
  }
}


#endif