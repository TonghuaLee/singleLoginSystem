
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <sstream>
#include <mysql/mysql.h>

#include "db_manager.h"
#include "source/cpp/utils/log_utils.h"
#include "source/cpp/utils/common_utils.h"

#define LOGD(msg) utils::LogUtil::LOGD(msg);
#define LOGW(msg) utils::LogUtil::LOGW(msg);
#define LOGI(msg) utils::LogUtil::LOGI(msg);
#define LOGE(msg) utils::LogUtil::LOGE(msg);

using namespace manager;
using namespace std;
using namespace my_model;
using namespace utils;

Database *Database::database = new Database();

Database *Database::getDatabase()
{
    return Database::database;
}

Database::Database()
{
}

/**
 * 数据库初始化
 * 私有方法，不对业务层开放
**/
void Database::connect(ServerConfig _conf)
{
    if (isHadInit)
    {
        LOGW("[db_manager.connect] database had init !");
        return;
    }
    isHadInit = true;

    // 根据配置文件打开数据库连接。如果数据库不存在，则创建。
    string msg;

    if (Database::db_base->connect(
            _conf.getMySqlHost(),
            _conf.getMySqlPort(),
            _conf.getMySqlDBName(),
            _conf.getMySqlUser(),
            _conf.getMySqlPassword(),
            _conf.getMySqlCharset(),
            msg) == 0)
    {
        LOGD("[db_manager.connect] db connect success !");
        //打开数据库成功，检查表是否存在
        Database::checkAndCreateTable(Database::TABLE_USER_ACCOUNT);
    }
    else
    {
        LOGE("[db_manager.connect] db connect fail");
        LOGD(msg);
    }
}

/**
 * 检查表是否存在
 * 私有方法，不对业务层开放
 **/
void Database::checkAndCreateTable(string tableName)
{
    //构建查询语句
    string str_sql_table = "SELECT table_name FROM information_schema.TABLES WHERE table_name ='" + tableName + "';";
    vector<string> coloumnsV;
    coloumnsV.push_back("table_name");
    if (!db_base->isExist(str_sql_table, coloumnsV))
    {
        string str_sql = "CREATE TABLE " + tableName + "(           \
            ID          INT PRIMARY KEY AUTO_INCREMENT      NOT NULL,   \
            ACCOUNT     CHAR(50)                            NOT NULL,   \
            PASSWORD    CHAR(128)                           NOT NULL,   \
            PWD_SALT    CHAR(32)                            NOT NULL    \
            );";

        if (db_base->createdbTable(str_sql))
        {
            LOGD("[db_manager.checkAndCreateTable] create tables success");
        }
        else
        {
            LOGE("[db_manager.checkAndCreateTable] create tables fail");
        }
    }
}

/***************************以下是对外的接口**************************/

/**
 * 添加用户数据到数据库
 **/
bool Database::addUserAccount(string account, string password, string pwdSalt)
{
    //参数判空
    if (account.empty() || password.empty() || pwdSalt.empty())
    {
        LOGW("[db_manager.addUserAccount] param is empty");
        return false;
    }

    //参数非法字符过滤
    filterIllegalKeyword(account);

    //这里保险再判断一层，判断账号是否已经存在
    if (Database::isUserExist(account))
    {
        LOGE("[db_manager.addUserAccount] account is exist");
        return false;
    }

    //执行插入数据操作
    string msg;
    Json::Value data = db_base->insertUserAccount(account, password, pwdSalt, msg);
    if (data["is_empty"].asBool() || CommonUtils::getIntByString(data["ID"].asString()) <= 0)
    {
        LOGE("[db_manager.addUserAccount] " + msg);
        return false;
    }
    return true;
}

/**
 * 判断用户是否存在
 **/
bool Database::isUserExist(string account)
{
    //参数判空
    if (account.empty())
    {
        LOGW("[db_manager.isUserExist] param is empty");
        return false;
    }

    //参数非法字符过滤
    filterIllegalKeyword(account);

    //执行查询数据操作
    UserAccount userAccount = queryUserAccountByAccount(account);
    return userAccount.getUid() > 0;
}

/**
 * 根据用户账号查询用户数据
 **/
