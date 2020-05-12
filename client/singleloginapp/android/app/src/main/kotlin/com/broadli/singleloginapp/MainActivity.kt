package com.broadli.singleloginapp

import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.widget.Toast
import io.flutter.app.FlutterActivity
import io.flutter.plugin.common.BasicMessageChannel
import io.flutter.plugin.common.BasicMessageChannel.*
import io.flutter.plugin.common.BinaryMessenger
import io.flutter.plugin.common.StandardMessageCodec
import io.flutter.plugins.GeneratedPluginRegistrant
import org.json.JSONException
import org.json.JSONObject
import java.util.*

class MainActivity : FlutterActivity() {
    private val CHANNEL = "samples.flutter.io/battery"
    var flutterView: BinaryMessenger? = null
    private var mContext: Context? = null
    private var mMessageChannel: BasicMessageChannel<Any>? = null

    var mHandler = Handler(Looper.myLooper())
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        GeneratedPluginRegistrant.registerWith(this)
        flutterView = getFlutterView()
        mContext = this
        messageChannelFunction()
    }

    private fun messageChannelFunction() { //消息接收监听
//BasicMessageChannel （主要是传递字符串和一些半结构体的数据）
        mMessageChannel = BasicMessageChannel<Any>(getFlutterView(), "flutter_and_native_100", StandardMessageCodec.INSTANCE)
        // 接收消息监听
        mMessageChannel!!.setMessageHandler { message, reply ->
            Log.d("Android", "Received message = $message")
            reply.reply("Reply from Android")
            println("onMessage: $message")
            var lJSONObject: JSONObject? = null
            try { //解析Flutter 传递的参数
                lJSONObject = JSONObject(message.toString())
                //方法名标识
                val lMethod = lJSONObject.getString("method")
                //测试 reply.reply()方法 发消息给Flutter
                if (lMethod == "test") {
                    Toast.makeText(mContext, "flutter 调用到了 android test", Toast.LENGTH_SHORT).show()
                    val resultMap: MutableMap<String, Any> = HashMap()
                    resultMap["message"] = "reply.reply 返回给flutter的数据"
                    resultMap["code"] = 200
                    reply.reply(resultMap)
                } else if (lMethod == "test2") { //测试 mMessageChannel.send 发消息给Flutter
                    channelSendMessage()
                } else if (lMethod == "test3") { //测试通过Flutter打开Android Activity
                    Toast.makeText(mContext, "flutter 调用到了 android test3", Toast.LENGTH_SHORT).show()
//                        val lIntent = Intent(this@MainActivity, TestBasicMessageActivity::class.java)
//                        this@MainActivity.startActivity(lIntent)
                }
            } catch (e: JSONException) {
                e.printStackTrace()
            }
        }

    }

    private fun channelSendMessage() {
        Toast.makeText(mContext, "flutter 调用到了 android test", Toast.LENGTH_SHORT).show()
        val resultMap: MutableMap<String, Any> = HashMap()
        resultMap["message"] = "reply.reply 返回给flutter的数据"
        resultMap["code"] = 200
        mMessageChannel?.send(resultMap) { reply ->
            Log.d("Android", "$reply")
        }
    }

}
