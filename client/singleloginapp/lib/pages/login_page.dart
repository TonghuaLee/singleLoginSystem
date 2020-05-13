import 'package:flutter/material.dart';
import 'package:singleloginapp/msg/message.dart';
import 'package:singleloginapp/msg/msg_channel.dart';
import 'package:singleloginapp/pages/register_page.dart';
import 'package:singleloginapp/utils/log_util.dart';

import 'home_page.dart';

class LoginPage extends StatefulWidget {
  @override
  _LoginPageState createState() => _LoginPageState();
}

class _LoginPageState extends State<LoginPage> {
  final _formKey = GlobalKey<FormState>();
  String _email, _password;
  bool _isObscure = true;
  Color _eyeColor;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: Form(
            key: _formKey,
            child: ListView(
              padding: EdgeInsets.symmetric(horizontal: 22.0),
              children: <Widget>[
                SizedBox(
                  height: kToolbarHeight,
                ),
                buildTitle(),
                buildTitleLine(),
                SizedBox(height: 70.0),
                buildEmailTextField(),
                SizedBox(height: 30.0),
                buildPasswordTextField(context),
                buildRegisterText(context),
                SizedBox(height: 60.0),
                buildLoginButton(context),
                SizedBox(height: 30.0),
              ],
            )));
  }

  Align buildLoginButton(BuildContext context) {
    return Align(
      child: SizedBox(
        height: 45.0,
        width: 270.0,
        child: RaisedButton(
          child: Text('Login',
              style: TextStyle(color: Colors.white, fontSize: 18.0)),
          color: Colors.black,
          onPressed: () async {
            if (_formKey.currentState.validate()) {
              ///只有输入的内容符合要求通过才会到达此处
              _formKey.currentState.save();
              //TODO 执行登录方法
              print('email:$_email , assword:$_password');
              //MsgChannelUtil.getInstance().addListener(listener)
              Message msg = new Message(1, 'req login from flutter', null,
                  MsgChannelUtil.MAIN_CMD_LOGIN,
                  MsgChannelUtil.MAIN_CMD_DEFALUT);
//              LogUtils.d('login_page', 'req from flutter: '+ msg.toJson());
              Message result = await MsgChannelUtil.getInstance().sendMessage(
                  msg);
              LogUtils.d('login_page', 'result: ' + result.toJson().toString());
              Navigator.pushAndRemoveUntil(context, new MaterialPageRoute(
                builder: (BuildContext context) {
                  return new HomePage();
                },
              ), (route) => route == null);
              //Navigator.pushNamedAndRemoveUntil(context, 'Home', (route) => route == null);
            }
          },
          shape: StadiumBorder(side: BorderSide()),
        ),
      ),
    );
  }

  Padding buildRegisterText(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.only(top: 8.0),
      child: Align(
        alignment: Alignment.centerRight,
        child: FlatButton(
          child: Text(
            'Sign up',
            style: TextStyle(fontSize: 14.0, color: Colors.grey),
          ),
          onPressed: () {
            Navigator.pushAndRemoveUntil(context, new MaterialPageRoute(
              builder: (BuildContext context) {
                return new RegisterPage();
              },
            ), (route) => route == null);
          },
        ),
      ),
    );
  }

  TextFormField buildPasswordTextField(BuildContext context) {
    return TextFormField(
      onSaved: (String value) => _password = value,
      obscureText: _isObscure,
      initialValue: "123456",
      validator: (String value) {
        if (value.isEmpty) {
          return '请输入密码';
        }
        return null;
      },
      decoration: InputDecoration(
          labelText: 'Password',
          suffixIcon: IconButton(
              icon: Icon(
                Icons.remove_red_eye,
                color: _eyeColor,
              ),
              onPressed: () {
                setState(() {
                  _isObscure = !_isObscure;
                  _eyeColor = Colors.grey;
                });
              })),
    );
  }

  TextFormField buildEmailTextField() {
    return TextFormField(
      //输入模式，邮件
      keyboardType: TextInputType.emailAddress,
      initialValue: '8888888@qq.com',
      decoration: InputDecoration(
        labelText: 'Emall Address',
      ),
      validator: (String value) {
        var emailReg = RegExp(
            r"[\w!#$%&'*+/=?^_`{|}~-]+(?:\.[\w!#$%&'*+/=?^_`{|}~-]+)*@(?:[\w](?:[\w-]*[\w])?\.)+[\w](?:[\w-]*[\w])?");
        if (!emailReg.hasMatch(value)) {
          return '请输入正确的邮箱地址';
        }
        return null;
      },
      onSaved: (String value) => _email = value,
    );
  }

  Padding buildTitleLine() {
    return Padding(
      padding: EdgeInsets.only(left: 12.0, top: 4.0),
      child: Align(
        alignment: Alignment.bottomLeft,
        child: Container(
          color: Colors.black,
          width: 40.0,
          height: 2.0,
        ),
      ),
    );
  }

  Padding buildTitle() {
    return Padding(
      padding: EdgeInsets.all(8.0),
      child: Text(
        'Login',
        style: TextStyle(fontSize: 42.0),
      ),
    );
  }
}
