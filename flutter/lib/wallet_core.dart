import 'dart:ffi';
import 'dart:io';
import 'package:path/path.dart' as path;

import 'package:flutter/wallet_core_bindings.dart';

WalletCore load() {
  var libraryPath = path.join(
    Directory.current.path,
    'lib',
    'libTrustWalletCore.so',
  );

  if (Platform.isMacOS) {
    libraryPath = path.join(
      Directory.current.path,
      'lib',
      'libTrustWalletCore.dylib',
    );
  }

  if (Platform.isWindows) {
    libraryPath = path.join(
      Directory.current.path,
      'lib',
      'libTrustWalletCore.dll',
    );
  }

  return WalletCore(DynamicLibrary.open(libraryPath));
}
