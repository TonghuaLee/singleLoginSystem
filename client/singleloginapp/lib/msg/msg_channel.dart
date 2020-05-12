import 'dart:developer';
import 'dart:html';

import 'package:flutter/services.dart';
import 'package:singleloginapp/msg/event_listener.dart';

import 'message.dart';

class MsgUtil {
  static const messageChannel = const BasicMessageChannel(
      'flutter_and_native_100', StandardMessageCodec());
  Set<EventListener> mListeners = new Set();

  bool addListener(EventListener listener) {
    if (listener == null) {
      return false;
    }
    mListeners.add(listener);
  }

  void removeListener(EventListener listener) {
    mListeners.remove(listener);
  }

  /**
   * 通知所有监听者
   */
  void notifyEvent(int event, Message msg) {
    for (final listener in mListeners) {
      listener.onEvent(event, msg);
    }
  }

  //发送消息
  Future<Map> sendMessage(Map json) async {
    Map reply = await messageChannel.send(json);
    //解析 原生发给 Flutter 的参数
    int code = reply["code"];
    String message = reply["message"];
    log("flutter receiveMessage:" + message);
    return reply;
  }

  //接收消息监听
  void receiveMessage() {
    messageChannel.setMessageHandler((result) async {
      //解析 原生发给 Flutter 的参数
      int code = result["code"];
      String message = result["message"];
      log("flutter receiveMessage:" + message);
      return 'Flutter 已收到消息';
    });
  }
}
