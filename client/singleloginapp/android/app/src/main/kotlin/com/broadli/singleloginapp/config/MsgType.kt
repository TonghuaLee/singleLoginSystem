package com.broadli.singleloginapp.config

class MsgType {
    companion object {
        val CODE_SUCC = 0
        val CODE_FAIL = -1
        val PARAM_MAIN_CMD = "mainCmd"
        val PARAM_SUB_CMD = "subCmd"
        val PARAM_CODE = "code"
        val PARAM_MSG = "msg"
        val PARAM_DATA = "data"
        val MAIN_CMD_TEST = 0
        val MAIN_CMD_DEFALUT = 0
        val MAIN_CMD_LOGIN = 100
        val MAIN_CMD_REGISTER = 101
        val MAIN_CMD_LOGINOUT = 102
        val MAIN_CMD_CHECK_LOGIN_STATE = 103
        val MAIN_CMD_ADD_CATEGORY = 104

        // subCmd
        val SUB_CMD_DEFAULT = 0    // 默认值
        val SUB_CMD_LOGINOUT_SELF = 1    // 主动下线
        val SUB_CMD_LOGINOUT_SERVER = 2  // 被踢下线
    }

}