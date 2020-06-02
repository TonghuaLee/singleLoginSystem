// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'todo_database.dart';

// **************************************************************************
// MoorGenerator
// **************************************************************************

// ignore_for_file: unnecessary_brace_in_string_interps, unnecessary_this
class Todo extends DataClass implements Insertable<Todo> {
  final int id;
  final String content;
  final String desc;
  final int cid;
  final bool status;
  Todo(
      {@required this.id,
      @required this.content,
      this.desc,
      this.cid,
      @required this.status});
  factory Todo.fromData(Map<String, dynamic> data, GeneratedDatabase db,
      {String prefix}) {
    final effectivePrefix = prefix ?? '';
    final intType = db.typeSystem.forDartType<int>();
    final stringType = db.typeSystem.forDartType<String>();
    final boolType = db.typeSystem.forDartType<bool>();
    return Todo(
      id: intType.mapFromDatabaseResponse(data['${effectivePrefix}id']),
      content:
          stringType.mapFromDatabaseResponse(data['${effectivePrefix}content']),
      desc: stringType
          .mapFromDatabaseResponse(data['${effectivePrefix}description']),
      cid: intType.mapFromDatabaseResponse(data['${effectivePrefix}cid']),
      status:
          boolType.mapFromDatabaseResponse(data['${effectivePrefix}status']),
    );
  }
  factory Todo.fromJson(Map<String, dynamic> json,
      {ValueSerializer serializer}) {
    serializer ??= moorRuntimeOptions.defaultSerializer;
    return Todo(
      id: serializer.fromJson<int>(json['id']),
      content: serializer.fromJson<String>(json['content']),
      desc: serializer.fromJson<String>(json['desc']),
      cid: serializer.fromJson<int>(json['cid']),
      status: serializer.fromJson<bool>(json['status']),
    );
  }
  @override
  Map<String, dynamic> toJson({ValueSerializer serializer}) {
    serializer ??= moorRuntimeOptions.defaultSerializer;
    return <String, dynamic>{
      'id': serializer.toJson<int>(id),
      'content': serializer.toJson<String>(content),
      'desc': serializer.toJson<String>(desc),
      'cid': serializer.toJson<int>(cid),
      'status': serializer.toJson<bool>(status),
    };
  }

  @override
  TodosCompanion createCompanion(bool nullToAbsent) {
    return TodosCompanion(
      id: id == null && nullToAbsent ? const Value.absent() : Value(id),
      content: content == null && nullToAbsent
          ? const Value.absent()
          : Value(content),
      desc: desc == null && nullToAbsent ? const Value.absent() : Value(desc),
      cid: cid == null && nullToAbsent ? const Value.absent() : Value(cid),
      status:
          status == null && nullToAbsent ? const Value.absent() : Value(status),
    );
  }

  Todo copyWith({int id, String content, String desc, int cid, bool status}) =>
      Todo(
        id: id ?? this.id,
        content: content ?? this.content,
        desc: desc ?? this.desc,
        cid: cid ?? this.cid,
        status: status ?? this.status,
      );
  @override
  String toString() {
    return (StringBuffer('Todo(')
          ..write('id: $id, ')
          ..write('content: $content, ')
          ..write('desc: $desc, ')
          ..write('cid: $cid, ')
          ..write('status: $status')
          ..write(')'))
        .toString();
  }

  @override
  int get hashCode => $mrjf($mrjc(
      id.hashCode,
      $mrjc(content.hashCode,
          $mrjc(desc.hashCode, $mrjc(cid.hashCode, status.hashCode)))));
  @override
  bool operator ==(dynamic other) =>
      identical(this, other) ||
      (other is Todo &&
          other.id == this.id &&
          other.content == this.content &&
          other.desc == this.desc &&
          other.cid == this.cid &&
          other.status == this.status);
}

