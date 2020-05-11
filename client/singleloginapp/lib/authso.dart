import 'dart:ffi';
import 'dart:io';

class AuthSo {
  static int add(int a, int b) {
    return a + b; //nativeAdd(a, b);
  }
}

final DynamicLibrary dylib = Platform.isAndroid
    ? DynamicLibrary.open("libauthso.so")
    : DynamicLibrary.open("native_add.framework/native_add");

final int Function(int x, int y) nativeAdd = dylib
    .lookup<NativeFunction<Int32 Function(Int32, Int32)>>("native_add")
    .asFunction();
