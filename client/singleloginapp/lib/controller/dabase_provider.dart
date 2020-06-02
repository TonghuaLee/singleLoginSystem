import 'package:flutter/material.dart';
import 'package:moor/moor.dart';
import 'package:singleloginapp/data/categories_dao.dart';
import 'package:singleloginapp/data/todo_database.dart';
import 'package:singleloginapp/data/todo_with_category.dart';
import 'package:singleloginapp/data/todos_dao.dart';

class DatabaseProvider extends ChangeNotifier {
  TodosDao _todosDao;
  CategoriesDao _categoriesDao;
  Category _selectedCategory;
  bool _hideCompleted = false;

  bool get hideCompleted => _hideCompleted;

  set hideCompleted(bool value) {
    _hideCompleted = value;
    notifyListeners();
  }

  Category get selectedCategory => _selectedCategory;

  TodosDao get todosDao => _todosDao;

  CategoriesDao get categoriesDao => _categoriesDao;

  DatabaseProvider() {
    TodoDatabase database = TodoDatabase();
    _todosDao = TodosDao(database);
    _categoriesDao = CategoriesDao(database);
  }

  void setSelectedCategory(Category category) {
    _selectedCategory = category;
    notifyListeners();
  }

  Future insertNewTodoItem(String title) {
    final todo = TodosCompanion(
        content: Value(title),
        status: Value(false),
        cid: _selectedCategory != null
            ? Value(_selectedCategory.cid)
            : Value.absent());
    return todosDao.insertTodo(todo);
  }

  Future insertNewTodoItemWithCid(String title, int cid) {
    final todo = TodosCompanion(
        content: Value(title),
        status: Value(false),
        cid: cid > 0
            ? Value(cid)
            : Value.absent());
    return todosDao.insertTodo(todo);
  }

  Stream<List<TodoWithCategory>> watchTodosInCategory() {
    return todosDao.watchTodosInCategory(_selectedCategory,
        hideCompleted: hideCompleted);
  }

  Future deleteCategory(Category category) {
    return categoriesDao.deleteCategory(category);
  }
}
