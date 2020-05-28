package com.broadli.singleloginapp

import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.KeyEvent
import android.widget.Toast
import com.broadli.singleloginapp.config.MsgType
import com.broadli.singleloginapp.config.MsgType.Companion.CODE_FAIL
import com.broadli.singleloginapp.config.MsgType.Companion.CODE_SUCC
import com.broadli.singleloginapp.config.MsgType.Companion.MAIN_CMD_LOGIN
import com.broadli.singleloginapp.config.MsgType.Companion.MAIN_CMD_LOGINOUT
import com.broadli.singleloginapp.config.MsgType.Companion.MAIN_CMD_REGISTER
import com.broadli.singleloginapp.config.MsgType.Companion.SUB_CMD_LOGINOUT_SELF
import com.broadli.singleloginapp.config.MsgType.Companion.SUB_CMD_LOGINOUT_SERVER
import com.broadli.singleloginapp.controller.LoginController
import com.broadli.singleloginapp.controller.LoginUIController
import com.broadli.singleloginapp.model.Msg
import com.broadli.singleloginapp.model.UserInfo
import com.google.gson.Gson
import io.flutter.app.FlutterActivity
import io.flutter.plugin.common.*
import io.flutter.plugins.GeneratedPluginRegistrant
import java.lang.System.loadLibrary
import java.util.*


class MainActivity : FlutterActivity(), LoginUIController {
    var flutterView: BinaryMessenger? = null
    private var mContext: Context? = null
    private var mMessageChannel: BasicMessageChannel<Any>? = null
    val TAG = "MainActivity"
    var mLoginControler: LoginController? = null
    private var exitTime: Long = 0

    companion object {
        const val FLUTTER_LOG_CHANNEL = "android_log"
        const val FLUTTER_NATIVE_MSG_CHANNEL = "dart_native_channel"
    }