UserAccount Database::queryUserAccountByAccount(string o_account)
{
    //参数判空
    if (o_account.empty())
    {
        LOGW("[db_manager.queryUserAccountByAccount] param is empty");
        return UserAccount(-1, "", "", "");
    }

    //参数非法字符过滤
    filterIllegalKeyword(o_account);

    //执行查询数据操作
    int uid = -1;
    string account;
    string password;
    string pwdSalt;
    string msg;
    Json::Value data = db_base->selectUserAccountByAccount(o_account, msg);

    Json::FastWriter fw;
    LOGD("[db_manager.queryUserAccountByAccount] query account info :" + fw.write(data));

    if (data["is_empty"].asBool())
    {
        LOGE("[db_manager.queryUserAccountByAccount] data is empty");
        return UserAccount(-1, "", "", "");
    }
    else
    {
        uid = CommonUtils::getIntByString(data["ID"].asString());
        account = data["ACCOUNT"].asString();
        password = data["PASSWORD"].asString();
        pwdSalt = data["PWD_SALT"].asString();
        if (uid < 0)
        {
            LOGE("[db_manager.queryUserAccountByAccount] can not find user = " + o_account);
            return UserAccount(-1, "", "", "");
        }
    }

    return UserAccount(uid, account, password, pwdSalt);
}

/**
 * 添加todo分类到数据库
 **/
bool Database::addCategory(string title, int uid)
{
    //参数判空
    if (title.empty() || uid < 1)
    {
        LOGW("[db_manager.addCategory] param is empty");
        return false;
    }

    //参数非法字符过滤
    filterIllegalKeyword(title);

    //执行插入数据操作
    string msg;
    Json::Value data = db_base->insertCategory(title, uid, msg);
    int cid = CommonUtils::getIntByString(data["ID"].asString());
    if (data["is_empty"].asBool() || cid <= 0)
    {
        LOGE("[db_manager.addCategory] " + msg);
        return false;
    }
    return true;
}

/**
 * 查询是否有内容相同的分类
 **/
Category Database::queryCategory(string o_title, int o_uid)
{
    //参数判空
    if (o_title.empty())
    {
        LOGW("[db_manager.queryCategory] param is empty");
        return Category(-1, "", -1);
    }

    //参数非法字符过滤
    filterIllegalKeyword(o_title);

    //执行查询数据操作
    int cid = -1;
    int uid = -1;
    string title;
    string msg;
    Json::Value data = db_base->selectCategory(o_title, o_uid, msg);

    Json::FastWriter fw;
    LOGD("[db_manager.queryCategory] query category info :" + fw.write(data));

    if (data["is_empty"].asBool())
    {
        LOGE("[db_manager.queryCategory] data is empty");
        return Category(-1, "", -1);
    }
    else
    {
        cid = CommonUtils::getIntByString(data["ID"].asString());
        uid = CommonUtils::getIntByString(data["UID"].asString());
        title = data["TITLE"].asString();
        if (cid < 0)
        {
            LOGE("[db_manager.queryCategory] can not find title = " + o_title);
            return Category(-1, "", -1);
        }
    }

    return Category(cid, title, uid);
}

std::vector<Category> Database::queryCategoryList(int o_uid)
{
    //执行查询数据操作
    int cid = -1;
    int uid = -1;
    string title;
    string msg;
    Json::Value data = db_base->selectCategoryList(o_uid, msg);

    Json::FastWriter fw;
    LOGD("[db_manager.queryCategoryList] query category info :" + fw.write(data));
    std::vector<Category> resultList;
    if (data["is_empty"].asBool())
    {
        LOGE("[db_manager.queryCategoryList] data is empty");
        return resultList;
    }
    else
    {
        Json::Value categoryList = data["data"];
        int size = 0;
        size = categoryList.size();
        LOGD("[db_manager.queryCategoryList] data size:" + size);

        for (int i = 0; i < size; i++)
        {
            Json::Value val_category = categoryList[i];
            int cid = CommonUtils::getIntByString(val_category["ID"].asString());
            string title = val_category["TITLE"].asString();
            if (cid > 0)
            {
                resultList.push_back(Category(cid, title, o_uid));
            }
        }
    }

    return resultList;
}

/**
 * 添加todo分类到数据库
 **/
