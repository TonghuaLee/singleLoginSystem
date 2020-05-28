// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'message.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

Message _$MessageFromJson(Map<String, dynamic> json) {
  return Message(
    json['code'] as int,
    json['message'] as String,
    json['data'] as Map<String, dynamic>,
    json['mainCmd'] as int,
    json['subCmd'] as int,
  );
}

Map<String, dynamic> _$MessageToJson(Message instance) => <String, dynamic>{
      'code': instance.code,
      'message': instance.message,
      'data': instance.data,
      'mainCmd': instance.mainCmd,
      'subCmd': instance.subCmd,
    };
