#include <cstdint>
#include <iostream>
#include <sstream>
#include <ctime>
#include <vector>
#include <exception>
#include <time.h>

# define APP_PLATFORM ANDROID

//#include "SCLoginListener.h"

#include "login_core_impl.hpp"

extern "C" {
// __attribute__((visibility("default"))) __attribute__((used))
    int32_t native_add(int32_t x, int32_t y) { return x + y+100; }
}

namespace Auth {

//    std::shared_ptr<LoginCore> LoginCore::create(const std::shared_ptr<LoginListener> & listener) {
//        return std::make_shared<LoginCoreImpl>(listener);
//    }

//    LoginCoreImpl::LoginCoreImpl(const std::shared_ptr<LoginListener> & listener) {
//        this->m_listener = listener;
//    }

    bool LoginCoreImpl::isLogin = false;

    std::int32_t LoginCoreImpl::native_add(const std::int32_t x, const std::int32_t y)
    {
//        LOGD("[login_core_impl.hello]");
        return x+y;
    }
}