class TodosCompanion extends UpdateCompanion<Todo> {
  final Value<int> id;
  final Value<String> content;
  final Value<String> desc;
  final Value<int> cid;
  final Value<bool> status;
  const TodosCompanion({
    this.id = const Value.absent(),
    this.content = const Value.absent(),
    this.desc = const Value.absent(),
    this.cid = const Value.absent(),
    this.status = const Value.absent(),
  });
  TodosCompanion.insert({
    this.id = const Value.absent(),
    @required String content,
    this.desc = const Value.absent(),
    this.cid = const Value.absent(),
    this.status = const Value.absent(),
  }) : content = Value(content);
  TodosCompanion copyWith(
      {Value<int> id,
      Value<String> content,
      Value<String> desc,
      Value<int> cid,
      Value<bool> status}) {
    return TodosCompanion(
      id: id ?? this.id,
      content: content ?? this.content,
      desc: desc ?? this.desc,
      cid: cid ?? this.cid,
      status: status ?? this.status,
    );
  }
}

class $TodosTable extends Todos with TableInfo<$TodosTable, Todo> {
  final GeneratedDatabase _db;
  final String _alias;
  $TodosTable(this._db, [this._alias]);
  final VerificationMeta _idMeta = const VerificationMeta('id');
  GeneratedIntColumn _id;
  @override
  GeneratedIntColumn get id => _id ??= _constructId();
  GeneratedIntColumn _constructId() {
    return GeneratedIntColumn('id', $tableName, false,
        hasAutoIncrement: true, declaredAsPrimaryKey: true);
  }

  final VerificationMeta _contentMeta = const VerificationMeta('content');
  GeneratedTextColumn _content;
  @override
  GeneratedTextColumn get content => _content ??= _constructContent();
  GeneratedTextColumn _constructContent() {
    return GeneratedTextColumn('content', $tableName, false,
        minTextLength: 1, maxTextLength: 50);
  }

  final VerificationMeta _descMeta = const VerificationMeta('desc');
  GeneratedTextColumn _desc;
  @override
  GeneratedTextColumn get desc => _desc ??= _constructDesc();
  GeneratedTextColumn _constructDesc() {
    return GeneratedTextColumn(
      'description',
      $tableName,
      true,
    );
  }

  final VerificationMeta _cidMeta = const VerificationMeta('cid');
  GeneratedIntColumn _cid;
  @override
  GeneratedIntColumn get cid => _cid ??= _constructCid();
  GeneratedIntColumn _constructCid() {
    return GeneratedIntColumn(
      'cid',
      $tableName,
      true,
    );
  }

  final VerificationMeta _statusMeta = const VerificationMeta('status');
  GeneratedBoolColumn _status;
  @override
  GeneratedBoolColumn get status => _status ??= _constructStatus();
  GeneratedBoolColumn _constructStatus() {
    return GeneratedBoolColumn('status', $tableName, false,
        defaultValue: Constant(false));
  }

  @override
  List<GeneratedColumn> get $columns => [id, content, desc, cid, status];
  @override
  $TodosTable get asDslTable => this;
  @override
  String get $tableName => _alias ?? 'todos';
  @override
  final String actualTableName = 'todos';
  @override
  VerificationContext validateIntegrity(TodosCompanion d,
      {bool isInserting = false}) {
    final context = VerificationContext();
    if (d.id.present) {
      context.handle(_idMeta, id.isAcceptableValue(d.id.value, _idMeta));
    }
    if (d.content.present) {
      context.handle(_contentMeta,
          content.isAcceptableValue(d.content.value, _contentMeta));
    } else if (isInserting) {
      context.missing(_contentMeta);
    }
    if (d.desc.present) {
      context.handle(
          _descMeta, desc.isAcceptableValue(d.desc.value, _descMeta));
    }
    if (d.cid.present) {
      context.handle(_cidMeta, cid.isAcceptableValue(d.cid.value, _cidMeta));
    }
    if (d.status.present) {
      context.handle(
          _statusMeta, status.isAcceptableValue(d.status.value, _statusMeta));
    }
    return context;
  }