int Database::addTodo(string content, int cid, int uid)
{
    //参数判空
    if (content.empty() || uid < 1)
    {
        LOGW("[db_manager.addTodo] param is empty");
        return -1;
    }

    //参数非法字符过滤
    filterIllegalKeyword(content);

    //执行插入数据操作
    string msg;
    Json::Value data = db_base->insertTodo(content, cid, uid, msg);
    int tid = CommonUtils::getIntByString(data["TID"].asString());
    if (data["is_empty"].asBool() || tid <= 0)
    {
        LOGE("[db_manager.addTodo] " + msg);
        return -1;
    }
    return tid;
}

/**
 * 根据用户账号查询用户数据
 **/
Todo Database::queryTodo(int o_tid)
{

    //执行查询数据操作
    int tid = -1;
    int cid = -1;
    int status = 0;
    int uid = 0;
    string content;
    string msg;
    Json::Value data = db_base->selectTodo(o_tid, msg);

    Json::FastWriter fw;
    LOGD("[db_manager.queryTodo] query todo info :" + fw.write(data));

    if (data["is_empty"].asBool())
    {
        LOGE("[db_manager.queryTodo] data is empty");
        return Todo(-1, "", -1, -1, 0);
    }
    else
    {
        tid = CommonUtils::getIntByString(data["TID"].asString());
        cid = CommonUtils::getIntByString(data["CID"].asString());
        uid = CommonUtils::getIntByString(data["UID"].asString());
        status = CommonUtils::getIntByString(data["STATUS"].asString());
        content = data["CONTENT"].asString();
        if (tid < 0)
        {
            LOGE("[db_manager.queryTodo] can not find tid");
            return Todo(-1, "", -1, -1, 0);
        }
    }

    return Todo(tid, content, uid, cid, status);
}

/**
 * 非法字符过滤
 **/
void Database::filterIllegalKeyword(string &source_word)
{
    // such as : select * from user_table where username='admin' and password='admin123'

    //select * from user_table where username='xxx' and password='xxx' or '1'='1'
    utils::CommonUtils::replaceAll(source_word, "'", "’");
    //select * from user_table where username='1\' and password='or 1=1;#';
    utils::CommonUtils::replaceAll(source_word, "\\", "");
}

std::vector<Todo> Database::queryTodoList(int o_uid, int o_cid)
{
    //执行查询数据操作
    int cid = -1;
    int uid = -1;
    string title;
    string msg;
    Json::Value data = db_base->selectTodoList(o_uid, o_cid, msg);

    Json::FastWriter fw;
    LOGD("[db_manager.queryTodoList] query todo info :" + fw.write(data));
    std::vector<Todo> resultList;
    if (data["is_empty"].asBool())
    {
        LOGE("[db_manager.queryTodoList] data is empty");
        return resultList;
    }
    else
    {
        Json::Value todoList = data["data"];
        int size = 0;
        size = todoList.size();
        LOGD("[db_manager.queryTodoList] data size:" + size);

        for (int i = 0; i < size; i++)
        {
            Json::Value val_todo = todoList[i];
            int tid = CommonUtils::getIntByString(val_todo["ID"].asString());
            string title = val_todo["CONTENT"].asString();
            int status = CommonUtils::getIntByString(val_todo["STATUS"].asString());

            if (tid > 0)
            {
                resultList.push_back(Todo(tid, title, o_uid, o_cid, status));
            }
        }
    }

    return resultList;
}

Todo Database::updateTodoStatus(int o_tid, int o_status)
{

    //执行查询数据操作
    int status = 0;
    int uid = 0;
    int cid = 0;
    string content;
    string msg;
    Json::Value data = db_base->updateTodoStatus(o_tid, o_status, msg);

    Json::FastWriter fw;
    LOGD("[db_manager.queryTodo] query todo info :" + fw.write(data));

    if (data["is_empty"].asBool())
    {
        LOGE("[db_manager.queryTodo] data is empty");
        return Todo(-1, "", -1, -1, 0);
    }
    else
    {
        cid = CommonUtils::getIntByString(data["CID"].asString());
        uid = CommonUtils::getIntByString(data["UID"].asString());
        status = CommonUtils::getIntByString(data["STATUS"].asString());
        content = data["CONTENT"].asString();
        if (cid < 0)
        {
            LOGE("[db_manager.queryTodo] can not find tid");
            return Todo(-1, "", -1, -1, 0);
        }
    }

    return Todo(o_tid, content, uid, cid, status);
}
