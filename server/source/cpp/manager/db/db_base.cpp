
#include "stdio.h"
#include "db_base.h"

#include "source/cpp/utils/log_utils.h"
#include "source/cpp/utils/common_utils.h"

#define LOGD(msg) utils::LogUtil::LOGD(msg);
#define LOGW(msg) utils::LogUtil::LOGW(msg);
#define LOGI(msg) utils::LogUtil::LOGI(msg);
#define LOGE(msg) utils::LogUtil::LOGE(msg);

ReadWriteLock *DBBase::rwlock = new ReadWriteLock();

DBBase::DBBase()
{
}

DBBase::~DBBase()
{
}

//初始化数据
int DBBase::connect(string host, int port, string Db, string user, string passwd, string charset, string &Msg)
{
       if (mysql_init(&mysql) == NULL)
       {
              Msg = "[db_base.connect] inital mysql handle error";
              return 1;
       }

       if (mysql_real_connect(&mysql, host.c_str(), user.c_str(), passwd.c_str(), Db.c_str(), port, NULL, 0) == NULL)
       {
              Msg = "[db_base.connect] Failed to connect to database: Error" +
                    host + user + passwd + Db;
              return 1;
       }
       if (mysql_set_character_set(&mysql, "UTF8") != 0)
       {
              Msg = "[db_base.connect] mysql_set_character_set Error";
              return 1;
       }
       return 0;
}

//创建表
bool DBBase::createdbTable(const std::string &query)
{
       int query_result = -1;
       rwlock->writeLock();
       try
       {
              query_result = mysql_query(&mysql, query.c_str());
       }
       catch (const std::exception &e)
       {
              rwlock->writeUnlock();
              return false;
       }
       rwlock->writeUnlock();

       if (0 == query_result)
       {
              return true;
       }
       DBBase::errorIntoMySQL();

       return false;
}

//查询数据
Json::Value DBBase::selectData(const char *SQL, string tableName, string &Msg)
{
       vector<string> columnsV = DBBase::findColumns(tableName, Msg);
       if (columnsV.empty())
       {
              Msg = "[db_base.selectData] select columns Error";
              return "";
       }
       return DBBase::selectData(SQL, columnsV, Msg);
}

//查询数据
Json::Value DBBase::selectData(const char *SQL, vector<string> columnsV, string &Msg)
{
       Json::Value root;
       root["is_empty"] = true;

       if (SQL == NULL)
       {
              Msg = "[db_base.selectData] sql is NULL";
              return root;
       }
       LOGD("[db_base.selectData] sql : " + (string)SQL);

       //加读锁
       rwlock->readLock();

       MYSQL_ROW m_row;
       MYSQL_RES *m_res;
       char sql[2048];
       sprintf(sql, SQL);

       if (mysql_query(&mysql, sql) != 0)
       {
              Msg = "[db_base.selectData] select ps_info Error";
              //释放读锁
              rwlock->readUnlock();
              return root;
       }
       m_res = mysql_store_result(&mysql);

       if (m_res == NULL)
       {
              Msg = "[db_base.selectData] select username Error";
              //释放读锁
              rwlock->readUnlock();
              mysql_free_result(m_res);
              return root;
       }

       while (m_row = mysql_fetch_row(m_res))
       {
              Json::Value data;
              for (uint i = 0; i < columnsV.size(); i++)
              {
                     data[columnsV[i]] = m_row[i];
              }
              root["data_array"].append(data);
              root["is_empty"] = false;
       }

       mysql_free_result(m_res);

       // Json::FastWriter fw;
       // LOGD(fw.write(root));

       //释放读锁
       rwlock->readUnlock();
       return root;
}

