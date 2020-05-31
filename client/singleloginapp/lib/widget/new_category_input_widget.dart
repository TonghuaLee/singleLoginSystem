import 'dart:convert' as JSON;

import 'package:flutter/material.dart';
import 'package:moor/moor.dart';
import 'package:provider/provider.dart';
import 'package:singleloginapp/controller/dabase_provider.dart';
import 'package:singleloginapp/data/categories_dao.dart';
import 'package:singleloginapp/data/todo_database.dart';
import 'package:singleloginapp/msg/event_listener.dart';
import 'package:singleloginapp/msg/message.dart';
import 'package:singleloginapp/msg/msg_channel.dart';
import 'package:singleloginapp/msg/result.dart';
import 'package:singleloginapp/utils/log_util.dart';
import 'package:singleloginapp/widget/animatedloginbutton.dart';

class NewCategoryInput extends StatefulWidget {
  const NewCategoryInput({Key key}) : super(key: key);

  @override
  State<StatefulWidget> createState() => _NewCategoryInputState();
}

class _NewCategoryInputState extends State<NewCategoryInput>
    with EventListener {
  final String TAG = "_NewCategoryInputState";
  final LoginErrorMessageController loginErrorMessageController =
      LoginErrorMessageController();
  TextEditingController controller;
  FocusNode focusNode;
  final _formKey = GlobalKey<FormState>();
  BuildContext _context;
  String _category;

  @override
  void initState() {
    super.initState();
    controller = TextEditingController();
    focusNode = new FocusNode();
    MsgChannelUtil.getInstance().addListener(this);
  }
  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
    _context = context;
  }
  @override
  Widget build(BuildContext context) {
    _context = context;
    return Container(
      child: Row(
        children: <Widget>[
          Expanded(
              child: Form(
                  key: _formKey,
                  child: TextFormField(
                    //输入模式，邮件
                    keyboardType: TextInputType.text,
                    controller: controller,
                    focusNode: focusNode,
                    style: TextStyle(
                      color: Colors.white,
                      decorationColor: Colors.white,
                      fontSize: 18,
                    ),
                    decoration: InputDecoration(
                      labelText: 'New Category',
                    ),
                    validator: (String value) {
                      LogUtils.d(TAG, 'new category change : $value');
                      if (value.isEmpty) {
                        return 'Category should not be empty!';
                      }
                      return null;
                    },
                    onSaved: (String value) => _category = value,
                  ))),
          SizedBox(width: 5.0),
          new AnimatedLoginButton(
            loginErrorMessageController: loginErrorMessageController,
            loginTip: "Add",
            indicatorWidth: 1,
            height: 25,
            width: 50,
            showErrorTime: const Duration(milliseconds: 1000),
            buttonColorNormal: Colors.black,
            onTap: () async {
              fetchCategoryList();
              if (_formKey.currentState.validate()) {
                _insertNewCategory(controller.text, context);
              } else {
                loginErrorMessageController.showErrorMessage("retry");
              }
            },
          ),
        ],
      ),
    );
  }

  void fetchCategoryList() async {
    Map req = new Map();
    Message msg = new Message(
        0,
        'req fetchCategoryList from flutter',
        req,
        MsgChannelUtil.MAIN_CMD_FETCH_CATEGORY_LIST,
        MsgChannelUtil.MAIN_CMD_DEFALUT);
    Message result = await MsgChannelUtil.getInstance().sendMessage(msg);
  }
  void _insertNewCategory(String input, BuildContext context) async {
    print('submitted! $input');
    Map req = new Map();
    req['title'] = input;
    Message msg = new Message(1, 'req login from flutter', req,
        MsgChannelUtil.MAIN_CMD_ADD_CATEGORY, MsgChannelUtil.MAIN_CMD_DEFALUT);
    LogUtils.d(TAG, 'req: ' + msg.toString());
//              LogUtils.d(TAG, 'req from flutter: '+ msg.toJson());
    Message result = await MsgChannelUtil.getInstance().sendMessage(msg);
  }

  void _resetValuesAfterSubmit() {
    setState(() {
      controller.clear();
      focusNode.unfocus();
    });
    loginErrorMessageController.reset();
  }

  @override
  void dispose() {
    controller.dispose();
    super.dispose();
  }

  @override
  void onEvent(int mainCmd, int subCmd, Message msg) {
    LogUtils.d(TAG, msg.toJson().toString());
    if (mainCmd == MsgChannelUtil.MAIN_CMD_ADD_CATEGORY) {
      var bSucc = false;
      if (msg != null) {
        if (msg.code == ResultCode.SUCCESS) {
          var data = msg.message;
          LogUtils.d(TAG, '添加分类$data');
          var category = Category.fromJson(JSON.jsonDecode(data));
          if (category != null) {
            LogUtils.d(TAG, '添加分类成功');
            bSucc = true;
            CategoriesDao categoriesDao =
                Provider.of<DatabaseProvider>(_context, listen: false)
                    .categoriesDao;

            categoriesDao
                .insertCategory(CategoriesCompanion(
                title: Value(category.title), uid: Value(category.uid)))
                .then(
                  (_) {
                _resetValuesAfterSubmit();
              },
            ).catchError((e) {
              showDialog(
                context: _context,
                builder: (_) => AlertDialog(
                  title: Text(
                    'Invalid data! Category should not be empty or too long!',
                    style: TextStyle(
                      color: Colors.red,
                    ),
                  ),
                ),
              );
            }, test: (e) => e is InvalidDataException);
          }
        }

        if (!bSucc) {
          loginErrorMessageController.showErrorMessage("fail");
        }
      }
    }
  }
}
