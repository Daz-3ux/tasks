#ifndef CL_JSON_H
#define CL_JSON_H

#include <vector>
#include <map>
#include <string>
#include <string.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include "cl_LOG.hpp"

using json = nlohmann::json;


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

#endif