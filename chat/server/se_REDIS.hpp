#ifndef _SE_REDIS_H
#define _SE_REDIS_H

#include <hiredis/hiredis.h>
#include <iostream>
#include <string>
#include <vector>


class Redis
{
public:
  Redis()
  {
    m_redis = NULL;
    init();
  }
  ~Redis()
  {
    if(m_redis != NULL) {
      redisFree(m_redis);
      std::cout << "redis关闭喽" << std::endl;
    }
  }

  int setString(std::string key, std::string value)
  {
    if(m_redis == NULL || m_redis->err) {
      std::cout << "Redis init error" << std::endl; // 不使用my_eror:不至于输入错误就导致程序崩溃
      init();
      return -1;
    }
    redisReply *reply;
    int result = 0;
    reply = (redisReply *)redisCommand(m_redis, "SET %s, %s", key.c_str(), value.c_str());
    if(reply == NULL) {
      redisFree(m_redis);
      m_redis = NULL;
      result = -1;
      std::cout << "set string faild" << __LINE__ <<std::endl;
      return -1;
    }else if(strcmp(reply->str, "OK") == 0) {
      result = 1;
    }else {
      result = -1;
      std::cout << "set String fail" << __LINE__ << std::endl;
    }
    freeReplyObject(reply);

    return result;
  }

  std::string getString(std::string key)
  {
    if(m_redis = NULL || m_redis->err) {
      std::cout << "Redis init error" << std::endl;
      init();
      return nullptr;
    }
    redisReply *reply;
    reply = (redisReply *)redisCommand(m_redis, "GET %s", key.c_str());
    if(reply == NULL) {
      redisFree(m_redis);
      m_redis = NULL;
      std::cout << "get nothing" << std::endl;
      return nullptr;
    }else if(reply->len <= 0) {
      freeReplyObject(reply);
      return nullptr;
    }else {
      std::string ss;
      ss << reply->str;
      freeReplyObject(reply);
      return ss;
    }
  }

  int setVector(std::string key, std::vector<int> value)
  {
    if(m_redis == NULL || m_redis->err) {
        cout << "Redis init Error !!!" << endl;
        init();
        return -1;
    }

    redisReply *reply;

    int valueSize = value.size();
    int result = 0;

    for(int i = 0; i < valueSize; i++) {
      reply = (redisReply *)redisCommand(m_redis, "PRUSH %s %d", key.c_str(), value.at(i));
      int old = reply->integer;
      if(reply == NULL) {
        redisFree(m_redis);
        m_redis = NULL;
        result = NULL;
        std::cout << "set list fail" << std::endl;
        return -1;
      }
    }else if(reply->integer == old++) {

    }else {
      next? i will tell you tomorrow!
    }

  }

  std::vector<int> getVector(std::string key)
  {

  }

private:
  void init()
  {
    struct timeval timeout = {1, 50000}; // 连接等待时间为1.5秒
    m_redis = redisConnectWithTimeout("127.0.0.1", 6379, timeout);
    if(m_redis->err) {
      my_error("RedisTool : Connection error", __FILE__, __LINE__);
    }else {
      std::cout << "init redis success" << std::endl;
    }
  }

  redisContext *m_redis;
};








#endif