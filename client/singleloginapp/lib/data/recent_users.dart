import 'package:json_annotation/json_annotation.dart';

@JsonSerializable()
class RecentUser {
  int code;
  String msg;
  Map data;
  int mainCmd;
  int subCmd;

  RecentUser.origin();

  Map getData() {
    Map map = new Map();
    map['code'] = code;
    map['msg'] = msg;
    return map;
  }

  RecentUser(this.code, this.msg, this.data, this.mainCmd, this.subCmd);

}
