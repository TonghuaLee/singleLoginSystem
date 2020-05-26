#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <hiredis/hiredis.h>
#include "login_redis.h"

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
    bool updateToken(int uid, string token, string refreshToken);

    /**
  主要功能：
  判断用户Token是否正确

  入口参数
  uid： 	          用户UID
  token： 	        用户Token

  出口参数：
  bool ：           true表示正确；false表示不正确
  **/
    bool isTokenRight(int uid, string token);

    /**
  主要功能：
  判断用户Token是否正确

  入口参数
  uid： 	          用户UID

  出口参数：
  bool ：           true表示成功；false表示失败
  **/
    bool cleanToken(int uid);

    /**
  主要功能：
  获取用户Token

  入口参数
  uid： 	          用户UID

  出口参数：
  string ：         用户Token
  **/
    string getUserToken(int uid);

    /**
     * 主要功能
     * 获取用户RefreshToken
     * 入口参数
     * uid： 	          用户UID
     * 出口参数：
     * string ：         用户RefreshToken
     * **/
    string getUserRefreshToken(int uid);

private:
    Redis redis = *Redis::getRedis();
};