//查询表的键
vector<string> DBBase::findColumns(string tableName, string &Msg)
{
       //加读锁
       rwlock->readLock();

       vector<string> columns;
       MYSQL_ROW m_row;
       MYSQL_RES *m_res;
       string sql_columns_find = "show columns from " + tableName + ";";
       if (mysql_query(&mysql, sql_columns_find.c_str()) != 0)
       {
              Msg = "[db_base.findColumns] select columns Error";
              //释放读锁
              rwlock->readUnlock();
              return columns;
       }

       m_res = mysql_store_result(&mysql);

       if (m_res == NULL)
       {
              Msg = "[db_base.findColumns] select columns Error";
              //释放读锁
              rwlock->readUnlock();
              return columns;
       }

       string str("");
       while (m_row = mysql_fetch_row(m_res))
       {
              columns.push_back(m_row[0]);
       }
       mysql_free_result(m_res);

       //释放读锁
       rwlock->readUnlock();
       return columns;
}

//更新数据
int DBBase::updateData(const char *SQL, string &Msg)
{
       if (SQL == NULL)
       {
              Msg = "[db_base.updateData] sql is NULL";
              return 1;
       }

       int query_result = -1;
       //加写锁
       rwlock->writeLock();
       try
       {
              query_result = mysql_query(&mysql, SQL);
       }
       catch (const std::exception &e)
       {
              //释放写锁
              rwlock->writeUnlock();
              return 1;
       }
       //释放写锁
       rwlock->writeUnlock();

       if (query_result != 0)
       {
              Msg = "[db_base.updateData] Update Data Error";
              return 1;
       }
       return 0;
}

//删除数据
int DBBase::deleteData(char *SQL, string &Msg)
{
       if (SQL == NULL)
       {
              Msg = "[db_base.deleteData] sql is NULL";
              return 1;
       }

       int query_result = -1;
       //加写锁
       rwlock->writeLock();
       try
       {
              query_result = mysql_query(&mysql, SQL);
       }
       catch (const std::exception &e)
       {
              //释放写锁
              rwlock->writeUnlock();
              return 1;
       }
       //释放写锁
       rwlock->writeUnlock();

       if (query_result != 0)
       {
              Msg = "[db_base.deleteData] Delete Data error";
              return 1;
       }
       return 0;
}

//错误信息打印
void DBBase::errorIntoMySQL()
{
       //int errorNum = mysql_errno(&mysql);
       string errorInfo = mysql_error(&mysql);
       LOGE(errorInfo);
}

//关闭数据库连接
void DBBase::CloseMySQLConn()
{
       mysql_close(&mysql);
}

//判断SQL查询结果是否存在
bool DBBase::isExist(string str_sql, string tableName)
{
       string msg;
       Json::Value data = selectData(str_sql.c_str(), tableName, msg);

       return !data["is_empty"].asBool();
}

//判断SQL查询结果是否存在
bool DBBase::isExist(string str_sql, vector<string> columnsV)
{
       string msg;
       Json::Value data = selectData(str_sql.c_str(), columnsV, msg);

       return !data["is_empty"].asBool();
}

Json::Value DBBase::selectUserAccountByAccount(string account, string &Msg)
{
       LOGD("[db_base.selectUserAccountByAccount] handle account db query:" + account);

       //返参初始化
       Json::Value root;
       root["is_empty"] = true;

       //参数判空
       if (account == "")
       {
              Msg = "[db_base.selectUserAccountByAccount] account is empty";
              return root;
       }

       //构建存储过程执行语句
       string query = "call queryaccount ('" + account + "',@out_id,@out_account,@out_password,@out_pwd_salt)";

       LOGD("[db_base.selectUserAccountByAccount] db mysql_query : " + query);

       //加读锁
       rwlock->readLock();

       //执行存储过程执行语句
       int ret = mysql_real_query(&mysql, query.c_str(), (unsigned int)strlen(query.c_str()));
       mysql_query(&mysql, "SELECT @out_id,@out_account,@out_password,@out_pwd_salt");

       LOGD("[db_base.selectUserAccountByAccount] handle account db mysql_query finish");

       //判断查询是否成功
       if (ret)
       {
              Msg = "[db_base.selectUserAccountByAccount] error exec query" + ret;
              //释放读锁
              rwlock->readUnlock();
              return root;
       }

       MYSQL_ROW m_row;
       MYSQL_RES *m_res;

       //获取查询结果
       m_res = mysql_store_result(&mysql);
       if (m_res == NULL)
       {
              Msg = "[db_base.selectUserAccountByAccount] select m_res null";
              //释放读锁
              rwlock->readUnlock();
              mysql_free_result(m_res);
              return root;
       }

       //这里只会返回一条数据
       while (m_row = mysql_fetch_row(m_res))
       {
              //小于0则表示查询无结果或失败
              stringstream ss;
              ss << m_row[0];
              int i_id;
              ss >> i_id;
              if (i_id <= 0)
              {
                     LOGD("[db_base.selectUserAccountByAccount] handle account db mysql_query empty , id = " + (string)m_row[0]);
                     break;
              }
              root["ID"] = m_row[0];
              root["ACCOUNT"] = m_row[1];
              root["PASSWORD"] = m_row[2];
              root["PWD_SALT"] = m_row[3];
              root["is_empty"] = false;
       }

       //释放指针
       mysql_free_result(m_res);

       //释放读锁
       rwlock->readUnlock();
       return root;
}

