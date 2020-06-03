import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:singleloginapp/pages/main_page.dart';
import 'package:singleloginapp/pages/register_page.dart';
import 'package:singleloginapp/pages/test_page.dart';
import 'package:singleloginapp/test/dart_ffi_page.dart';

import 'controller/dabase_provider.dart';
import 'pages/login_page.dart';

void main() {
  runApp(MyApp());
}
final GlobalKey<NavigatorState> navigatorKey=GlobalKey();

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (_) => DatabaseProvider(),
      child: MaterialApp(
        navigatorKey: navigatorKey,
        theme: ThemeData(
            primarySwatch: Colors.green,
            backgroundColor: Colors.white,
            primaryColor: Colors.blue,
            brightness: Brightness.light),
        home: LoginPage(),
        routes: <String, WidgetBuilder>{
          //静态路由,无法传参
          'LoginPage': (_) => LoginPage(),
          'RegisterPage': (_) => RegisterPage(),
          'Home': (_) => MyHomePage(),
          'Test': (_) => TestPage(),
          'DartffiPage': (_) => DartFFIPage(),
        },
      ),
    );
  }
}
