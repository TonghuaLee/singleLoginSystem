#pragma once

#include "login_core.hpp"
#include "login_listener.hpp"
#include "./model/network_result.h"

using namespace std;

namespace auth
{

    class LoginCoreImpl : public auth::LoginCore
    {

    public:
        LoginCoreImpl(const shared_ptr<LoginListener> &listener);
        /**
         *  测试 
         */
        virtual std::int32_t native_add(const std::int32_t x, const std::int32_t y) override;

        //是否登录
        static bool isLogin;

        /*
        主要功能：
        执行用户登录
        入口参数
        account： 	      用户账号
        password： 	      用户密码
        */
        virtual void user_login(const string &account, const string &password) override;

        /*
        主要功能：
        执行用户注册

        入口参数
        account： 	      用户账号
        password： 	      用户密码
        */
        virtual void user_sign(const string &account, const string &password) override;

        /*
        主要功能：
        执行检测用户连接状态
        */
        virtual void check_connection() override;

        /*
        主要功能：
        执行用户退出登录
        */
        virtual void user_logout() override;

        /*
        主要功能：
        执行检测用户登录状态（打开APP的时候）
        */
        virtual void check_login_status() override;

        virtual void add_category(const string &title) override;

        virtual void get_category_list() override;
        virtual void get_todo_list(const int32_t cid) override;

        virtual void add_todo(const string &content, int32_t cid) override;

        virtual void update_todo(int32_t tid, int32_t status) override;

        /*
        主要功能：
        执行刷新用户Token

        出口参数：
        ReqResult ：         接口请求返回数据对象
        */
        ReqResult refresh_token();

    private:
        //UI层回调对象
        shared_ptr<LoginListener> m_listener;

        /*
        主要功能：
        执行清除用户本地数据
        */
        void cleanUserInfo();

        /*
        主要功能：
        执行更新用户本地数据

        入口参数
        account： 	        用户账号
        token： 	            用户Token
        refreshToken： 	    用户refreshToken
        isConnect： 	        用户是否在线
        */
        void updateUserInfo(string account, string token, string refreshToken, int32_t tokenExpirationTime, string isConnect);
    };

} // namespace auth
