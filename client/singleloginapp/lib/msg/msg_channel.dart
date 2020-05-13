import 'dart:developer';
import 'package:flutter/services.dart';
import 'package:singleloginapp/config/config.dart';
import 'package:singleloginapp/msg/event_listener.dart';
import 'package:singleloginapp/utils/log_util.dart';
import 'dart:convert' as convert;
import 'message.dart';

class MsgChannelUtil {
  ///type
  static const MSG_TYPE_TEST = 0;
  static const MAIN_CMD_DEFALUT = 0;
  static const MAIN_CMD_LOGIN = 100;
  static const MAIN_CMD_REGISTER = 101;

  static MsgChannelUtil _instance;
  BasicMessageChannel messageChannel;
  Set<EventListener> mListeners = new Set();

  MsgChannelUtil() {
    if (messageChannel == null) {
      messageChannel = const BasicMessageChannel(
          Config.MSG_CHANNEL_NAME, StandardMessageCodec());
    }
  }

  static MsgChannelUtil getInstance() {
    if (_instance == null) {
      _instance = new MsgChannelUtil();
    }

    return _instance;
  }

  bool addListener(EventListener listener) {
    if (listener == null) {
      return false;
    }
    mListeners.add(listener);
  }

  void removeListener(EventListener listener) {
    mListeners.remove(listener);
  }

  /// 通知所有监听者
  Future<void> notifyEvent(int event, Message msg) async {
    for (final listener in mListeners) {
      listener.onEvent(event, msg);
    }
  }

  //发送消息
  Future<Message> sendMessage(Message sendMsg) async {
    String josnNameString = convert.jsonEncode(sendMsg);
    LogUtils.d('msgChannel', 'flutter sendMessage:' + josnNameString);
    String reply = await messageChannel.send(josnNameString);
    //解析 原生发给 Flutter 的参数
    if (reply != null) {
//      int code = reply['code'];
//      String message = reply['message'];
      LogUtils.d('msgChannel', 'flutter receiveMessage:' + reply);
    }
    Message replyMsg = new Message.origin();
    return replyMsg;
  }

  //接收消息监听
  void receiveMessage() {
    messageChannel.setMessageHandler((result) async {
      //解析 原生发给 Flutter 的参数
      int code = result['code'];
      String message = result['message'];
      LogUtils.d('msgChannel', 'flutter receiveMessage:' + message);
      return 'Flutter 已收到消息';
    });
  }
}
