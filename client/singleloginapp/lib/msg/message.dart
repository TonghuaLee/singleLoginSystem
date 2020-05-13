import 'package:json_annotation/json_annotation.dart';

part 'message.g.dart';

@JsonSerializable()
class Message {
  int code;
  String msg;
  Map data;
  int mainCmd;
  int subCmd;

  Message.origin();

  Map getData() {
    Map map = new Map();
    map['code'] = code;
    map['msg'] = msg;
    return map;
  }

  Message(this.code, this.msg, this.data, this.mainCmd, this.subCmd);

  factory Message.fromJson(Map<String, dynamic> json) =>
      _$MessageFromJson(json);

  Map<String, dynamic> toJson() => _$MessageToJson(this);

}
