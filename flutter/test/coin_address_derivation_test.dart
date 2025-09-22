import 'package:flutter/wallet_core.dart';
import 'package:flutter/wallet_core_bindings.dart';
import 'package:test/test.dart';
import 'dart:ffi';
import 'package:ffi/ffi.dart';

void main() {
  test('derive addresses from phrase', () {
    final walletCore = load();
    final mnemonic =
        'shoot island position soft burden budget tooth cruel issue economy destroy above';

    final mnemonicTwString = walletCore.TWStringCreateWithUTF8Bytes(
      mnemonic.toNativeUtf8().cast<Char>(),
    );
    final passwordTwString = walletCore.TWStringCreateWithUTF8Bytes(
      "".toNativeUtf8().cast<Char>(),
    );

    final wallet = walletCore.TWHDWalletCreateWithMnemonic(
      mnemonicTwString,
      passwordTwString,
    );

    // Test a few key coins
    final coinsAndAddresses = [
      (
        TWCoinType.TWCoinTypeBitcoin,
        'bc1quvuarfksewfeuevuc6tn0kfyptgjvwsvrprk9d',
      ),
      (
        TWCoinType.TWCoinTypeEthereum,
        '0x8f348F300873Fd5DA36950B2aC75a26584584feE',
      ),
      (
        TWCoinType.TWCoinTypeBinance,
        'bnb12vtaxl9952zm6rwf7v8jerq74pvaf77fcmvzhw',
      ),
      (
        TWCoinType.TWCoinTypeCosmos,
        'cosmos142j9u5eaduzd7faumygud6ruhdwme98qsy2ekn',
      ),
    ];

    for (final coinAndAddress in coinsAndAddresses) {
      final coin = coinAndAddress.$1;
      final expectedAddress = coinAndAddress.$2;

      final address = walletCore.TWHDWalletGetAddressForCoin(wallet, coin);
      final addressStr = walletCore.TWStringUTF8Bytes(
        address,
      ).cast<Utf8>().toDartString();

      expect(addressStr, expectedAddress);

      walletCore.TWStringDelete(address);
    }

    walletCore.TWHDWalletDelete(wallet);
    walletCore.TWStringDelete(mnemonicTwString);
    walletCore.TWStringDelete(passwordTwString);
  });
}
