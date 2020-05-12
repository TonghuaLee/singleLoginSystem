import 'package:flutter/material.dart';
import 'package:singleloginapp/msg/event_listener.dart';
import 'package:singleloginapp/msg/message.dart';
import 'package:singleloginapp/pages/login_page.dart';
import 'package:singleloginapp/authso.dart';

class DartFFIPage extends StatelessWidget with EventListener {
  final String tag;

  DartFFIPage({this.tag, Key key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    Widget titleSection = Container(
      padding: const EdgeInsets.all(32.0),
      child: Row(
        children: <Widget>[
          Expanded(
            child: new Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: <Widget>[
                Container(
                  padding: const EdgeInsets.only(bottom: 8.0),
                  child: Text(
                    'Oeschinen Lake Campground',
                    style: TextStyle(fontWeight: FontWeight.bold),
                  ),
                ),
                Text(
                  'Kandersteg, Switzerland',
                  style: TextStyle(color: Colors.grey[500]),
                )
              ],
            ),
          ),
        ],
      ),
    );

    Widget textSection = new Container(
      padding: const EdgeInsets.symmetric(vertical: 22.0, horizontal: 32.0),
      child: new Text(
        'test native_add: 10+20=' + AuthSo.add(10, 20).toString(),
        softWrap: true,
      ),
    );
    return Scaffold(
      appBar: AppBar(title: Text('Dart ffi'), actions: <Widget>[]),
      body: ListView(
        children: <Widget>[
          titleSection,
          textSection,
          Container(
            padding: EdgeInsets.symmetric(horizontal: 32.0),
          )
        ],
      ),
    );
  }

  @override
  void onEvent(int event, Message msg) {
    // TODO: implement onEvent
  }
}
