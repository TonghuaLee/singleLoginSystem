import 'package:flutter/material.dart';
import 'package:singleloginapp/pages/home_page.dart';
import 'pages/login_page.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: LoginPage(),
      routes: <String, WidgetBuilder>{
        //静态路由,无法传参
        'LoginPage': (_) => LoginPage(),
        'Home': (_) => HomePage(),
      },
    );
  }
}
