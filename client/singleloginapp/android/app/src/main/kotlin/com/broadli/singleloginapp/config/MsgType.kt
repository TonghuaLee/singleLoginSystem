package com.broadli.singleloginapp.config

class MsgType {
    companion object {
        val CODE_SUCC = 0
        val CODE_FAIL = -1
        public val PARAM_MAIN_CMD = "mainCmd"
        public val PARAM_SUB_CMD = "subCmd"
        public val PARAM_CODE = "code"
        public val PARAM_MSG = "msg"
        public val PARAM_DATA = "data"
        public val MAIN_CMD_TEST = 0
        public val MAIN_CMD_DEFALUT = 0
        public val MAIN_CMD_LOGIN = 100
        public val MAIN_CMD_REGISTER = 101
        public val MAIN_CMD_LOGINOUT= 102
        
        // subCmd
        public val SUB_CMD_LOGINOUT_SELF = 1    // 主动下线
        public val SUB_CMD_LOGINOUT_SERVER = 2  // 被踢下线
    }

}