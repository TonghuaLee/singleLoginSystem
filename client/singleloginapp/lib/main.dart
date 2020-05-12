import 'package:flutter/material.dart';
import 'package:singleloginapp/pages/home_page.dart';
import 'package:singleloginapp/pages/register_page.dart';
import 'package:singleloginapp/pages/test_page.dart';
import 'package:singleloginapp/test/dart_ffi_page.dart';

import 'pages/login_page.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData(
          primarySwatch: Colors.blue,
          backgroundColor: Colors.white,
          primaryColor: Colors.green,
          brightness: Brightness.light),
      home: LoginPage(),
      routes: <String, WidgetBuilder>{
        //静态路由,无法传参
        'LoginPage': (_) => LoginPage(),
        'RegisterPage': (_) => RegisterPage(),
        'Home': (_) => HomePage(),
        'Test': (_) => TestPage(),
        'DartffiPage': (_) => DartFFIPage(),
      },
    );
  }
}
