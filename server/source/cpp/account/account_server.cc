//
// Created by melon on 2019/1/22.
//

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>

#include <grpcpp/grpcpp.h>
#include <hiredis/hiredis.h>
#include "source/libs/json/json.h"

#include "source/cpp/manager/conf/server_conf.h"
#include "source/cpp/manager/db/db_manager.h"
#include "source/cpp/manager/redis/redis_manager.h"

#include "../utils/common_utils.h"
#include "../utils/param_utils.h"
#include "../utils/log_utils.h"

#include "../constant/my_constant.h"

#include "source/protos/account.grpc.pb.h"

#define LOGD(msg) utils::LogUtil::LOGD(msg);
#define LOGW(msg) utils::LogUtil::LOGW(msg);
#define LOGI(msg) utils::LogUtil::LOGI(msg);
#define LOGE(msg) utils::LogUtil::LOGE(msg);
#define LOGM(bean) utils::LogUtil::LOGM(bean);

using namespace std;
using namespace utils;
using namespace manager;
using namespace my_model;
using namespace constants;

using account::Account;
using account::AddCategoryRequest;
using account::AddTodoRequest;
using account::CodeReply;
using account::ConnectRequest;
using account::FetchCategoryRequest;
using account::FetchTodoListRequest;
using account::LoginRequest;
using account::LogoutRequest;
using account::RefreshRequest;
using account::SignRequest;
using account::UpdateTodoRequest;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using utils::LogMBean;

class LoginDatabase
{
public:
  /*
  主要功能：
  判断是否已经注册

  入口参数
  account： 	      用户账号
 
  出口参数：
  bool ：           true表示已经注册；false表示未注册
  */
  bool isHadSign(string account)
  {
    return Database::getDatabase()->isUserExist(account);
  };

  /**
  主要功能：
  注册用户账号

  入口参数
  account： 	      用户账号
  encrypt_pwd：     用户密码
  pwdSalt：         参与密码初始化的随机盐
  出口参数：
  bool ：           true表示注册成功；false表示注册失败
  **/
  bool signAccount(string account, string encrypt_pwd, string pwdSalt)
  {
    return Database::getDatabase()->addUserAccount(account, encrypt_pwd, pwdSalt);
  }

  /**
  主要功能：
  获取用户信息

  入口参数
  account： 	      用户账号

  出口参数：
  UserAccount：     用户账号信息
  **/
  UserAccount getUserAccount(string account)
  {
    return Database::getDatabase()->queryUserAccountByAccount(account);
  }

  /***------------------- Todo 功能模块------------------------ **/
  /**
   * 添加分类
   **/
  bool addCategory(string title, int uid)
  {
    return Database::getDatabase()->addCategory(title, uid);
  }

  Category getCategory(string title, int uid)
  {
    return Database::getDatabase()->queryCategory(title, uid);
  }

  vector<Category> getCategoryList(int uid)
  {
    return Database::getDatabase()->queryCategoryList(uid);
  }

  int addTodo(string content, int cid, int uid)
  {
    return Database::getDatabase()->addTodo(content, cid, uid);
  }

  Todo getTodo(int tid)
  {
    return Database::getDatabase()->queryTodo(tid);
  }

  Todo updateTodoStatus(int tid, int status)
  {
    return Database::getDatabase()->updateTodoStatus(tid, status);
  }

  vector<Todo> getTodoList(int uid, int cid)
  {
    return Database::getDatabase()->queryTodoList(uid, cid);
  }

private:
};

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
  bool updateToken(int uid, string token, string refreshToken)
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
  bool isTokenRight(int uid, string token)
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
  bool cleanToken(int uid)
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
  string getUserToken(int uid)
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
  string getUserRefreshToken(int uid)
  {
    stringstream ssToken;
    ssToken << uid << "_refresh_token";
    string token;
    redis.getString(ssToken.str(), token);
    return token;
  }

private:
  Redis redis = *Redis::getRedis();
};

class LoginCore
{
public:
  /**
  用户模块-手机号登录 
  
  入口参数
  account： 	      用户账号（手机号）
  password：        用户密码（明文）

  出口参数：
  token：           用户Token（用来接口请求）
  refresh_token：   用户Token（用来刷新Token）
  **/
  CodeReply *handleUserLogin(string account, string password)
  {
    CodeReply *result = new CodeReply();

    LOGD("[account_server.handleUserLogin] user login in:" + account);

    LoginDatabase login_db;
    LoginRedis login_redis;

    UserAccount userAccount = login_db.getUserAccount(account);

    //查询账号是否存在
    if (userAccount.getUid() <= 0)
    {
      result->set_code(ResultCode::UserLogin_AccountNotExist);
      result->set_msg(MsgTip::UserLogin_AccountNotExist);
      return result;
    }
    LOGD("[account_server.handleUserLogin] user account is exist");

    //获得加密后password
    string encrypt_password = CommonUtils::EncryptPwd(account, password, userAccount.getPwdSalt());
    if (encrypt_password.empty())
    {
      // 更新失败次数
      result->set_code(ResultCode::UserLogin_PasswordInitFail);
      result->set_msg(MsgTip::UserLogin_PasswordInitFail);
      return result;
    }
    LOGD("[account_server.handleUserLogin] get user encrypt password success");

    //查询密码是否正确
    string db_password = userAccount.getPassword();
    if (db_password != encrypt_password)
    {
      result->set_code(ResultCode::UserLogin_PasswordError);
      result->set_msg(MsgTip::UserLogin_PasswordError);
      return result;
    }
    LOGD("[account_server.handleUserLogin] user input password is right");

    //获取用户UID
    int uid = userAccount.getUid();

    //生成Token、refreshToken、Token过期时间
    string token;
    int32_t tokenEndTime;
    CommonUtils::GenToken(uid, account, token, tokenEndTime);
    string refreshToken;
    CommonUtils::GenRefreshToken(uid, account, refreshToken);
    LOGD("[account_server.handleUserLogin] create user's new token success");

    //更新Token到redis
    if (!login_redis.updateToken(uid, token, refreshToken))
    {
      result->set_code(ResultCode::UserLogin_UpdateTokenFail);
      result->set_msg(MsgTip::UserLogin_UpdateTokenFail);
      return result;
    }
    LOGD("[account_server.handleUserLogin] update user's token to redis success");

    //返回Token
    result->set_code(ResultCode::SUCCESS);
    Json::Value root;
    root["token"] = token;
    root["refresh_token"] = refreshToken;
    root["token_expiration_time"] = tokenEndTime;
    Json::FastWriter fw;
    result->set_data(fw.write(root));
    return result;
  };