  @override
  Set<GeneratedColumn> get $primaryKey => {id};
  @override
  Todo map(Map<String, dynamic> data, {String tablePrefix}) {
    final effectivePrefix = tablePrefix != null ? '$tablePrefix.' : null;
    return Todo.fromData(data, _db, prefix: effectivePrefix);
  }

  @override
  Map<String, Variable> entityToSql(TodosCompanion d) {
    final map = <String, Variable>{};
    if (d.id.present) {
      map['id'] = Variable<int, IntType>(d.id.value);
    }
    if (d.content.present) {
      map['content'] = Variable<String, StringType>(d.content.value);
    }
    if (d.desc.present) {
      map['description'] = Variable<String, StringType>(d.desc.value);
    }
    if (d.cid.present) {
      map['cid'] = Variable<int, IntType>(d.cid.value);
    }
    if (d.status.present) {
      map['status'] = Variable<bool, BoolType>(d.status.value);
    }
    return map;
  }

  @override
  $TodosTable createAlias(String alias) {
    return $TodosTable(_db, alias);
  }
}

class Category extends DataClass implements Insertable<Category> {
  final int cid;
  final String title;
  final int uid;
  Category({@required this.cid, @required this.title, @required this.uid});
  factory Category.fromData(Map<String, dynamic> data, GeneratedDatabase db,
      {String prefix}) {
    final effectivePrefix = prefix ?? '';
    final intType = db.typeSystem.forDartType<int>();
    final stringType = db.typeSystem.forDartType<String>();
    return Category(
      cid: intType.mapFromDatabaseResponse(data['${effectivePrefix}cid']),
      title:
          stringType.mapFromDatabaseResponse(data['${effectivePrefix}title']),
      uid: intType.mapFromDatabaseResponse(data['${effectivePrefix}uid']),
    );
  }
  factory Category.fromJson(Map<String, dynamic> json,
      {ValueSerializer serializer}) {
    serializer ??= moorRuntimeOptions.defaultSerializer;
    return Category(
      cid: serializer.fromJson<int>(json['cid']),
      title: serializer.fromJson<String>(json['title']),
      uid: serializer.fromJson<int>(json['uid']),
    );
  }
  @override
  Map<String, dynamic> toJson({ValueSerializer serializer}) {
    serializer ??= moorRuntimeOptions.defaultSerializer;
    return <String, dynamic>{
      'cid': serializer.toJson<int>(cid),
      'title': serializer.toJson<String>(title),
      'uid': serializer.toJson<int>(uid),
    };
  }

  @override
  CategoriesCompanion createCompanion(bool nullToAbsent) {
    return CategoriesCompanion(
      cid: cid == null && nullToAbsent ? const Value.absent() : Value(cid),
      title:
          title == null && nullToAbsent ? const Value.absent() : Value(title),
      uid: uid == null && nullToAbsent ? const Value.absent() : Value(uid),
    );
  }

  Category copyWith({int cid, String title, int uid}) => Category(
        cid: cid ?? this.cid,
        title: title ?? this.title,
        uid: uid ?? this.uid,
      );
  @override
  String toString() {
    return (StringBuffer('Category(')
          ..write('cid: $cid, ')
          ..write('title: $title, ')
          ..write('uid: $uid')
          ..write(')'))
        .toString();
  }

  @override
  int get hashCode =>
      $mrjf($mrjc(cid.hashCode, $mrjc(title.hashCode, uid.hashCode)));
  @override
  bool operator ==(dynamic other) =>
      identical(this, other) ||
      (other is Category &&
          other.cid == this.cid &&
          other.title == this.title &&
          other.uid == this.uid);
}

class CategoriesCompanion extends UpdateCompanion<Category> {
  final Value<int> cid;
  final Value<String> title;
  final Value<int> uid;
  const CategoriesCompanion({
    this.cid = const Value.absent(),
    this.title = const Value.absent(),
    this.uid = const Value.absent(),
  });
  CategoriesCompanion.insert({
    this.cid = const Value.absent(),
    @required String title,
    @required int uid,
  })  : title = Value(title),
        uid = Value(uid);
  CategoriesCompanion copyWith(
      {Value<int> cid, Value<String> title, Value<int> uid}) {
    return CategoriesCompanion(
      cid: cid ?? this.cid,
      title: title ?? this.title,
      uid: uid ?? this.uid,
    );
  }
}