Json::Value DBBase::insertUserAccount(string account, string password, string pwdSalt, string &Msg)
{

       //返参初始化
       Json::Value root;
       root["is_empty"] = true;

       //参数判空
       if (account == "")
       {
              Msg = "[db_base.insertUserAccount] account is empty";
              return root;
       }
       if (password == "")
       {
              Msg = "[db_base.insertUserAccount] password is empty";
              return root;
       }

       //构建存储过程执行语句
       string query = "call insertaccount ('" + account + "','" + password + "','" + pwdSalt + "',@out_id)";

       LOGD("[db_base.insertUserAccount] " + query);

       //加写锁
       rwlock->writeLock();

       //执行存储过程执行语句
       int ret = mysql_real_query(&mysql, query.c_str(), (unsigned int)strlen(query.c_str()));
       mysql_query(&mysql, "SELECT @out_id");

       //判断插入是否成功
       if (ret)
       {
              std::stringstream ssTemp;
              ssTemp << "[db_base.insertUserAccount] Error exec insert :" << ret;
              string msg = ssTemp.str();
              Msg = msg;
              DBBase::errorIntoMySQL();
              //释放写锁
              rwlock->writeUnlock();
              return root;
       }

       MYSQL_ROW m_row;
       MYSQL_RES *m_res;

       //获取查询结果
       m_res = mysql_store_result(&mysql);
       if (m_res == NULL)
       {
              Msg = "[db_base.insertUserAccount] select m_res null";
              //释放写锁
              DBBase::errorIntoMySQL();
              rwlock->writeUnlock();
              return root;
       }

       //这里只会返回一条数据
       while (m_row = mysql_fetch_row(m_res))
       {
              root["ID"] = m_row[0];
              root["is_empty"] = false;
       }

       //释放指针
       mysql_free_result(m_res);

       Json::FastWriter fw;
       LOGD("[db_base.insertUserAccount] " + fw.write(root));

       //释放写锁
       rwlock->writeUnlock();

       return root;
}

Json::Value DBBase::selectCategory(string title, int uid, string &Msg)
{
       LOGD("[db_base.selectCategory] handle category db query:" + title);

       //返参初始化
       Json::Value root;
       root["is_empty"] = true;

       //参数判空
       if (title == "")
       {
              Msg = "[db_base.selectCategory] title is empty";
              return root;
       }

       //构建存储过程执行语句
       std::stringstream ssTemp;
       ssTemp << "call querycategory ('" << title << "','" << uid << "',@out_id,@out_title,@out_uid)";
       string query = ssTemp.str();
       LOGD("[db_base.selectCategory] db mysql_query : " + query);

       //加读锁
       rwlock->readLock();

       //执行存储过程执行语句
       int ret = mysql_real_query(&mysql, query.c_str(), (unsigned int)strlen(query.c_str()));
       mysql_query(&mysql, "SELECT @out_id,@out_title,@out_uid");

       LOGD("[db_base.querycategory] handle category db mysql_query finish");

       //判断查询是否成功
       if (ret)
       {
              Msg = "[db_base.querycategory] error exec query";
              //释放读锁
              rwlock->readUnlock();
              return root;
       }

       MYSQL_ROW m_row;
       MYSQL_RES *m_res;

       //获取查询结果
       m_res = mysql_store_result(&mysql);
       if (m_res == NULL)
       {
              Msg = "[db_base.selectUserAccountByAccount] select m_res null";
              //释放读锁
              rwlock->readUnlock();
              mysql_free_result(m_res);
              return root;
       }

       //这里只会返回一条数据
       while (m_row = mysql_fetch_row(m_res))
       {
              //小于0则表示查询无结果或失败
              stringstream ss;
              ss << m_row[0];
              int i_id;
              ss >> i_id;
              if (i_id <= 0)
              {
                     LOGD("[db_base.selectUserAccountByAccount] handle account db mysql_query empty , id = " + (string)m_row[0]);
                     break;
              }
              root["ID"] = m_row[0];
              root["TITLE"] = m_row[1];
              root["UID"] = m_row[2];
              root["is_empty"] = false;
       }
       mysql_free_result(m_res);

       Json::FastWriter fw;
       LOGD("[db_base.selectCategory] root: " + fw.write(root));
       //释放读锁
       rwlock->readUnlock();
       return root;
}

