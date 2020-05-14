#pragma once
 
#include "login_core.hpp"

using namespace std;

namespace Auth {
    
    class LoginCoreImpl : public Auth::LoginCore {
        
    public:

        LoginCoreImpl();

        //是否登录
        static bool isLogin;

        virtual ~LoginCoreImpl() {}

        virtual std::int32_t native_add(const std::int32_t x, const std::int32_t y) override;

        // virtual void login(const std::string &account, const std::string &password) override;

        // virtual void singUp(const std::string &account, const std::string &password) override;

        // virtual void userLogout() override;

        // virtual void checkLoginStatus() override;

        // virtual void checkConnection() override;

        /*
        主要功能：
        执行刷新用户Token

        出口参数：
        ReqResult ：         接口请求返回数据对象
        */
   //     ReqResult refreshToken();

    private:

        //UI层回调对象
    //    shared_ptr<LoginListener> m_listener;

        /*
        主要功能：
        执行清除用户本地数据
        */
      //  void cleanUserInfo();

        /*
        主要功能：
        执行更新用户本地数据

        入口参数
        account： 	        用户账号
        token： 	            用户Token
        refreshToken： 	    用户refreshToken
        isConnect： 	        用户是否在线
        */
    //    void updateUserInfo(string account,string token,string refreshToken,int32_t tokenExpirationTime, string isConnect);
    };


}
