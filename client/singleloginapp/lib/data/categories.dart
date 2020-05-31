import 'package:moor/moor.dart';

@DataClassName('Category')
class Categories extends Table {
  IntColumn get cid => integer().autoIncrement()();

  TextColumn get title => text().withLength(min: 1, max: 50)();

  IntColumn get uid => integer()();
}