Json::Value DBBase::selectCategoryList(int uid, string &Msg)
{
       LOGD("[db_base.selectCategoryList] handle category db query:");

       //返参初始化
       Json::Value root;
       root["is_empty"] = true;

       //构建存储过程执行语句
       std::stringstream ssTemp;
       ssTemp << "call querycategorylist ('" << uid << "')";
       string query = ssTemp.str();
       LOGD("[db_base.selectCategoryList] db mysql_query : " + query + "end");

       //加读锁
       rwlock->readLock();

       //执行存储过程执行语句
       int ret = mysql_real_query(&mysql, query.c_str(), (unsigned int)strlen(query.c_str()));
       // int ret = mysql_real_query(&mysql, query)
       //Json::Value data = selectData(query.c_str(), "category", Msg);
       LOGD("[db_base.selectCategoryList] handle category db mysql_query finish");

       //判断查询是否成功
       if (ret)
       {
              DBBase::errorIntoMySQL();
              Msg = "[db_base.selectCategoryList] error exec query";
              //释放读锁
              rwlock->readUnlock();
              return root;
       }

       Json::Value categorylist;
       MYSQL_ROW m_row;
       MYSQL_RES *m_res;
       if (mysql_field_count(&mysql) > 0)
       {
              if (m_res = mysql_store_result(&mysql))
              {
                     while (m_row = mysql_fetch_row(m_res))
                     {
                            stringstream ss;
                            ss << m_row[0];
                            int i_id;
                            ss >> i_id;
                            if (i_id <= 0)
                            {
                                   LOGD("[db_base.selectCategoryList] handle category db mysql_query empty , id = " + (string)m_row[0]);
                                   break;
                            }

                            Json::Value categoryItem;
                            categoryItem["ID"] = m_row[0];
                            categoryItem["TITLE"] = m_row[1];
                            categorylist.append(categoryItem);
                            if (root["is_empty"].asBool())
                            {
                                   root["is_empty"] = false;
                            }
                     }
              }
              mysql_free_result(m_res);
       }
       while (mysql_next_result(&mysql))
              ;
       root["data"] = categorylist;

       LOGD("[db_base.selectCategoryList] handle mysql_free_result finish");
       //释放读锁
       rwlock->readUnlock();
       LOGD("[db_base.selectCategoryList] handle readUnlock finish");
       Json::FastWriter fw;
       LOGD("[db_base.selectCategoryList] root: " + fw.write(root));
       return root;
}

