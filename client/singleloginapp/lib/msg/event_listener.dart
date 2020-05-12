import 'message.dart';

mixin EventListener {
  int EVENT_TEST = 0;
  int EVENT_ON_LOGIN = 1;
  int EVENT_ON_REGISTER = 2;

  void onEvent(int event, Message msg);
}