  /**
  用户模块-手机号注册
  
  入口参数
  account： 	      用户账号（手机号）
  password：        用户密码（明文）
  
  出口参数：
  token：           用户Token（用来接口请求）
  refresh_token：   用户Token（用来刷新Token）
  **/
  CodeReply *handleUserSign(string account, string password)
  {
    CodeReply *result = new CodeReply();

    LOGD("[account_server.handleUserSign] user sign in:" + account);
    LoginDatabase login_db;
    LoginRedis login_redis;

    //查询账号是否存在
    if (login_db.isHadSign(account))
    {
      result->set_code(ResultCode::UserSign_AccountHadExist);
      result->set_msg(MsgTip::UserSign_AccountHadExist);
      return result;
    }
    LOGD("[account_server.handleUserSign] useraccount not exist");

    //生成密码初始化的随机盐
    string pwdSalt = CommonUtils::GenPwdSalt();
    if (pwdSalt.empty())
    {
      result->set_code(ResultCode::UserSign_CreatePwdSaltFail);
      result->set_msg(MsgTip::UserSign_CreatePwdSaltFail);
      return result;
    }
    LOGD("[account_server.handleUserSign] create salt success:" + pwdSalt);

    //获得加密后password
    string encrypt_password = CommonUtils::EncryptPwd(account, password, pwdSalt);
    if (encrypt_password.empty())
    {
      result->set_code(ResultCode::UserSign_PasswordInitFail);
      result->set_msg(MsgTip::UserSign_PasswordInitFail);
      return result;
    }
    LOGD("[account_server.handleUserSign] get enc password success");

    //新增用户，插入账号信息到数据库
    if (!login_db.signAccount(account, encrypt_password, pwdSalt))
    {
      result->set_code(ResultCode::UserSign_CreateAccountFail);
      result->set_msg(MsgTip::UserSign_CreateAccountFail);
      return result;
    }
    LOGD("[account_server.handleUserSign] insert account into db success");

    //获得用户信息
    UserAccount userAccount = login_db.getUserAccount(account);
    if (userAccount.getUid() <= 0)
    {
      result->set_code(ResultCode::UserSign_GetAccountInfoFail);
      result->set_msg(MsgTip::UserSign_GetAccountInfoFail);
      return result;
    }
    LOGD("[account_server.handleUserSign] get account info success");

    //获得用户UID
    int uid = userAccount.getUid();

    //生成Token、refreshToken、Token过期时间
    string token;
    int32_t tokenEndTime;
    CommonUtils::GenToken(uid, account, token, tokenEndTime);
    string refreshToken;
    CommonUtils::GenRefreshToken(uid, account, refreshToken);
    LOGD("[account_server.handleUserLogin] create user's new token success");

    //更新Token到redis
    if (!login_redis.updateToken(uid, token, refreshToken))
    {
      result->set_code(ResultCode::UserSign_CreateSeesionFail);
      result->set_msg(MsgTip::UserSign_CreateSeesionFail);
      return result;
    }
    LOGD("[account_server.handleUserSign] update token into redis success");

    //返回Token
    result->set_code(ResultCode::SUCCESS);
    Json::Value root;
    root["token"] = token;
    root["refresh_token"] = refreshToken;
    root["token_expiration_time"] = tokenEndTime;
    Json::FastWriter fw;
    result->set_data(fw.write(root));
    return result;
  };

  /**
  todo 功能模块，添加分类
  **/
  CodeReply *handleAddCategory(string title, int uid, string token)
  {
    LOGD("[account_server.handleAddCategory] user addCategory in:" + title);
    // 1. 首先检查是否连接
    LoginCore loginCore;
    CodeReply *connectResult = loginCore.handleUserCheckConnect(token);
    CodeReply *result = new CodeReply();
    if (connectResult->code() != ResultCode::SUCCESS)
    {
      LOGD("[account_server.handleAddCategory] user is not connected, addCategory in:" + title);
      return connectResult;
    }

    LoginDatabase login_db;
    LoginRedis login_redis;

    // 添加分类到数据库，内部会校验
    if (!login_db.addCategory(title, uid))
    {
      result->set_code(ResultCode::AddCategory_InsertDBFail);
      result->set_msg(MsgTip::AddCategory_InsertDBFail);
      LOGD("[account_server.handleAddCategory] insert category into db fail");
      return result;
    }
    LOGD("[account_server.handleAddCategory] insert category into db success");

    //获得用户信息
    Category category = login_db.getCategory(title, uid);
    if (category.getCid() <= 0)
    {
      result->set_code(ResultCode::AddCategory_InsertDBFail);
      result->set_msg(MsgTip::AddCategory_InsertDBFail);
      return result;
    }
    LOGD("[account_server.handleAddCategory] get category info success");

    //返回Token
    result->set_code(ResultCode::SUCCESS);
    Json::Value root;
    root["cid"] = category.getCid();
    root["title"] = category.getTitle();
    root["uid"] = category.getUid();
    Json::FastWriter fw;
    result->set_data(fw.write(root));
    return result;
  };

