#ifndef _ERROR_H
#define _ERROR_H

#include <iostream>
#include <errno.h>

void my_error(std::string msg, std::string file, int line)
{
  perror(msg.c_str());
  std::cout << "错误发生在: "<< file << " " << line << std::endl;
  exit(1);
}

#endif