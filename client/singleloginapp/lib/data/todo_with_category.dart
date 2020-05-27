import 'package:flutter/cupertino.dart';
import 'package:singleloginapp/data/todo_database.dart';

class TodoWithCategory {
  final Todo todo;
  final Category category;

  TodoWithCategory({@required this.todo, @required this.category});
}
