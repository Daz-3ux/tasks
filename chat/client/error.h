#include <iostream>
#include <errno.h>

void my_error(std::string msg, int line)
{
  perror(msg.c_str());
  std::cout << ": " << line << std::endl;
}