  CodeReply *handleFetchCategoryList(int uid, string token)
  {
    LOGD("[account_server.handleFetchCategoryList] user handleFetchCategoryList ");
    // 1. 首先检查是否连接
    LoginCore loginCore;
    CodeReply *connectResult = loginCore.handleUserCheckConnect(token);
    CodeReply *result = new CodeReply();
    if (connectResult->code() != ResultCode::SUCCESS)
    {
      LOGD("[account_server.handleFetchCategoryList] user is not connected, handleFetchCategoryList in:");
      return connectResult;
    }

    LoginDatabase login_db;
    LoginRedis login_redis;

    // 添加分类到数据库，内部会校验
    std::vector<Category> categoryList = login_db.getCategoryList(uid);
    LOGD("[account_server.handleFetchCategoryList] get category info success");
    result->set_code(ResultCode::SUCCESS);

    {
      Json::Value root;
      Json::Value list;
      int size = categoryList.size();
      root["count"] = size;

      for (int i = 0; i < size; i++)
      {
        Json::Value item;
        item["uid"] = uid;
        item["title"] = categoryList[i].getTitle();
        item["cid"] = categoryList[i].getCid();
        list.append(item);
      }
      root["data"] = list;
      Json::FastWriter fw;
      result->set_data(fw.write(root));
    }

    return result;
  };

  CodeReply *handleFetchTodoList(int uid, int cid, string token)
  {
    LOGD("[account_server.handleFetchTodoList] user handleFetchTodoList ");
    // 1. 首先检查是否连接
    LoginCore loginCore;
    CodeReply *connectResult = loginCore.handleUserCheckConnect(token);
    CodeReply *result = new CodeReply();
    if (connectResult->code() != ResultCode::SUCCESS)
    {
      LOGD("[account_server.handleFetchTodoList] user is not connected, handleFetchTodoList in:");
      return connectResult;
    }

    LoginDatabase login_db;
    LoginRedis login_redis;

    // 添加分类到数据库，内部会校验
    std::vector<Todo> todoList = login_db.getTodoList(uid, cid);
    LOGD("[account_server.handleFetchTodoList] get todoList info success");
    result->set_code(ResultCode::SUCCESS);

    {
      Json::Value root;
      Json::Value list;
      int size = todoList.size();
      root["count"] = size;

      for (int i = 0; i < size; i++)
      {
        Json::Value item;
        item["uid"] = uid;
        item["tid"] = todoList[i].getTid();
        item["content"] = todoList[i].getContent();
        item["status"] = todoList[i].getStatus();
        item["cid"] = cid;
        list.append(item);
      }
      root["data"] = list;
      Json::FastWriter fw;
      result->set_data(fw.write(root));
    }

    return result;
  };

  /**
 * 添加todo
 * */
  CodeReply *handleAddTodo(string content, int cid, string token)
  {
    LOGD("[account_server.handleAddTodo] user addTodo in:" + content);
    // 1. 首先检查是否连接
    LoginCore loginCore;
    CodeReply *connectResult = loginCore.handleUserCheckConnect(token);
    CodeReply *result = new CodeReply();
    if (connectResult->code() != ResultCode::SUCCESS)
    {
      LOGD("[account_server.handleAddTodo] user is not connected, addTodo in:" + content);
      return connectResult;
    }

    LoginDatabase login_db;
    LoginRedis login_redis;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      result->set_code(ResultCode::CheckConnect_TokenNotValid);
      result->set_msg(MsgTip::CheckConnect_TokenNotValid);
      return result;
    }
    LOGD("[account_server.handleAddTodo] user token decrypt success");

