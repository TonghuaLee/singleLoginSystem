import 'dart:developer';

import 'package:flutter/material.dart';
import 'package:singleloginapp/msg/event_listener.dart';
import 'package:singleloginapp/msg/message.dart';
import 'package:singleloginapp/msg/msg_channel.dart';
import 'package:singleloginapp/msg/result.dart';
import 'package:singleloginapp/pages/main_page.dart';
import 'package:singleloginapp/pages/register_page.dart';
import 'package:singleloginapp/utils/log_util.dart';
import 'package:singleloginapp/widget/animatedloginbutton.dart';

class LoginPage extends StatefulWidget {
  @override
  _LoginPageState createState() => _LoginPageState();
}

class _LoginPageState extends State<LoginPage> with EventListener {
  final String TAG = "LoginPage";
  final _formKey = GlobalKey<FormState>();
  String _phone, _password;
  bool _isObscure = true;
  Color _eyeColor;

  @override
  void initState() {
    super.initState();
    MsgChannelUtil.getInstance().addListener(this);
    phoneController.text = '16625205201';
    passController.text = '123456thLee';
  }

  final LoginErrorMessageController loginErrorMessageController =
      LoginErrorMessageController();

  //手机号的控制器
  TextEditingController phoneController = TextEditingController();

  //密码的控制器
  TextEditingController passController = TextEditingController();

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
                buildPhoneTextField(),
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
      child: new AnimatedLoginButton(
        loginErrorMessageController: loginErrorMessageController,
        loginTip: "Login",
        indicatorWidth: 1,
        height: 45,
        width: 270,
        showErrorTime: const Duration(milliseconds: 1000),
        buttonColorNormal: Colors.black,
        onTap: () async {
          log("login confirm btn onTap");
          if (_formKey.currentState.validate()) {
            ///只有输入的内容符合要求通过才会到达此处
            _formKey.currentState.save();
            //TODO 执行登录方法
            print('phone:$_phone , password:$_password');
            //MsgChannelUtil.getInstance().addListener(listener)

            Map req = new Map();
            req['account'] = phoneController.text;
            req['password'] = passController.text;
            Message msg = new Message(1, 'req login from flutter', req,
                MsgChannelUtil.MAIN_CMD_LOGIN, MsgChannelUtil.MAIN_CMD_DEFALUT);
            LogUtils.d(TAG, 'req: ' + msg.toString());
//              LogUtils.d(TAG, 'req from flutter: '+ msg.toJson());
            Message result =
                await MsgChannelUtil.getInstance().sendMessage(msg);
            LogUtils.d(TAG, 'result: ' + result.toJson().toString());
            //Navigator.pushNamedAndRemoveUntil(context, 'Home', (route) => route == null);
          } else {
            loginErrorMessageController.showErrorMessage("参数错误");
          }
        },
      ),
    );
  }

  TextFormField buildPasswordTextField(BuildContext context) {
    return TextFormField(
      onSaved: (String value) => _password = value,
      obscureText: _isObscure,
      controller: passController,
      validator: (String value) {
        RegExp pswReg =
            new RegExp(r"(?![0-9]+$)(?![a-zA-Z]+$)[0-9A-Za-z]{6,16}$");
        if (!pswReg.hasMatch(value)) {
          return '请输入6~16位数字和字符组合密码';
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

  TextFormField buildPhoneTextField() {
    return TextFormField(
      //输入模式，邮件
      keyboardType: TextInputType.phone,
      controller: phoneController,
      decoration: InputDecoration(
        labelText: 'Phone number',
      ),
      validator: (String value) {
        LogUtils.d(TAG, 'account change : $value');
        RegExp phoneReg = RegExp(r"1[0-9]\d{9}$");
        if (!phoneReg.hasMatch(value)) {
          return '请输入正确的手机号码';
        }
        return null;
      },
      onSaved: (String value) => _phone = value,
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

  Padding buildTitle() {
    return Padding(
      padding: EdgeInsets.all(8.0),
      child: Text(
        'Login',
        style: TextStyle(fontSize: 42.0),
      ),
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

  @override
  void onEvent(int mainCmd, int subCmd, Message msg) {
    if (mainCmd == MsgChannelUtil.MAIN_CMD_LOGIN) {
      if (msg != null) {
        if (msg.code == ResultCode.SUCCESS) {
          Navigator.pushAndRemoveUntil(context, new MaterialPageRoute(
            builder: (BuildContext context) {
              return new MyHomePage();
            },
          ), (route) => route == null);
        } else {
          loginErrorMessageController.showErrorMessage(msg.message ??= "登录失败");
        }
      } else {
        loginErrorMessageController.showErrorMessage("登录失败");
      }
    }
  }
}