class $CategoriesTable extends Categories
    with TableInfo<$CategoriesTable, Category> {
  final GeneratedDatabase _db;
  final String _alias;
  $CategoriesTable(this._db, [this._alias]);
  final VerificationMeta _cidMeta = const VerificationMeta('cid');
  GeneratedIntColumn _cid;
  @override
  GeneratedIntColumn get cid => _cid ??= _constructCid();
  GeneratedIntColumn _constructCid() {
    return GeneratedIntColumn('cid', $tableName, false,
        hasAutoIncrement: true, declaredAsPrimaryKey: true);
  }

  final VerificationMeta _titleMeta = const VerificationMeta('title');
  GeneratedTextColumn _title;
  @override
  GeneratedTextColumn get title => _title ??= _constructTitle();
  GeneratedTextColumn _constructTitle() {
    return GeneratedTextColumn('title', $tableName, false,
        minTextLength: 1, maxTextLength: 50);
  }

  final VerificationMeta _uidMeta = const VerificationMeta('uid');
  GeneratedIntColumn _uid;
  @override
  GeneratedIntColumn get uid => _uid ??= _constructUid();
  GeneratedIntColumn _constructUid() {
    return GeneratedIntColumn(
      'uid',
      $tableName,
      false,
    );
  }

  @override
  List<GeneratedColumn> get $columns => [cid, title, uid];
  @override
  $CategoriesTable get asDslTable => this;
  @override
  String get $tableName => _alias ?? 'categories';
  @override
  final String actualTableName = 'categories';
  @override
  VerificationContext validateIntegrity(CategoriesCompanion d,
      {bool isInserting = false}) {
    final context = VerificationContext();
    if (d.cid.present) {
      context.handle(_cidMeta, cid.isAcceptableValue(d.cid.value, _cidMeta));
    }
    if (d.title.present) {
      context.handle(
          _titleMeta, title.isAcceptableValue(d.title.value, _titleMeta));
    } else if (isInserting) {
      context.missing(_titleMeta);
    }
    if (d.uid.present) {
      context.handle(_uidMeta, uid.isAcceptableValue(d.uid.value, _uidMeta));
    } else if (isInserting) {
      context.missing(_uidMeta);
    }
    return context;
  }

  @override
  Set<GeneratedColumn> get $primaryKey => {cid};
  @override
  Category map(Map<String, dynamic> data, {String tablePrefix}) {
    final effectivePrefix = tablePrefix != null ? '$tablePrefix.' : null;
    return Category.fromData(data, _db, prefix: effectivePrefix);
  }

  @override
  Map<String, Variable> entityToSql(CategoriesCompanion d) {
    final map = <String, Variable>{};
    if (d.cid.present) {
      map['cid'] = Variable<int, IntType>(d.cid.value);
    }
    if (d.title.present) {
      map['title'] = Variable<String, StringType>(d.title.value);
    }
    if (d.uid.present) {
      map['uid'] = Variable<int, IntType>(d.uid.value);
    }
    return map;
  }

  @override
  $CategoriesTable createAlias(String alias) {
    return $CategoriesTable(_db, alias);
  }
}

abstract class _$TodoDatabase extends GeneratedDatabase {
  _$TodoDatabase(QueryExecutor e) : super(SqlTypeSystem.defaultInstance, e);
  $TodosTable _todos;
  $TodosTable get todos => _todos ??= $TodosTable(this);
  $CategoriesTable _categories;
  $CategoriesTable get categories => _categories ??= $CategoriesTable(this);
  @override
  Iterable<TableInfo> get allTables => allSchemaEntities.whereType<TableInfo>();
  @override
  List<DatabaseSchemaEntity> get allSchemaEntities => [todos, categories];
}
