#ifndef CL_LOG_H
#define CL_LOG_H
#include "cl_UI.hpp"
#include <vector>
#include <map>
#include <string>
#include <string.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include "cl_LOG.hpp"

using json = nlohmann::json;

enum Status{
  _Register = 1
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
  std::cout << "Register youself" << std::endl;
  std::cout << "Please input a name" << std::endl;
  std::cin >> msg.name;
  std::cout << "Please input a passwd" << std::endl;
	system("stty -echo");
  std::cin >> msg.passwd;
  system("stty echo");
  msg.loginStatus = _Register;
  msg.online = false;
  msg.root = false;

  std::string newone = registerJson(msg, sockfd);

  system("clear");

  // 发送新用户到服务器
  send(sockfd, newone.c_str(), newone.size(), 0);
}

#endif