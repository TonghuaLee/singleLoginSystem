#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <grpcpp/grpcpp.h>

#include "source/libs/json/json.h"

#include "source/cpp/manager/db/db_manager.h"

#include "../utils/common_utils.h"
#include <hiredis/hiredis.h>
#include "redis_manager.h"

class LoginRedis
{
public:
    /**
  主要功能：
  更新用户token

  入口参数
  uid： 	          用户UID
  token： 	        用户Token
  refreshToken：    用户RefreshToken

  出口参数：
  bool ：           true表示成功；false表示失败
  **/
  bool LoginRedis::updateToken(int uid, string token, string refreshToken)
  {
    stringstream ssToken;
    ssToken << uid << "_token";
    stringstream ssRToken;
    ssRToken << uid << "_refresh_token";
    return redis.setString(ssToken.str(), token) && redis.setString(ssRToken.str(), refreshToken);
    }

    /**
  主要功能：
  判断用户Token是否正确

  入口参数
  uid： 	          用户UID
  token： 	        用户Token

  出口参数：
  bool ：           true表示正确；false表示不正确
  **/
    bool LoginRedis::isTokenRight(int uid, string token)
    {
        stringstream ssToken;
        ssToken << uid << "_token";
        string redisToken;
        redis.getString(ssToken.str(), redisToken);
        return redisToken == token;
    }

    /**
  主要功能：
  判断用户Token是否正确

  入口参数
  uid： 	          用户UID

  出口参数：
  bool ：           true表示成功；false表示失败
  **/
    bool LoginRedis::cleanToken(int uid)
    {
        stringstream ssToken;
        ssToken << uid << "_token";
        stringstream ssRToken;
        ssRToken << uid << "_refresh_token";
        return redis.delByKey(ssToken.str()) && redis.delByKey(ssRToken.str());
    }

    /**
  主要功能：
  获取用户Token

  入口参数
  uid： 	          用户UID

  出口参数：
  string ：         用户Token
  **/
    string LoginRedis::getUserToken(int uid)
    {
        stringstream ssToken;
        ssToken << uid << "_token";
        string token;
        redis.getString(ssToken.str(), token);
        return token;
    }

    /**
  主要功能：
  获取用户RefreshToken

  入口参数
  uid： 	          用户UID

  出口参数：
  string ：         用户RefreshToken
  **/
    string LoginRedis::getUserRefreshToken(int uid)
    {
        stringstream ssToken;
        ssToken << uid << "_refresh_token";
        string token;
        redis.getString(ssToken.str(), token);
        return token;
    }
};