    var mHandler = Handler(Looper.myLooper())
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        GeneratedPluginRegistrant.registerWith(this)
        loadLibrary("authso")
        flutterView = getFlutterView()
        mContext = this
        mLoginControler = LoginController(this, this)
        listenMsgChannel()
        listenLogChannel()
        testNativeAdd()
    }

    private fun listenMsgChannel() { //消息接收监听
//BasicMessageChannel （主要是传递字符串和一些半结构体的数据）
        mMessageChannel = BasicMessageChannel<Any>(getFlutterView(), FLUTTER_NATIVE_MSG_CHANNEL, StandardMessageCodec.INSTANCE)
        // 接收消息监听
        mMessageChannel!!.setMessageHandler { message, reply ->
            Log.d("Android", "Received message = $message")
            println("onMessage: $message")
            var reqMsg = Gson().fromJson(message.toString(), Msg::class.java)
            if (reqMsg != null) {
                if (reqMsg.mainCmd == MsgType.MAIN_CMD_LOGIN) {
                    Log.d(TAG, "login receive from flutter")
                    var data = reqMsg.data
                    val userInfo = UserInfo(data?.get("account") as String, data["password"] as String)
                    mLoginControler?.run {
                        actionLoginIn(userInfo.account, userInfo.password)
                    }

                    // test for
                    var replyMsg = reqMsg
                    replyMsg.code = 200
                    replyMsg.message = "reply from Android"
                    var gson = Gson()
                    var replyJsonStr = gson.toJson(replyMsg)
                    Log.d(TAG, "reply: " + replyJsonStr)
                    reply.reply(replyJsonStr)
                } else if (reqMsg.mainCmd == MsgType.MAIN_CMD_REGISTER) { //测试 mMessageChannel.send 发消息给Flutter
                    //  channelSendMessage()
                    var data = reqMsg.data
                    val userInfo = UserInfo(data?.get("account") as String, data["password"] as String)
                    mLoginControler?.run {
                        actionSignIn(userInfo.account, userInfo.password)
                    }
                } else { //测试通过Flutter打开Android Activity
                    Toast.makeText(mContext, "flutter 调用到了 android test3", Toast.LENGTH_SHORT).show()
//                        val lIntent = Intent(this@MainActivity, TestBasicMessageActivity::class.java)
//                        this@MainActivity.startActivity(lIntent)
                }
            }
        }

    }

    private fun sendMsg() {
        Toast.makeText(mContext, "flutter 调用到了 android test", Toast.LENGTH_SHORT).show()
        val resultMap: MutableMap<String, Any> = HashMap()
        resultMap["message"] = "reply.reply 返回给flutter的数据"
        resultMap["code"] = 200

        mMessageChannel?.send(resultMap) { reply ->
            Log.d("Android", "$reply")
        }
    }

    private fun listenLogChannel() {
        MethodChannel(flutterView, FLUTTER_LOG_CHANNEL).setMethodCallHandler { call, result ->
            logPrint(call)
        }
    }

    private fun logPrint(call: MethodCall) {
        var tag: String = call.argument("tag")!!
        var message: String = call.argument("msg")!!
        when (call.method) {
            "logV" -> Log.v(tag, message)
            "logD" -> Log.d(tag, message)
            "logI" -> Log.i(tag, message)
            "logW" -> Log.w(tag, message)
            "logE" -> Log.e(tag, message)
        }
    }

    private fun testNativeAdd() {
        mLoginControler?.addTest()
    }

    override fun performLoginSuccess() {
        var replyMsg = Msg(MAIN_CMD_LOGIN, 0, CODE_SUCC, "登录成功。")
        var gson = Gson()
        var replyJsonStr = gson.toJson(replyMsg)
        mMessageChannel?.send(replyJsonStr) { reply ->
            Log.d("Android", "$reply")
        }
    }

    override fun performSignSuccess() {
        var replyMsg = Msg(MAIN_CMD_REGISTER, 0, CODE_SUCC, "注册成功。")
        var gson = Gson()
        var replyJsonStr = gson.toJson(replyMsg)
        mMessageChannel?.send(replyJsonStr) { reply ->
            Log.d("Android", "$reply")
        }
    }

    override fun performLoginFail(msg: String?) {
        var replyMsg = Msg(MAIN_CMD_LOGIN, 0, CODE_FAIL, msg)
        var gson = Gson()
        var replyJsonStr = gson.toJson(replyMsg)
        mMessageChannel?.send(replyJsonStr) { reply ->
            Log.d("Android", "$reply")
        }
    }

    override fun performSignFail(msg: String?) {
        Log.d(TAG, "注册失败")
        var replyMsg = Msg(MAIN_CMD_REGISTER, 0, CODE_FAIL, msg)
        var gson = Gson()
        var replyJsonStr = gson.toJson(replyMsg)
        mMessageChannel?.send(replyJsonStr) { reply ->
            Log.d("Android", "$reply")
        }
    }

    override fun performLogout() {
        var replyMsg = Msg(MAIN_CMD_LOGINOUT, SUB_CMD_LOGINOUT_SELF, CODE_SUCC, "下线。")
        var gson = Gson()
        var replyJsonStr = gson.toJson(replyMsg)
        mMessageChannel?.send(replyJsonStr) { reply ->
            Log.d("Android", "$reply")
        }
    }

    override fun performUserOnline(account: String?) {
        var replyMsg = Msg(MAIN_CMD_LOGIN, SUB_CMD_LOGINOUT_SELF, CODE_SUCC, "已经在线。")
        var gson = Gson()
        var replyJsonStr = gson.toJson(replyMsg)
        mMessageChannel?.send(replyJsonStr) { reply ->
            Log.d("Android", "$reply")
        }
    }

    override fun toastMsg(content: String?) {
        Toast.makeText(this, content, Toast.LENGTH_LONG).show()
    }

    override fun disconnect() {
        var replyMsg = Msg(MAIN_CMD_LOGINOUT, SUB_CMD_LOGINOUT_SERVER, CODE_SUCC, "在其他终端登录，被踢下线。")
        var gson = Gson()
        var replyJsonStr = gson.toJson(replyMsg)
        mMessageChannel?.send(replyJsonStr) { reply ->
            Log.d("Android", "$reply")
        }
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            exit()
            return false
        }
        return super.onKeyDown(keyCode, event)
    }

    fun exit() {
        if ((System.currentTimeMillis() - exitTime) > 2000) {
            Toast.makeText(applicationContext,
                    "再按一次退出程序", Toast.LENGTH_SHORT).show()
            exitTime = System.currentTimeMillis()

        } else {
            finish()
            System.exit(0)
        }
    }
}
