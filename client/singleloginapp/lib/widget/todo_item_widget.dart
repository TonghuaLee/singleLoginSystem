import 'package:flutter/material.dart';
import 'package:flutter_slidable/flutter_slidable.dart';
import 'package:singleloginapp/data/todo_with_category.dart';
import 'package:singleloginapp/data/todos_dao.dart';

class TodoItemWidget extends StatelessWidget {
  final TodoWithCategory _todoItem;
  final TodosDao _todosDao;

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
            _todosDao.updateTodo(_todoItem.todo.copyWith(status: newValue));
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
}
