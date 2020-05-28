import 'dart:developer';

import 'package:singleloginapp/widget/animatedloginbutton.dart';
import 'package:flutter/material.dart';
import 'package:singleloginapp/msg/event_listener.dart';
import 'package:singleloginapp/msg/message.dart';
import 'package:singleloginapp/msg/msg_channel.dart';
import 'package:singleloginapp/msg/result.dart';
import 'package:singleloginapp/pages/login_page.dart';
import 'package:singleloginapp/utils/log_util.dart';

class RegisterPage extends StatefulWidget {
  @override
  _RegisterPageState createState() => _RegisterPageState();
}

class _RegisterPageState extends State<RegisterPage> with EventListener {
  final TAG = "RegisterPage";
  final _formKey = GlobalKey<FormState>();
  String _phone, _password;
  bool _isObscure = true;
  Color _eyeColor;
  final LoginErrorMessageController loginErrorMessageController =
      LoginErrorMessageController();

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    MsgChannelUtil.getInstance().addListener(this);
  }

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
                buildLoginText(context),
                SizedBox(height: 60.0),
                buildConfirmButton(context),
                SizedBox(height: 30.0),
              ],
            )));
  }

  Align buildConfirmButton(BuildContext context) {
    return Align(
      child: new AnimatedLoginButton(
        loginErrorMessageController: loginErrorMessageController,
        loginTip: "Confirm",
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
            req['account'] = _phone;
            req['password'] = _password;
            Message msg = new Message(
                1,
                'req register from flutter',
                req,
                MsgChannelUtil.MAIN_CMD_REGISTER,
                MsgChannelUtil.MAIN_CMD_DEFALUT);
            LogUtils.d(TAG, 'req: ' + msg.toString());
            Message result =
                await MsgChannelUtil.getInstance().sendMessage(msg);
            LogUtils.d(TAG, 'result: ' + result.toJson().toString());
          } else {
            loginErrorMessageController.showErrorMessage("参数错误");
          }
        },
      ),
    );
  }

  Padding buildLoginText(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.only(top: 8.0),
      child: Align(
        alignment: Alignment.centerRight,
        child: FlatButton(
          child: Text(
            'Sign in',
            style: TextStyle(fontSize: 14.0, color: Colors.grey),
          ),
          onPressed: () {
            Navigator.pushAndRemoveUntil(context, new MaterialPageRoute(
              builder: (BuildContext context) {
                return new LoginPage();
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
      initialValue: '16625205201',
      decoration: InputDecoration(
        labelText: 'Phone Number',
      ),
      validator: (String value) {
        RegExp phoneReg = RegExp(r"1[0-9]\d{9}$");
        if (!phoneReg.hasMatch(value)) {
          return '请输入正确的手机号码';
        }
        return null;
      },
      onSaved: (String value) => _phone = value,
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
        'Register',
        style: TextStyle(fontSize: 42.0),
      ),
    );
  }

  @override
  void onEvent(int mainCmd, int subCmd, Message msg) {
    // TODO: implement onEvent
    LogUtils.d(TAG, "onEvent:" + msg.toString());
    if (mainCmd == MsgChannelUtil.MAIN_CMD_REGISTER) {
      if (msg != null) {
        if (msg.code == ResultCode.SUCCESS) {
          Navigator.pushAndRemoveUntil(context, new MaterialPageRoute(
            builder: (BuildContext context) {
              return new LoginPage();
            },
          ), (route) => route == null);
        } else {
          loginErrorMessageController.showErrorMessage(msg.message ??= "注册失败");
        }
      } else {
        loginErrorMessageController.showErrorMessage("注册失败");
      }
    }
  }
}