Json::Value DBBase::insertCategory(string title, int uid, string &Msg)
{

       //返参初始化
       Json::Value root;
       root["is_empty"] = true;

       //参数判空
       if (title == "")
       {
              Msg = "[db_base.insertCategory] title is empty";
              return root;
       }
       if (uid < 1)
       {
              Msg = "[db_base.insertCategory] uid is invaial";
              return root;
       }

       //构建存储过程执行语句
       std::stringstream ssTemp;
       ssTemp << "call insertcategory ('" << title << "','" << uid << "',@out_cid)";
       string query = ssTemp.str();

       LOGD("[db_base.insertCategory] " + query);

       //加写锁
       rwlock->writeLock();

       //执行存储过程执行语句
       int ret = mysql_real_query(&mysql, query.c_str(), (unsigned int)strlen(query.c_str()));
       mysql_query(&mysql, "SELECT @out_cid");

       //判断插入是否成功
       if (ret)
       {
              DBBase::errorIntoMySQL();
              std::stringstream ssTemp;
              ssTemp << "[db_base.insertCategory] Error exec insert :" << ret;
              string msg = ssTemp.str();
              Msg = msg;
              //释放写锁
              rwlock->writeUnlock();
              return root;
       }

       MYSQL_ROW m_row;
       MYSQL_RES *m_res;

       //获取查询结果
       m_res = mysql_store_result(&mysql);
       if (m_res == NULL)
       {
              Msg = "[db_base.insertTodo] select m_res null";
              //释放写锁
              rwlock->writeUnlock();
              return root;
       }

       //这里只会返回一条数据
       while (m_row = mysql_fetch_row(m_res))
       {
              root["ID"] = m_row[0];
              root["is_empty"] = false;
       }

       //释放指针
       mysql_free_result(m_res);

       Json::FastWriter fw;
       LOGD("[db_base.insertCategory] " + fw.write(root));

       //释放写锁
       rwlock->writeUnlock();

       return root;
}

Json::Value DBBase::selectTodo(int tid, string &Msg)
{
       LOGD("[db_base.selectTodo] handle todos db query:");

       //返参初始化
       Json::Value root;
       root["is_empty"] = true;

       //参数判空
       if (tid < 1)
       {
              Msg = "[db_base.selectTodo] tid is wrong";
              return root;
       }

       //构建存储过程执行语句
       std::stringstream ssTemp;
       ssTemp << "call querytodo ('" << tid << "',@out_tid,@out_content,@out_cid,@out_status)";
       string query = ssTemp.str();
       LOGD("[db_base.selectTodo] db mysql_query : " + query);

       //加读锁
       rwlock->readLock();

       //执行存储过程执行语句
       int ret = mysql_real_query(&mysql, query.c_str(), (unsigned int)strlen(query.c_str()));
       mysql_query(&mysql, "SELECT @out_tid,@out_content,@out_cid,@out_status");

       LOGD("[db_base.querytodo] handle todos db mysql_query finish");

       //判断查询是否成功
       if (ret)
       {
              Msg = "[db_base.querytodo] error exec query";
              //释放读锁
              DBBase::errorIntoMySQL();
              rwlock->readUnlock();
              return root;
       }

       MYSQL_ROW m_row;
       MYSQL_RES *m_res;

       //获取查询结果
       m_res = mysql_store_result(&mysql);
       if (m_res == NULL)
       {
              Msg = "[db_base.querytodo] select m_res null";
              //释放读锁
              DBBase::errorIntoMySQL();
              rwlock->readUnlock();
              mysql_free_result(m_res);
              return root;
       }

       //这里只会返回一条数据
       while (m_row = mysql_fetch_row(m_res))
       {
              //小于0则表示查询无结果或失败
              stringstream ss;
              ss << m_row[0];
              int i_id;
              ss >> i_id;
              if (i_id <= 0)
              {
                     LOGD("[db_base.querytodo] handle todos db mysql_query empty , id = " + (string)m_row[0]);
                     break;
              }
              root["TID"] = m_row[0];
              root["CONTENT"] = m_row[1];
              root["UID"] = m_row[2];
              root["CID"] = m_row[3];
              root["STATUS"] = m_row[4];
              root["is_empty"] = false;
       }

       //释放指针
       mysql_free_result(m_res);

       //释放读锁
       rwlock->readUnlock();
       return root;
}

