import 'dart:convert' as JSON;

import 'package:flutter/material.dart';
import 'package:flutter_slidable/flutter_slidable.dart';
import 'package:singleloginapp/data/todo_with_category.dart';
import 'package:singleloginapp/data/todos_dao.dart';
import 'package:singleloginapp/msg/event_listener.dart';
import 'package:singleloginapp/msg/message.dart';
import 'package:singleloginapp/msg/msg_channel.dart';
import 'package:singleloginapp/msg/result.dart';
import 'package:singleloginapp/utils/log_util.dart';

class TodoItemWidget extends StatelessWidget with EventListener {
  final TodoWithCategory _todoItem;
  final TodosDao _todosDao;
  final String TAG = "TodoItemWidget";

  TodoItemWidget(this._todoItem, this._todosDao);

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Slidable(
        actionExtentRatio: 0.2,
        actionPane: SlidableDrawerActionPane(),
        child: CheckboxListTile(
          title: Text(_todoItem.todo.content),
          subtitle: Text(
              'category ${_todoItem.category != null ? _todoItem.category.title : 'Inbox'}'),
          value: _todoItem.todo.status,
          onChanged: (newValue) {
            updateTodoStatus(_todoItem.todo.id, newValue);
          },
        ),
        secondaryActions: <Widget>[
          IconSlideAction(
            caption: 'Archive',
            color: Colors.blue,
            icon: Icons.archive,
            onTap: () {},
          ),
          IconSlideAction(
            caption: 'Share',
            color: Colors.indigo,
            icon: Icons.share,
            onTap: () {},
          ),
          IconSlideAction(
            caption: 'Delete',
            color: Colors.red,
            icon: Icons.remove,
            onTap: () {
              _todosDao.deleteTodo(_todoItem.todo);
            },
          ),
        ],
      ),
    );
  }

  updateTodoStatus(int tid, bool done) async {
    LogUtils.d(TAG, ('submitted! $tid bool done'));
    Map req = new Map();
    req['tid'] = tid;
    req['status'] = done ? 1 : 0;
    Message msg = new Message(
        1,
        'req update todo from flutter',
        req,
        MsgChannelUtil.MAIN_CMD_UPDATE_TODO_STATUS,
        MsgChannelUtil.MAIN_CMD_DEFALUT);
    LogUtils.d(TAG, 'req: ' + msg.toString());
//              LogUtils.d(TAG, 'req from flutter: '+ msg.toJson());
    Message result = await MsgChannelUtil.getInstance().sendMessage(msg);
  }

  @override
  void onEvent(int mainCmd, int subCmd, Message msg) {
    if (mainCmd == MsgChannelUtil.MAIN_CMD_UPDATE_TODO_STATUS) {
      LogUtils.d(TAG, msg.toJson().toString());
      if (msg != null) {
        if (msg.code == ResultCode.SUCCESS) {
          var data = msg.message;
          var todoJson = JSON.jsonDecode(data);
          var status = todoJson['status'] != 0;
          _todosDao.updateTodo(_todoItem.todo.copyWith(status: status));
        }
      }
    }
  }
}