    std::vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      result->set_code(ResultCode::CheckConnect_TokenNotValid);
      result->set_msg(MsgTip::CheckConnect_TokenNotValid);
      return result;
    }
    LOGD("[account_server.handleAddTodo] get token info success");

    //获取Token过期时间
    string token_end_time = vToken[4];
    int end_time = CommonUtils::getIntByString(token_end_time);

    //获得账号UID
    string str_uid = vToken[0];
    int uid = CommonUtils::getIntByString(str_uid);

    // 添加分类到数据库，内部会校验
    int tid = -1;
    tid = login_db.addTodo(content, cid, uid);
    if (tid < 1)
    {
      result->set_code(ResultCode::AddTodo_InsertDBFail);
      result->set_msg(MsgTip::AddTodo_InsertDBFail);
      LOGD("[account_server.handleAddTodo] insert todos into db fail");
      return result;
    }
    LOGD("[account_server.handleAddTodo] insert todos into db success");

    //获得用户信息
    Todo todoItem = login_db.getTodo(tid);
    if (todoItem.getTid() <= 0)
    {
      result->set_code(ResultCode::AddTodo_InsertDBFail);
      result->set_msg(MsgTip::AddTodo_InsertDBFail);
      return result;
    }
    LOGD("[account_server.handleAddTodo] get todo info success");

    //返回Token
    result->set_code(ResultCode::SUCCESS);
    Json::Value root;
    root["tid"] = todoItem.getTid();
    root["content"] = todoItem.getContent();
    root["cid"] = todoItem.getCid();
    root["status"] = todoItem.getStatus();
    Json::FastWriter fw;
    result->set_data(fw.write(root));
    return result;
  };

  CodeReply *handleUpdateTodo(int tid, int status, string token)
  {
    LOGD("[account_server.handleUpdateTodo] user handleUpdateTodo in");
    // 1. 首先检查是否连接
    LoginCore loginCore;
    CodeReply *connectResult = loginCore.handleUserCheckConnect(token);
    CodeReply *result = new CodeReply();
    if (connectResult->code() != ResultCode::SUCCESS)
    {
      LOGD("[account_server.handleUpdateTodo] user is not connected, handleUpdateTodo i:");
      return connectResult;
    }

    LoginDatabase login_db;
    LoginRedis login_redis;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      result->set_code(ResultCode::CheckConnect_TokenNotValid);
      result->set_msg(MsgTip::CheckConnect_TokenNotValid);
      return result;
    }
    LOGD("[account_server.handleUpdateTodo] user token decrypt success");

    std::vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      result->set_code(ResultCode::CheckConnect_TokenNotValid);
      result->set_msg(MsgTip::CheckConnect_TokenNotValid);
      return result;
    }
    LOGD("[account_server.handleUpdateTodo] get token info success");

    //获取Token过期时间
    string token_end_time = vToken[4];
    int end_time = CommonUtils::getIntByString(token_end_time);

    //获得账号UID
    string str_uid = vToken[0];
    int uid = CommonUtils::getIntByString(str_uid);

    // 添加分类到数据库，内部会校验

    Todo todoItem = login_db.updateTodoStatus(tid, status);
    LOGD("[account_server.handleUpdateTodo] insert todos into db success");

    if (todoItem.getTid() <= 0)
    {
      result->set_code(ResultCode::UpdateTodoStatus_Fail);
      result->set_msg(MsgTip::UpdateTodoStatus_Fail);
      return result;
    }
    LOGD("[account_server.handleUpdateTodo] get todo info success");

    //返回Token
    result->set_code(ResultCode::SUCCESS);
    Json::Value root;
    root["tid"] = todoItem.getTid();
    root["content"] = todoItem.getContent();
    root["cid"] = todoItem.getCid();
    root["status"] = todoItem.getStatus();
    Json::FastWriter fw;
    result->set_data(fw.write(root));
    return result;
  };

  /**
  用户模块-退出登录
  
  入口参数
  token： 	        用户Token
  **/
  CodeReply *handleUserLogout(string token)
  {
    CodeReply *result = new CodeReply();

    LOGD("[account_server.handleUserLogout] user logout in:" + token);

    LoginRedis login_redis;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      result->set_code(ResultCode::UserLogout_TokenNotValid);
      result->set_msg(MsgTip::UserLogout_TokenNotValid);
      return result;
    }
    LOGD("[account_server.handleUserLogout] user token decrypt success");

    //解析Token，获取用户信息
    vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      result->set_code(ResultCode::UserLogout_TokenNotValid);
      result->set_msg(MsgTip::UserLogout_TokenNotValid);
      return result;
    }
    LOGD("[account_server.handleUserLogout] get token info success");

    //获得账号UID
    int uid = CommonUtils::getIntByString(vToken[0]);

    //token是否正确
    if (!login_redis.isTokenRight(uid, token))
    {
      result->set_code(ResultCode::UserLogout_TokenNotExist);
      result->set_msg(MsgTip::UserLogout_TokenNotExist);
      return result;
    }
    LOGD("[account_server.handleUserLogout] user token is right");

    //清除token
    if (!login_redis.cleanToken(uid))
    {
      result->set_code(ResultCode::UserLogout_UpdateSessionFail);
      result->set_msg(MsgTip::UserLogout_UpdateSessionFail);
      return result;
    }
    LOGD("[account_server.handleUserLogout] clean user token in redis success");

    result->set_code(ResultCode::SUCCESS);
    return result;
  };

  /**
  用户模块-检查连接状态
  
  入口参数
  token： 	        用户Token
  **/
  CodeReply *handleUserCheckConnect(string token)
  {
    CodeReply *result = new CodeReply();

    LOGD("[account_server.handleUserCheckConnect] check connect in:" + token);

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      result->set_code(ResultCode::CheckConnect_TokenNotValid);
      result->set_msg(MsgTip::CheckConnect_TokenNotValid);
      return result;
    }
    LOGD("[account_server.handleUserCheckConnect] user token decrypt success");

    std::vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      result->set_code(ResultCode::CheckConnect_TokenNotValid);
      result->set_msg(MsgTip::CheckConnect_TokenNotValid);
      return result;
    }
    LOGD("[account_server.handleUserCheckConnect] get token info success");

    //获取Token过期时间
    string token_end_time = vToken[4];
    int end_time = CommonUtils::getIntByString(token_end_time);

    //获得账号UID
    string str_uid = vToken[0];
    int uid = CommonUtils::getIntByString(str_uid);

    //获取redis中用户Token
    LoginRedis login_redis;
    if (token != login_redis.getUserToken(uid))
    {
      result->set_code(ResultCode::CheckConnect_AccountTokenNotEqual);
      result->set_msg(MsgTip::CheckConnect_AccountTokenNotEqual);
      return result;
    }
    LOGD("[account_server.handleUserCheckConnect] user token in redis is match");

    //判断Token是否过期
    if (isTimeExpired(end_time))
    {
      result->set_code(ResultCode::CheckConnect_TokenHadExpire);
      result->set_msg(MsgTip::CheckConnect_TokenHadExpire);
      return result;
    }
    LOGD("[account_server.handleUserCheckConnect] user token is in time limit");

    result->set_code(ResultCode::SUCCESS);
    return result;
  };

  /**
  用户模块-刷新用户Token
  
  入口参数
  token：           用户Token（旧）
  refresh_token：   用户Token（旧）
  
  出口参数：
  token：           用户Token（新）
  refresh_token：   用户Token（新）
  **/
  CodeReply *handleRefreshToken(string token, string refreshToken)
  {
    CodeReply *result = new CodeReply();

    LOGD("[account_server.handleRefreshToken] refresh token in:" + token);

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      result->set_code(ResultCode::RefreshToken_TokenNotValid);
      result->set_msg(MsgTip::RefreshToken_TokenNotValid);
      return result;
    }

    std::vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      result->set_code(ResultCode::RefreshToken_TokenNotValid);
      result->set_msg(MsgTip::RefreshToken_TokenNotValid);
      return result;
    }
    LOGD("[account_server.handleRefreshToken] user token is right");

    //解密refreshToken
    string decodeRefreshToken = CommonUtils::DecryptToken(refreshToken);
    if (decodeRefreshToken.empty())
    {
      result->set_code(ResultCode::RefreshToken_RefreshTokenNotValid);
      result->set_msg(MsgTip::RefreshToken_RefreshTokenNotValid);
      return result;
    }

    std::vector<string> vRefreshToken;
    CommonUtils::SplitString(decodeRefreshToken, vRefreshToken, ":");
    if (vRefreshToken.size() != 5)
    {
      result->set_code(ResultCode::RefreshToken_RefreshTokenNotValid);
      result->set_msg(MsgTip::RefreshToken_RefreshTokenNotValid);
      return result;
    }
    LOGD("[account_server.handleRefreshToken] user refresh token is right");

    //对比token与refreshToken中的UID是否一致
    if (vToken[0] != vRefreshToken[0])
    {
      result->set_code(ResultCode::RefreshToken_TUidARTUidNotEqual);
      result->set_msg(MsgTip::RefreshToken_TUidARTUidNotEqual);
      return result;
    }
    LOGD("[account_server.handleRefreshToken] user token's uid and refresh token's uid is match");

    //对比refreshToken是否与redis中的一致
    int uid = CommonUtils::getIntByString(vRefreshToken[0]);
    LoginRedis login_redis;
    string redis_refresh_token = login_redis.getUserRefreshToken(uid);
    if (refreshToken != redis_refresh_token)
    {
      result->set_code(ResultCode::RefreshToken_RefreshTokenCacheNotEqual);
      result->set_msg(MsgTip::RefreshToken_RefreshTokenCacheNotEqual);
      return result;
    }
    LOGD("[account_server.handleRefreshToken] user refresh token in redis is match");

    //判断refreshToken是否过期
    string token_end_time = vRefreshToken[4];
    int end_time = CommonUtils::getIntByString(token_end_time);
    if (isTimeExpired(end_time))
    {
      result->set_code(ResultCode::RefreshToken_RefreshTokenHadExpire);
      result->set_msg(MsgTip::RefreshToken_RefreshTokenHadExpire);
      return result;
    }
    LOGD("[account_server.handleRefreshToken] user refresh token in time limit");

    //生成Token、refreshToken、Token过期时间
    string account = vToken[1];
    string new_token;
    int32_t tokenEndTime;
    CommonUtils::GenToken(uid, account, new_token, tokenEndTime);
    string new_refreshToken;
    CommonUtils::GenRefreshToken(uid, account, new_refreshToken);
    LOGD("[account_server.handleRefreshToken] create user new token success");

    //更新token、refreshToken到redis
    if (!login_redis.updateToken(uid, new_token, new_refreshToken))
    {
      result->set_code(ResultCode::RefreshToken_CreateSeesionFail);
      result->set_msg(MsgTip::RefreshToken_CreateSeesionFail);
      return result;
    }
    LOGD("[account_server.handleRefreshToken] update user token redis success");

    //返回token、refreshToken
    result->set_code(ResultCode::SUCCESS);
    Json::Value root;
    root["token"] = new_token;
    root["refresh_token"] = new_refreshToken;
    root["token_expiration_time"] = tokenEndTime;
    Json::FastWriter fw;
    result->set_data(fw.write(root));
    return result;
  }