Json::Value DBBase::insertTodo(string content, int cid, int uid, string &Msg)
{

       //返参初始化
       Json::Value root;
       root["is_empty"] = true;

       //参数判空
       if (content == "")
       {
              Msg = "[db_base.insertTodo] content is empty";
              return root;
       }
       if (uid < 1)
       {
              Msg = "[db_base.insertTodo] uid is invaial";
              return root;
       }

       //构建存储过程执行语句
       std::stringstream ssTemp;
       ssTemp << "call inserttodo ('" << content << "','" << cid << "','"
              << uid << "',@out_tid)";
       string query = ssTemp.str();

       LOGD("[db_base.insertTodo] " + query);

       //加写锁
       rwlock->writeLock();

       //执行存储过程执行语句
       int ret = mysql_real_query(&mysql, query.c_str(), (unsigned int)strlen(query.c_str()));
       mysql_query(&mysql, "SELECT @out_tid");

       //判断插入是否成功
       if (ret)
       {
              std::stringstream ssTemp;
              ssTemp << "[db_base.insertTodo] Error exec insert :" << ret;
              string msg = ssTemp.str();
              Msg = msg;
              //释放写锁
              DBBase::errorIntoMySQL();
              rwlock->writeUnlock();
              return root;
       }

       MYSQL_ROW m_row;
       MYSQL_RES *m_res;

       //获取查询结果
       m_res = mysql_store_result(&mysql);
       if (m_res == NULL)
       {
              Msg = "[db_base.insertTodo] select m_res null";
              //释放写锁
              rwlock->writeUnlock();
              return root;
       }

       //这里只会返回一条数据
       while (m_row = mysql_fetch_row(m_res))
       {
              root["TID"] = m_row[0];
              root["is_empty"] = false;
       }

       //释放指针
       mysql_free_result(m_res);

       Json::FastWriter fw;
       LOGD("[db_base.insertTodo] " + fw.write(root));

       //释放写锁
       rwlock->writeUnlock();

       return root;
}

Json::Value DBBase::selectTodoList(int uid, int cid, string &Msg)
{
       LOGD("[db_base.selectTodoList] handle category db query:");

       //返参初始化
       Json::Value root;
       root["is_empty"] = true;

       //构建存储过程执行语句
       std::stringstream ssTemp;
       ssTemp << "call querytodolist ('" << uid << "','" << cid << "')";
       string query = ssTemp.str();
       LOGD("[db_base.selectTodoList] db mysql_query : " + query + "end");

       //加读锁
       rwlock->readLock();

       //执行存储过程执行语句
       int ret = mysql_real_query(&mysql, query.c_str(), (unsigned int)strlen(query.c_str()));
       LOGD("[db_base.selectTodoList] handle category db mysql_query finish");

       //判断查询是否成功
       if (ret)
       {
              DBBase::errorIntoMySQL();
              Msg = "[db_base.selectTodoList] error exec query";
              //释放读锁
              rwlock->readUnlock();
              return root;
       }

       Json::Value todolist;
       MYSQL_ROW m_row;
       MYSQL_RES *m_res;
       if (mysql_field_count(&mysql) > 0)
       {
              if (m_res = mysql_store_result(&mysql))
              {
                     while (m_row = mysql_fetch_row(m_res))
                     {
                            stringstream ss;
                            ss << m_row[0];
                            int i_id;
                            ss >> i_id;
                            if (i_id <= 0)
                            {
                                   LOGD("[db_base.selectTodoList] handle category db mysql_query empty , id = " + (string)m_row[0]);
                                   break;
                            }

                            Json::Value todoItem;
                            todoItem["ID"] = m_row[0];
                            todoItem["CONTENT"] = m_row[1];
                            todoItem["STATUS"] = m_row[2];
                            todolist.append(todoItem);
                            if (root["is_empty"].asBool())
                            {
                                   root["is_empty"] = false;
                            }
                     }
              }
              mysql_free_result(m_res);
       }
       while (mysql_next_result(&mysql))
              ;
       root["data"] = todolist;

       LOGD("[db_base.selectTodoList] handle mysql_free_result finish");
       //释放读锁
       rwlock->readUnlock();
       LOGD("[db_base.selectTodoList] handle readUnlock finish");
       Json::FastWriter fw;
       LOGD("[db_base.selectTodoList] root: " + fw.write(root));
       return root;
}
