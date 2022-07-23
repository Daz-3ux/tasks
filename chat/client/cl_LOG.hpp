#ifndef CL_LOG_H
#define CL_LOG_H
#include "cl_UI.hpp"
#include <vector>
#include <map>
#include <string>
#include <string.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum Status{
  _Register = 1, _LOGIN, _NEWPSWD, _NEWNAME, FINDPSWD, _OFF
};

struct MsgData 
{
  char name[32];
  char passwd[32];
  bool online;
  bool root;
  int loginStatus;
  char question[256];
  char answer[256];

  MsgData() : online(false), root(false) {}
};


std::string registerJson(MsgData &msg, int sockfd)
{
  json root;
  root["name"] = msg.name;
  root["passwd"] = msg.passwd;
  root["online"] = msg.online;
  root["root"] = msg.root;
  root["loginStatus"] = msg.loginStatus;
  root["question"] = msg.question;
  root["answer"] = msg.answer;

  std::string s = root.dump();
  return s;
}

void registerNewAccount(MsgData &msg,int sockfd)
{
  std::cout << "Register youself!" << std::endl;

  std::cout << "Please input a name" << std::endl;
  std::cin >> msg.name;
  std::cout << "Please input a passwd" << std::endl;
	system("stty -echo");
  std::cin >> msg.passwd;
  system("stty echo");

  std::cout << "Please input a secur-question" << std::endl;
  std::cin >> msg.question;
  std::cout << "Please input a secur-answer" << std::endl;
  std::cin >> msg.answer;

  msg.loginStatus = _Register;
  msg.online = false;
  msg.root = false;


  std::string newone = registerJson(msg, sockfd);
  std::cout << newone.size() << std::endl;
  // std::cout << newone.c_str() << std::endl;
  // 发送新用户到服务器
  send(sockfd, newone.c_str(), newone.size(), 0);
}

void loginAccount()
{
  UserMenu();
  while(1)
  {
    std::string action;
    std::cout << "请选择操作" << std::endl;
    std::cin >> action;
    int opt = atoi(action.c_str());
    switch(opt)
    {
      case 1: // 添加好友
      {
        break;
      }
      case 2: // 删除好友
      {
        
        break;
      }
      case 3: // 查看好友列表

        break;
      case 4: // 查看好友请求
        //updataAccount();
        break;
      case 5: // 加入群
        //findPassword();
        break;
      case 6: //

        break;
      case 7: //

        break;
      case 8: //

        break;
      case 0: // 退出
      {
        //loginOff();
        std::cout << "即将退出账号!" << std::endl;
      }
      default:
      {
        std::cout << "请重新输入" << std::endl;
      }
    }
  }
}

#endif