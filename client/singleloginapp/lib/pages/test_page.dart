import 'package:flutter/material.dart';

class TestPage extends StatelessWidget {
  final List<ListItem> listData = [];

  void initData(BuildContext context) {
    listData.clear();

    listData.add(ListItem("Dart ffi", "DartffiPage", Icons.landscape));
    listData.add(ListItem("Djinni", "DjinniPage", Icons.web));
    listData.add(ListItem("gRPC", "gRPCPage", Icons.web));
  }

  @override
  Widget build(BuildContext context) {
    initData(context);
    return Scaffold(
      appBar: AppBar(
        title: Text('资料学习导航'),
        centerTitle: true,
      ),
      body: ListView.builder(
        itemBuilder: (BuildContext context, int index) {
          return ListItemWidget(listData[index]);
        },
        itemCount: listData.length,
      ),
    );
  }
}

class ListItem {
  final String title;
  final String routeName;
  final IconData iconData;

  ListItem(this.title, this.routeName, this.iconData);
}

class ListItemWidget extends StatelessWidget {
  final ListItem listItem;

  ListItemWidget(this.listItem);

  @override
  Widget build(BuildContext context) {
    return ListTile(
        title: Text(listItem.title),
        leading: Icon(listItem.iconData),
        trailing: Icon(Icons.arrow_forward),
        onTap: () {
          Navigator.pushNamed(context, listItem.routeName);
        });
  }
}
