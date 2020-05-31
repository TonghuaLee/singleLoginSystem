import 'dart:convert' as JSON;

import 'package:flutter/material.dart';
import 'package:moor/moor.dart';
import 'package:provider/provider.dart';
import 'package:singleloginapp/controller/dabase_provider.dart';
import 'package:singleloginapp/data/todo_database.dart';
import 'package:singleloginapp/msg/event_listener.dart';
import 'package:singleloginapp/msg/message.dart';
import 'package:singleloginapp/msg/msg_channel.dart';
import 'package:singleloginapp/msg/result.dart';
import 'package:singleloginapp/utils/log_util.dart';

import 'animatedloginbutton.dart';

class NewTodoInput extends StatefulWidget {
  const NewTodoInput({Key key}) : super(key: key);

  @override
  State<StatefulWidget> createState() => _NewTodoInputState();
}

class _NewTodoInputState extends State<NewTodoInput> with EventListener {
  final String TAG = "_NewTodoInputState";
  TextEditingController controller;
  final LoginErrorMessageController loginErrorMessageController =
  LoginErrorMessageController();
  FocusNode focusNode;
  BuildContext _context;
  final _formKey = GlobalKey<FormState>();

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
    return Container(
      padding: EdgeInsets.symmetric(vertical: 16, horizontal: 8),
      child: Row(
        children: <Widget>[
          Expanded(
              child: Form(
                  key: _formKey,
                  child: TextFormField(
                    //输入模式
                    keyboardType: TextInputType.text,
                    controller: controller,
                    focusNode: focusNode,
                    style: TextStyle(
                      color: Colors.blue,
                      decorationColor: Colors.blue,
                      fontSize: 18,
                    ),
                    decoration: InputDecoration(
                      labelText: 'New TODO',
                    ),
                    validator: (String value) {
                      LogUtils.d(TAG, 'new todo change : $value');
                      if (value.isEmpty) {
                        return 'Category should not be empty!';
                      }
                      return null;
                    },
                  ))),
          new AnimatedLoginButton(
            loginErrorMessageController: loginErrorMessageController,
            loginTip: "Add",
            indicatorWidth: 1,
            height: 25,
            width: 50,
            showErrorTime: const Duration(milliseconds: 1000),
            buttonColorNormal: Colors.blue,
            onTap: () async {
              if (_formKey.currentState.validate()) {
                _requsetAddTodo(controller.text, context);
              } else {
                loginErrorMessageController.showErrorMessage("retry");
              }
            },
          ),
        ],
      ),
    );
  }

  void _requsetAddTodo(String input, BuildContext context) async {
    print('_requsetAddTodo! $input');
    var databaseProvider =
    Provider.of<DatabaseProvider>(context, listen: false);
    Map req = new Map();
    req['content'] = input;
    var cid = databaseProvider.selectedCategory.id;
    req['cid'] = cid; // databaseProvider.selectedCategory.id;
    Message msg = new Message(1, 'req add todo from flutter', req,
        MsgChannelUtil.MAIN_CMD_ADD_TODO, MsgChannelUtil.MAIN_CMD_DEFALUT);
    LogUtils.d(TAG, 'req: ' + msg.toString());
//              LogUtils.d(TAG, 'req from flutter: '+ msg.toJson());
    Message result = await MsgChannelUtil.getInstance().sendMessage(msg);
  }

  void _insertNewTodoItem(String input, int cid, BuildContext context) {
    print('submitted! $input');
    var databaseProvider =
    Provider.of<DatabaseProvider>(context, listen: false);
    databaseProvider.insertNewTodoItemWithCid(input, cid).then((_) {
      _resetValuesAfterSubmit();
    }).catchError(
          (e) {
        Scaffold.of(context).showSnackBar(
          SnackBar(
            content: Text(
              'Invalid data! Todo item should not be empty or too long!',
              style: TextStyle(
                color: Colors.red,
              ),
            ),
          ),
        );
      },
      test: (e) => e is InvalidDataException,
    );
  }

  @override
  void dispose() {
    controller.dispose();
    focusNode.dispose();
    super.dispose();
  }

  void _resetValuesAfterSubmit() {
    setState(() {
      controller.clear();
      focusNode.unfocus();
    });
    loginErrorMessageController.reset();
  }

  @override
  void onEvent(int mainCmd, int subCmd, Message msg) {
    if (mainCmd == MsgChannelUtil.MAIN_CMD_ADD_TODO) {
      LogUtils.d(TAG, msg.toJson().toString());
      var bSucc = false;
      if (msg != null) {
        if (msg.code == ResultCode.SUCCESS) {
          var data = msg.message;
          LogUtils.d(TAG, '添加TODO$data');
          var todoJson = JSON.jsonDecode(data);
          var tid = todoJson['tid'];
          var content = todoJson['content'];
          var cid = todoJson['cid'];
          var status = todoJson['status'];
          var todo = Todo(
              id: tid, title: content, category: cid, completed: status == 0 ? false : true);
          if (todo != null) {
            LogUtils.d(TAG, '添加todo成功');
            bSucc = true;
            _insertNewTodoItem(todo.title, todo.category, _context);
          }
        }

        if (!bSucc) {
          showDialog(
            context: _context,
            builder: (_) =>
                AlertDialog(
                  title: Text(
                    'Sorry,add todo fail.',
                    style: TextStyle(
                      color: Colors.red,
                    ),
                  ),
                ),
          );
          loginErrorMessageController.showErrorMessage("fail");
        }
      }
    }
  }
}
