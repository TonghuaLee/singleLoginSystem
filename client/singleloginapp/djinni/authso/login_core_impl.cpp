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

// extern "C" {
// // __attribute__((visibility("default"))) __attribute__((used))
//     int32_t native_add(int32_t x, int32_t y) { return x + y+100; }
// }
using namespace Auth;

namespace Auth {

   std::shared_ptr<LoginCore> LoginCore::create() {
       return std::make_shared<LoginCoreImpl>();
   }

   LoginCoreImpl::LoginCoreImpl() {
   }

    bool LoginCoreImpl::isLogin = false;

    std::int32_t LoginCoreImpl::native_add(const std::int32_t x, const std::int32_t y)
    {
//        LOGD("[login_core_impl.hello]");
        return x+y;
    }
}
