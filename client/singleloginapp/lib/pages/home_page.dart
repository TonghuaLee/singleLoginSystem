import 'package:flutter/material.dart';
import 'package:singleloginapp/pages/login_page.dart';
import 'package:singleloginapp/authso.dart';
class HomePage extends StatelessWidget {
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

    Column buildButtonColumn(IconData icon, String label) {
      Color color = Theme.of(context).primaryColor;
      return new Column(
        mainAxisSize: MainAxisSize.min,
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          new Icon(icon, color: color),
          new Container(
            margin: const EdgeInsets.only(top: 8.0),
            child: new Text(
              label,
              style: new TextStyle(
                fontSize: 12.0,
                fontWeight: FontWeight.w400,
                color: color,
              ),
            ),
          ),
        ],
      );
    }

    Widget buttonSection = Container(
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: <Widget>[
          buildButtonColumn(Icons.call, 'CALL'),
          buildButtonColumn(Icons.near_me, 'ROUTE'),
          buildButtonColumn(Icons.share, 'SHARE'),
        ],
      ),
    );
    Widget textSection = new Container(
      padding: const EdgeInsets.symmetric(vertical: 22.0, horizontal: 32.0),
      child: new Text(
        '''
Lake1 Oeschinen lies at the foot of the Blüemlisalp in the Bernese Alps. Situated 1,578 meters above sea level, it is one of the larger Alpine Lakes. A gondola ride from Kandersteg, followed by a half-hour walk through pastures and pine forest, leads you to the lake, which warms to 20 degrees Celsius in the summer. 
        ''',
        softWrap: true,
      ),
    );

    RaisedButton btn = new RaisedButton(
        onPressed: () {
          print('tt');
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
      appBar: AppBar(title: Text('欢迎光临' + AuthSo.add(10, 2).toString()),
          actions: <Widget>[]),
      body: ListView(
        children: [
          Hero(
            tag: tag ?? 'hero',
            child: Image.asset(
              'images/pic.jpg',
              width: 600.0,
              height: 240.0,
              fit: BoxFit.cover,
            ),
          ),
          titleSection,
          buttonSection,
          textSection,
          Container(
            padding: EdgeInsets.symmetric(horizontal: 32.0),
            child: btn,
          )
        ],
      ),
    );
  }
}