private:
  /**
  * 判断时间是否过期
  **/
  bool isTimeExpired(int end_time)
  {
    time_t now_time = time(NULL);
    return now_time > end_time;
  }
};

class AccountServiceImpl final : public Account::Service
{

  /**
   * 用户模块-手机号登录 
   **/
  Status requestUserLogin(ServerContext *context, const LoginRequest *request,
                          CodeReply *reply) override
  {

    string account = request->account();
    string password = request->password();

    LogMBean log_bean("requestUserLogin");

    bool isParamValid = true;
    string error_msg;

    //校验用户账号
    if (!ParamUtils::CheckAccountValid(account, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("account is not valid");
    };

    //校验用户密码
    if (isParamValid && !ParamUtils::CheckPassMwordValid(password, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("password is not valid");
    };

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      CodeReply *result = loginCore.handleUserLogin(account, password);
      reply->set_code(result->code());
      reply->set_msg(result->msg());
      reply->set_data(result->data());
      delete result;
    }

    //校验返回数据的合法性
    string msg;
    if (!ParamUtils::CheckBackDataValid(reply->data(), msg))
    {
      reply->set_code(ResultCode::RetrunDataInvalid);
      reply->set_msg(msg);
      reply->set_data("");
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("account", account);
    log_bean.addParam("password", password);
    LOGM(log_bean);
    return Status::OK;
  }

  /**
   * 用户模块-手机号注册
   **/
  Status requestUserSign(ServerContext *context, const SignRequest *request,
                         CodeReply *reply) override
  {

    LogMBean log_bean("requestUserSign");

    string account = request->account();
    string password = request->password();

    bool isParamValid = true;
    string error_msg;

    //校验用户账号
    if (!ParamUtils::CheckAccountValid(account, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("account is not valid");
    };

    //校验用户密码
    if (isParamValid && !ParamUtils::CheckPassMwordValid(password, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("password is not valid");
    };

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      CodeReply *result = loginCore.handleUserSign(account, password);
      reply->set_code(result->code());
      reply->set_msg(result->msg());
      reply->set_data(result->data());
      delete result;
    }

    //校验返回数据的合法性
    string msg;
    if (!ParamUtils::CheckBackDataValid(reply->data(), msg))
    {
      reply->set_code(ResultCode::RetrunDataInvalid);
      reply->set_msg(msg);
      reply->set_data("");
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("account", account);
    log_bean.addParam("password", password);
    LOGM(log_bean);

    return Status::OK;
  }

  /**
   * 用户模块-用户退出登录
   **/
  Status requestLogout(ServerContext *context, const LogoutRequest *request,
                       CodeReply *reply) override
  {

    LogMBean log_bean("requestLogout");

    string token = request->token();

    //执行请求
    LoginCore loginCore;
    CodeReply *result = loginCore.handleUserLogout(token);
    reply->set_code(result->code());
    reply->set_msg(result->msg());
    reply->set_data(result->data());
    delete result;

    //校验返回数据的合法性
    string msg;
    if (!ParamUtils::CheckBackDataValid(reply->data(), msg))
    {
      reply->set_code(ResultCode::RetrunDataInvalid);
      reply->set_msg(msg);
      reply->set_data("");
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("token", token);
    LOGM(log_bean);

    return Status::OK;
  }

  /**
   * 用户模块-检查用户连接状态
   **/
  Status checkConnect(ServerContext *context, const ConnectRequest *request,
                      CodeReply *reply) override
  {

    LogMBean log_bean("checkConnect");

    string token = request->token();

    bool isParamValid = true;
    string error_msg;

    //校验用户token
    if (!ParamUtils::CheckStringValid(token, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("token is empty");
    };

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      CodeReply *result = loginCore.handleUserCheckConnect(token);
      reply->set_code(result->code());
      reply->set_msg(result->msg());
      reply->set_data(result->data());
      delete result;
    }

    //校验返回数据的合法性
    string msg;
    if (!ParamUtils::CheckBackDataValid(reply->data(), msg))
    {
      reply->set_code(ResultCode::RetrunDataInvalid);
      reply->set_msg(msg);
      reply->set_data("");
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("token", token);
    LOGM(log_bean);

    return Status::OK;
  }

  /**
   * 用户模块-刷新用户Token，更新refreshToken
   **/
  Status refreshToken(ServerContext *context, const RefreshRequest *request,
                      CodeReply *reply) override
  {
    LogMBean log_bean("refreshToken");

    string token = request->token();
    string refreshToken = request->refresh_token();

    bool isParamValid = true;
    string error_msg;

    //校验用户token
    if (!ParamUtils::CheckStringValid(token, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("token is empty");
    };

    //校验用户refresh_token
    if (!ParamUtils::CheckStringValid(refreshToken, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("refresh_token is empty");
    };

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      CodeReply *result = loginCore.handleRefreshToken(token, refreshToken);
      reply->set_code(result->code());
      reply->set_msg(result->msg());
      reply->set_data(result->data());
      delete result;
    }

    //校验返回数据的合法性
    string msg;
    if (!ParamUtils::CheckBackDataValid(reply->data(), msg))
    {
      reply->set_code(ResultCode::RetrunDataInvalid);
      reply->set_msg(msg);
      reply->set_data("");
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("token", token);
    log_bean.addParam("refresh_token", refreshToken);
    LOGM(log_bean);

    return Status::OK;
  }
  Status requestAddCategory(ServerContext *context, const AddCategoryRequest *request,
                            CodeReply *reply) override
  {
    LogMBean log_bean("requestAddCategory");

    string token = request->token();
    string title = request->title();
    bool isParamValid = true;
    string error_msg;

    //校验用户token
    if (!ParamUtils::CheckStringValid(token, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("token is empty");
    };

    //校验用户token
    if (!ParamUtils::CheckStringValid(title, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("title is empty");
    };

    LoginRedis login_redis;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      reply->set_code(ResultCode::UserLogout_TokenNotValid);
      reply->set_msg(MsgTip::UserLogout_TokenNotValid);
      return Status::OK;
    }
    LOGD("[account_server.requestAddCategory] user token decrypt success");

    //解析Token，获取用户信息
    vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      reply->set_code(ResultCode::UserLogout_TokenNotValid);
      reply->set_msg(MsgTip::UserLogout_TokenNotValid);
      return Status::OK;
    }
    LOGD("[account_server.requestAddCategory] get token info success");

    //获得账号UID
    int uid = CommonUtils::getIntByString(vToken[0]);

    //token是否正确
    if (!login_redis.isTokenRight(uid, token))
    {
      reply->set_code(ResultCode::UserLogout_TokenNotExist);
      reply->set_msg(MsgTip::UserLogout_TokenNotExist);
      return Status::OK;
    }
    LOGD("[account_server.requestAddCategory] user token is right");

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      CodeReply *result = loginCore.handleAddCategory(title, uid, token);
      reply->set_code(result->code());
      reply->set_msg(result->msg());
      reply->set_data(result->data());
      delete result;
    }

    //校验返回数据的合法性
    string msg;
    if (!ParamUtils::CheckBackDataValid(reply->data(), msg))
    {
      reply->set_code(ResultCode::RetrunDataInvalid);
      reply->set_msg(msg);
      reply->set_data("");
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("title", title);
    log_bean.addParam("uid", uid);
    LOGM(log_bean);

    return Status::OK;
  }

  Status requestFetchCategory(ServerContext *context, const FetchCategoryRequest *request,
                              CodeReply *reply) override
  {
    LogMBean log_bean("requestFetchCategory");

    string token = request->token();
    bool isParamValid = true;
    string error_msg;

    //校验用户token
    if (!ParamUtils::CheckStringValid(token, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("token is empty");
    };

    LoginRedis login_redis;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      reply->set_code(ResultCode::UserLogout_TokenNotValid);
      reply->set_msg(MsgTip::UserLogout_TokenNotValid);
      return Status::OK;
    }
    LOGD("[account_server.requestFetchCategory] user token decrypt success");

    //解析Token，获取用户信息
    vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      reply->set_code(ResultCode::UserLogout_TokenNotValid);
      reply->set_msg(MsgTip::UserLogout_TokenNotValid);
      return Status::OK;
    }
    LOGD("[account_server.requestFetchCategory] get token info success");

    //获得账号UID
    int uid = CommonUtils::getIntByString(vToken[0]);

    //token是否正确
    if (!login_redis.isTokenRight(uid, token))
    {
      reply->set_code(ResultCode::UserLogout_TokenNotExist);
      reply->set_msg(MsgTip::UserLogout_TokenNotExist);
      return Status::OK;
    }
    LOGD("[account_server.requestFetchCategory] user token is right");

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      CodeReply *result = loginCore.handleFetchCategoryList(uid, token);
      reply->set_code(result->code());
      reply->set_msg(result->msg());
      reply->set_data(result->data());
      delete result;
    }

    //校验返回数据的合法性
    string msg;
    if (!ParamUtils::CheckBackDataValid(reply->data(), msg))
    {
      reply->set_code(ResultCode::RetrunDataInvalid);
      reply->set_msg(msg);
      reply->set_data("");
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("uid", uid);
    LOGM(log_bean);

    return Status::OK;
  }

  Status requestAddTodo(ServerContext *context, const AddTodoRequest *request,
                        CodeReply *reply) override
  {
    LogMBean log_bean("requestAddTodo");

    string token = request->token();
    string content = request->content();
    int cid = request->cid();

    bool isParamValid = true;
    string error_msg;

    //校验用户token
    if (!ParamUtils::CheckStringValid(token, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("token is empty");
    };

    //校验title
    if (!ParamUtils::CheckStringValid(content, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("content is empty");
    };

    LoginRedis login_redis;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      reply->set_code(ResultCode::UserLogout_TokenNotValid);
      reply->set_msg(MsgTip::UserLogout_TokenNotValid);
      return Status::OK;
    }
    LOGD("[account_server.requestAddTodo] user token decrypt success");

    //解析Token，获取用户信息
    vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      reply->set_code(ResultCode::UserLogout_TokenNotValid);
      reply->set_msg(MsgTip::UserLogout_TokenNotValid);
      return Status::OK;
    }
    LOGD("[account_server.requestAddTodo] get token info success");

    //获得账号UID
    int uid = CommonUtils::getIntByString(vToken[0]);

    //token是否正确
    if (!login_redis.isTokenRight(uid, token))
    {
      reply->set_code(ResultCode::UserLogout_TokenNotExist);
      reply->set_msg(MsgTip::UserLogout_TokenNotExist);
      return Status::OK;
    }
    LOGD("[account_server.requestAddTodo] user token is right");

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      CodeReply *result = loginCore.handleAddTodo(content, cid, token);
      reply->set_code(result->code());
      reply->set_msg(result->msg());
      reply->set_data(result->data());
      delete result;
    }

    //校验返回数据的合法性
    string msg;
    if (!ParamUtils::CheckBackDataValid(reply->data(), msg))
    {
      reply->set_code(ResultCode::RetrunDataInvalid);
      reply->set_msg(msg);
      reply->set_data("");
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("content", content);
    log_bean.addParam("uid", uid);
    LOGM(log_bean);

    return Status::OK;
  }

  Status requestUpdateTodo(ServerContext *context, const UpdateTodoRequest *request,
                           CodeReply *reply) override
  {
    LogMBean log_bean("requestUpdateTodo");

    string token = request->token();
    int status = request->status();
    int tid = request->tid();

    bool isParamValid = true;
    string error_msg;

    //校验用户token
    if (!ParamUtils::CheckStringValid(token, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("token is empty");
    };

    //校验title
    if (tid < 1)
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("tid is empty");
    };

    LoginRedis login_redis;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      reply->set_code(ResultCode::UserLogout_TokenNotValid);
      reply->set_msg(MsgTip::UserLogout_TokenNotValid);
      return Status::OK;
    }
    LOGD("[account_server.requestUpdateTodo] user token decrypt success");

    //解析Token，获取用户信息
    vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      reply->set_code(ResultCode::UserLogout_TokenNotValid);
      reply->set_msg(MsgTip::UserLogout_TokenNotValid);
      return Status::OK;
    }
    LOGD("[account_server.requestUpdateTodo] get token info success");

    //获得账号UID
    int uid = CommonUtils::getIntByString(vToken[0]);

    //token是否正确
    if (!login_redis.isTokenRight(uid, token))
    {
      reply->set_code(ResultCode::UserLogout_TokenNotExist);
      reply->set_msg(MsgTip::UserLogout_TokenNotExist);
      return Status::OK;
    }
    LOGD("[account_server.requestUpdateTodo] user token is right");

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      CodeReply *result = loginCore.handleUpdateTodo(tid, status, token);
      reply->set_code(result->code());
      reply->set_msg(result->msg());
      reply->set_data(result->data());
      delete result;
    }

    //校验返回数据的合法性
    string msg;
    if (!ParamUtils::CheckBackDataValid(reply->data(), msg))
    {
      reply->set_code(ResultCode::RetrunDataInvalid);
      reply->set_msg(msg);
      reply->set_data("");
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("status", status);
    log_bean.addParam("tid", tid);
    LOGM(log_bean);

    return Status::OK;
  }

  Status requestFetchTodoList(ServerContext *context, const FetchTodoListRequest *request,
                              CodeReply *reply) override
  {
    LogMBean log_bean("requestFetchTodoList");

    string token = request->token();
    int cid = request->cid();
    bool isParamValid = true;
    string error_msg;

    //校验用户token
    if (!ParamUtils::CheckStringValid(token, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("token is empty");
    };

    LoginRedis login_redis;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      reply->set_code(ResultCode::UserLogout_TokenNotValid);
      reply->set_msg(MsgTip::UserLogout_TokenNotValid);
      return Status::OK;
    }
    LOGD("[account_server.requestAddCategory] user token decrypt success");

    //解析Token，获取用户信息
    vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      reply->set_code(ResultCode::UserLogout_TokenNotValid);
      reply->set_msg(MsgTip::UserLogout_TokenNotValid);
      return Status::OK;
    }
    LOGD("[account_server.requestAddCategory] get token info success");

    //获得账号UID
    int uid = CommonUtils::getIntByString(vToken[0]);

    //token是否正确
    if (!login_redis.isTokenRight(uid, token))
    {
      reply->set_code(ResultCode::UserLogout_TokenNotExist);
      reply->set_msg(MsgTip::UserLogout_TokenNotExist);
      return Status::OK;
    }
    LOGD("[account_server.requestAddCategory] user token is right");

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      CodeReply *result = loginCore.handleFetchTodoList(uid, cid, token);
      reply->set_code(result->code());
      reply->set_msg(result->msg());
      reply->set_data(result->data());
      delete result;
    }

    //校验返回数据的合法性
    string msg;
    if (!ParamUtils::CheckBackDataValid(reply->data(), msg))
    {
      reply->set_code(ResultCode::RetrunDataInvalid);
      reply->set_msg(msg);
      reply->set_data("");
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("uid", uid);
    log_bean.addParam("cid", cid);
    LOGM(log_bean);

    return Status::OK;
  }
};

void RunServer(manager::ServerConfig conf)
{

  std::string server_address(conf.getServerIpAndPort());
  AccountServiceImpl service;

  ServerBuilder builder;

  LOGD("[account_server.RunServer] begin load ssl key info");
  std::ifstream skey(conf.getSSLPathKey());
  std::string strServerKey((std::istreambuf_iterator<char>(skey)), std::istreambuf_iterator<char>());
  //std::cout << "key: " <<strServerKey << std::endl;
  std::ifstream sCrt(conf.getSSLPathCert());
  std::string strServerCert((std::istreambuf_iterator<char>(sCrt)), std::istreambuf_iterator<char>());
  //std::cout << "crt: " << strServerCert << std::endl;

  grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp = {strServerKey.c_str(), strServerCert.c_str()};

  grpc::SslServerCredentialsOptions ssl_opts;
  ssl_opts.pem_root_certs = "";
  ssl_opts.pem_key_cert_pairs.push_back(pkcp);
  std::shared_ptr<grpc::ServerCredentials> creds = grpc::SslServerCredentials(ssl_opts);
  LOGD("[account_server.RunServer] finish load ssl key info");
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, creds);
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  LOGD("[account_server.RunServer] Server listening on " + server_address);

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char **argv)
{

  manager::ServerConfig conf;
  //设置是否打印DEBUG信息
  LogUtil::setConsoleDebugInfo(conf.isConsoleDebugInfo());
  //根据配置文件初始化Redis
  Redis::getRedis()->connect(conf);
  //根据配置文件初始化DB
  Database::getDatabase()->connect(conf);
  //设置参与Token生成的AES算法的KEY
  CommonUtils::setAesKey(conf.getTokenAesKey());
  //设置Token过期时间
  // CommonUtils::setTokenTimeout(conf.getTokenTimeout());
  //设置RefreshToken过期时间
  CommonUtils::setRefreshTokenTimeout(conf.getRefreshTokenTimeout());
  
  RunServer(conf);
  return 0;
}
