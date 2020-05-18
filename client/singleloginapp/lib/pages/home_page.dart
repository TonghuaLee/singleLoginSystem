import 'package:flutter/material.dart';
import 'package:singleloginapp/msg/event_listener.dart';
import 'package:singleloginapp/msg/message.dart';
import 'package:singleloginapp/pages/login_page.dart';
import 'package:singleloginapp/authso.dart';

class HomePage extends StatelessWidget with EventListener {
  final String tag;

  HomePage({this.tag, Key key}) : super(key: key);

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
        'Welcome Come',
        softWrap: true,
      ),
    );

    RaisedButton btn = new RaisedButton(
        onPressed: () {
          Navigator.pushAndRemoveUntil(context, new MaterialPageRoute(
            builder: (BuildContext context) {
              return new LoginPage();
            },
          ), (route) => route == null);
//          Navigator.of(context).pushNamed('LoginPage');
        },
        color: Colors.green,
        child: new Text('Sign out', style: new TextStyle(color: Colors.white)));
    return Scaffold(
      appBar: AppBar(title: Text('欢迎光临'),
          actions: <Widget>[]),
      body: ListView(
        children: <Widget>[
          GestureDetector(
              child: Hero(
                tag: tag ?? 'hero',
                child: Image.asset(
                  'images/pic.jpg',
                  width: 600.0,
                  height: 240.0,
                  fit: BoxFit.cover,
                ),
              ),
              onTap: () {
                Navigator.pushNamed(context, 'Test');
              }
          ),
          titleSection,
          textSection,
          Container(
            padding: EdgeInsets.symmetric(horizontal: 32.0),
            child: btn,
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
