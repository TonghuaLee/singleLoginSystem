import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:singleloginapp/controller/dabase_provider.dart';
import 'package:singleloginapp/data/categories_dao.dart';
import 'package:singleloginapp/data/todo_database.dart';
import 'package:singleloginapp/data/todo_with_category.dart';
import 'package:singleloginapp/data/todos_dao.dart';
import 'package:singleloginapp/msg/event_listener.dart';
import 'package:singleloginapp/msg/message.dart';
import 'package:singleloginapp/msg/msg_channel.dart';
import 'package:singleloginapp/utils/log_util.dart';
import 'package:singleloginapp/widget/new_category_input_widget.dart';
import 'package:singleloginapp/widget/new_todo_input_widget.dart';
import 'package:singleloginapp/widget/todo_item_widget.dart';

import 'login_page.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (_) => DatabaseProvider(),
      child: MaterialApp(
        title: 'Todos Need To Be Done',
        theme: ThemeData(
          primarySwatch: Colors.green,
          primaryColor: Colors.green,
        ),
        home: MyHomePage(),
      ),
    );
  }
}

class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> with EventListener {
  final TAG = "MyHomePageState";

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Todo'),
        actions: <Widget>[
          Row(
            children: <Widget>[
              Text('Hide completed'),
              Consumer<DatabaseProvider>(
                builder: (BuildContext context,
                        DatabaseProvider databaseProvider, Widget child) =>
                    Switch(
                  value: databaseProvider.hideCompleted,
                  onChanged: (value) {
                    print('changed $value');
                    databaseProvider.hideCompleted = value;
                  },
                ),
              ),
            ],
          ),
        ],
      ),
      drawer: _buildDrawer(context),
      body: Column(
        children: <Widget>[
          Expanded(
            child: _buildList(context),
          ),
          NewTodoInput(),
        ],
      ),
    );
  }

  @override
  void initState() {
    super.initState();
    MsgChannelUtil.getInstance().addListener(this);
  }

  Drawer _buildDrawer(BuildContext context) {
    CategoriesDao categoriesDao =
        Provider.of<DatabaseProvider>(context, listen: false).categoriesDao;
    return Drawer(
      child: StreamBuilder(
        stream: categoriesDao.watchAllCategories(),
        builder:
            (BuildContext context, AsyncSnapshot<List<Category>> snapshot) {
          List<Category> categories = snapshot.data ?? List();
          return ListView.builder(
            itemCount: categories.length + 3,
            itemBuilder: (BuildContext context, int index) {
              return _buildDrawerItem(context, categories, index);
            },
          );
        },
      ),
    );
  }

  Widget _buildDrawerItem(
      BuildContext context, List<Category> categories, int index) {
    var databaseProvider =
        Provider.of<DatabaseProvider>(context, listen: false);
    if (index == 0) {
      return DrawerHeader(
        decoration: BoxDecoration(
          color: Colors.blue,
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          crossAxisAlignment: CrossAxisAlignment.start,
          children: <Widget>[
            Text(
              'Todos Will Be Done',
              style: TextStyle(
                color: Colors.white,
                fontSize: 24,
              ),
            ),
            NewCategoryInput(),
          ],
        ),
      );
    } else if (index == 1) {
      return ListTile(
        leading: Icon(Icons.inbox),
        title: Text('Inbox'),
        selected: databaseProvider.selectedCategory == null,
        onTap: () {
          databaseProvider.setSelectedCategory(null);
          Navigator.pop(context);
        },
      );
    } else if (index == categories.length + 2) {
      return ListTile(
        leading: Icon(Icons.exit_to_app),
        title: Text("退出"),
        onTap: () async {
          // 通知后台退出
          Map req = new Map();
          Message msg = new Message(
              0,
              'req loginout from flutter',
              req,
              MsgChannelUtil.MAIN_CMD_LOGINOUT,
              MsgChannelUtil.MAIN_CMD_DEFALUT);
          Message result = await MsgChannelUtil.getInstance().sendMessage(msg);
          // 跳转到登录页
          Navigator.pushAndRemoveUntil(context, new MaterialPageRoute(
            builder: (BuildContext context) {
              return new LoginPage();
            },
          ), (route) => route == null);
        },
      );
    } else {
      Category category = categories[index - 2];
      var title = category.title;
      LogUtils.d(TAG, "login confirm btn $title");
      if(title == null) {
        title = "分类";
      }
      return ListTile(
        leading: Icon(Icons.inbox),
        title: Text(title),
        selected: databaseProvider.selectedCategory == category,
        onTap: () {
          databaseProvider.setSelectedCategory(category);
          Navigator.pop(context);
        },
        onLongPress: () {
          _showDeleteCategoryDialog(context, databaseProvider, category);
        },
      );
    }
  }

  void _showDeleteCategoryDialog(BuildContext context,
      DatabaseProvider databaseProvider, Category category) {
    showDialog(
      context: context,
      builder: (_) => AlertDialog(
        title: Text('DELETE'),
        content: Column(
          mainAxisSize: MainAxisSize.min,
          children: <Widget>[
            Text('Do you want to delete this category?'),
            SizedBox(
              height: 8,
            ),
            Text(
              'All the items under this category would be gone.',
              style: TextStyle(color: Colors.redAccent),
            ),
          ],
        ),
        actions: <Widget>[
          FlatButton(
            child: Text('NO'),
            onPressed: () {
              Navigator.pop(context);
            },
          ),
          FlatButton(
            child: Text(
              'YES',
              style: TextStyle(color: Colors.red),
            ),
            onPressed: () {
              databaseProvider.deleteCategory(category);
              Navigator.pop(context);
            },
          ),
        ],
      ),
    );
  }

  Widget _buildList(BuildContext context) {
    return Consumer<DatabaseProvider>(
      builder: (context, databaseProvider, child) => StreamBuilder(
        stream: databaseProvider.watchTodosInCategory(),
        builder: (BuildContext context,
            AsyncSnapshot<List<TodoWithCategory>> snapshot) {
          print('buid list for ${databaseProvider.selectedCategory}');
          final todosWithCategory = snapshot.data ?? List();
          return ListView.builder(
            itemCount: todosWithCategory.length,
            itemBuilder: (BuildContext context, int index) {
              final item = todosWithCategory[index];
              return _buildItem(context, item, databaseProvider.todosDao);
            },
          );
        },
      ),
    );
  }

  Widget _buildItem(
      BuildContext context, TodoWithCategory item, TodosDao todosDao) {
    return TodoItemWidget(item, todosDao);
  }

  @override
  void onEvent(int mainCmd, int subCmd, Message msg) async {
    if (mainCmd == MsgChannelUtil.MAIN_CMD_LOGINOUT) {
      if (subCmd == MsgChannelUtil.SUB_CMD_LOGINOUT_SERVER) {
        Map req = new Map();
        Message msg = new Message(0, 'req loginout from flutter', req,
            MsgChannelUtil.MAIN_CMD_CHECK_LOGIN_STATE, MsgChannelUtil.MAIN_CMD_DEFALUT);
        await MsgChannelUtil.getInstance().sendMessage(msg);
      }
      Navigator.pushAndRemoveUntil(context, new MaterialPageRoute(
        builder: (BuildContext context) {
          return new LoginPage();
        },
      ), (route) => route == null);
    }
  }
}
