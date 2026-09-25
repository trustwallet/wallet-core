import 'package:flutter/wallet_core.dart';
import 'package:test/test.dart';
import 'dart:ffi';
import 'package:ffi/ffi.dart';

void main() {
  test('twstring', () {
    final walletCore = load();

    final twString = walletCore.TWStringCreateWithUTF8Bytes(
      'Hello, World!'.toNativeUtf8().cast<Char>(),
    );
    final bytes = walletCore.TWStringUTF8Bytes(twString);
    expect(bytes.cast<Utf8>().toDartString(), 'Hello, World!');

    final size = walletCore.TWStringSize(twString);
    expect(size, 13);

    walletCore.TWStringDelete(twString);
  });
}
