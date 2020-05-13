import 'package:flutter/services.dart';
import 'package:singleloginapp/config/config.dart';

class LogUtils {
  static const perform = const MethodChannel(Config.FLUTTER_LOG_CHANNEL);

  static void v(String tag, String message) {
    perform.invokeMethod('logV', {'tag': tag, 'msg': message});
  }

  static void d(String tag, String message) {
    perform.invokeMethod('logD', {'tag': tag, 'msg': message});
  }

  static void i(String tag, String message) {
    perform.invokeMethod('logI', {'tag': tag, 'msg': message});
  }

  static void w(String tag, String message) {
    perform.invokeMethod('logW', {'tag': tag, 'msg': message});
  }

  static void e(String tag, String message) {
    perform.invokeMethod('logE', {'tag': tag, 'msg': message});
  }
}