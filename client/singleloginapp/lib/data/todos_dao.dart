import 'package:moor/moor.dart';
import "package:singleloginapp/data/categories.dart";
import 'package:singleloginapp/data/todo_database.dart';
import 'package:singleloginapp/data/todo_with_category.dart';

part 'todos_dao.g.dart';

// the _TodosDaoMixin will be created by moor. It contains all the necessary
// fields for the tables. The <MyDatabase> type annotation is the database class
// that should use this dao.
@UseDao(tables: [Todos, Categories])
class TodosDao extends DatabaseAccessor<TodoDatabase> with _$TodosDaoMixin {
  // this constructor is required so that the main database can create an instance
  // of this object.
  TodosDao(TodoDatabase db) : super(db);

  Future<List<Todo>> getAllTodos() => select(todos).get();

  Stream<List<TodoWithCategory>> watchTodosInCategory(Category category,
      {bool hideCompleted = false}) {
    final query = select(todos).join([
      leftOuterJoin(categories, categories.cid.equalsExp(todos.cid)),
    ]);

    print('watch category: $category');
    if (category != null) {
      if (hideCompleted) {
        query.where(
            categories.cid.equals(category.cid) & todos.status.equals(false));
      } else {
        query.where(categories.cid.equals(category.cid));
      }
    } else {
      if (hideCompleted) {
        query.where(isNull(categories.cid) & todos.status.equals(false));
      } else {
        query.where(isNull(categories.cid));
      }
    }

    return query.watch().map((rows) {
      return rows.map((row) {
        return TodoWithCategory(
          todo: row.readTable(todos),
          category: row.readTable(categories),
        );
      }).toList();
    });
  }

  Stream<List<TodoWithCategory>> watchAllTodos() {
    final query = select(todos).join([
      leftOuterJoin(categories, categories.cid.equalsExp(todos.cid)),
    ]);

    return query.watch().map((rows) {
      return rows.map((row) {
        return TodoWithCategory(
          todo: row.readTable(todos),
          category: row.readTable(categories),
        );
      }).toList();
    });
  }

  Future insertTodo(Insertable<Todo> todo) => into(todos).insert(todo);

  Future updateTodo(Todo todo) => update(todos).replace(todo);

  Future deleteTodo(Todo todo) => delete(todos).delete(todo);

  Future clearTodos() =>
      delete(todos).go();
}
