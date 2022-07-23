#include "../include/SOCK.hpp"
#include "cl_UI.hpp"
#include "cl_LOG.hpp"


void loopAction(MsgData &msg, int sockfd)
{
  // 增加程序健壮性
  firstMenu();
  while(1)
  {
    std::string action;
    std::cout << "请选择操作" << std::endl;
    std::cin >> action;
    int opt = atoi(action.c_str());
    switch(opt)
    {
      case 1: // 注册
      {
        registerNewAccount(msg, sockfd);
        break;
      }
      case 2: // 登陆
      {
        loginAccount();
        break;
      }
      case 3: // 修改密码
        //updataPassword();
        break;
      case 4: // 修改昵称
        //updataAccount();
        break;
      case 5: // 找回密码
        //findPassword();
        break;
      case 0: // 退出
        //loginOff();
        std::cout << "程序即将退出!" << std::endl;
        sleep(1);
        exit(EXIT_SUCCESS);
        break;
    }
  }
}

int main(int argc, char* argv[])
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

  int clientSocket = Sock::Socket();
  Sock::Connect(clientSocket, IP, PORT);

  std::cout << "服务器连接成功" << std::endl;
  MsgData msg;
  loopAction(msg, clientSocket);
  close(clientSocket);

  return 0;
}