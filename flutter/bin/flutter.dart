import 'package:flutter/wallet_core.dart';
import 'package:flutter/wallet_core_bindings.dart';

import 'dart:ffi';
import 'package:ffi/ffi.dart';

void main(List<String> arguments) {
  final walletCore = load();

  final passwordTwString = walletCore.TWStringCreateWithUTF8Bytes(
    "".toNativeUtf8().cast<Char>(),
  );

  print("Creating a new HD wallet ... ");
  final walletNew = walletCore.TWHDWalletCreate(128, passwordTwString);
  print("done.");
  print(
    "Secret mnemonic for new wallet: '${walletCore.TWStringUTF8Bytes(walletCore.TWHDWalletMnemonic(walletNew)).cast<Utf8>().toDartString()}'.",
  );
  walletCore.TWHDWalletDelete(walletNew);

  // Alternative: Import wallet with existing recovery phrase (mnemonic)
  print("Importing an HD wallet from earlier ... ");
  final secretMnemonic = walletCore.TWStringCreateWithUTF8Bytes(
    "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal"
        .toNativeUtf8()
        .cast<Char>(),
  );
  final walletImp = walletCore.TWHDWalletCreateWithMnemonic(
    secretMnemonic,
    walletCore.TWStringCreateWithUTF8Bytes("".toNativeUtf8().cast<Char>()),
  );
  walletCore.TWStringDelete(secretMnemonic);
  print("done.");
  print(
    "Secret mnemonic for imported wallet: '${walletCore.TWStringUTF8Bytes(walletCore.TWHDWalletMnemonic(walletImp)).cast<Utf8>().toDartString()}'.",
  );

  // coin type: we use Ethereum
  const coinType = TWCoinType.TWCoinTypeEthereum;
  print(
    "Working with coin: ${walletCore.TWStringUTF8Bytes(walletCore.TWCoinTypeConfigurationGetName(coinType)).cast<Utf8>().toDartString()} ${walletCore.TWStringUTF8Bytes(walletCore.TWCoinTypeConfigurationGetSymbol(coinType)).cast<Utf8>().toDartString()}",
  );

  // Derive default address
  print("Obtaining default address ... ");
  final address = walletCore.TWStringUTF8Bytes(
    walletCore.TWHDWalletGetAddressForCoin(walletImp, coinType),
  ).cast<Utf8>().toDartString();
  print("done.");
  print("Default address:          '$address'");

  // Alternative: Derive address using default derivation path
  // Done in 2 steps: derive private key, then address from private key
  // Note that private key is passed around between the two calls by the wallet -- be always cautious when handling secrets, avoid the risk of leaking secrets
  print(
    "Default derivation path:  ${walletCore.TWStringUTF8Bytes(walletCore.TWCoinTypeDerivationPath(coinType)).cast<Utf8>().toDartString()}",
  );
  final secretPrivateKeyDefault = walletCore.TWHDWalletGetKeyForCoin(
    walletImp,
    coinType,
  );
  final addressDefault = walletCore.TWStringUTF8Bytes(
    walletCore.TWCoinTypeDeriveAddress(coinType, secretPrivateKeyDefault),
  ).cast<Utf8>().toDartString();
  print("Address from default key: '$addressDefault'");

  // Alternative: Derive address using custom derivation path
  final customDerivationPath = walletCore.TWStringCreateWithUTF8Bytes(
    "m/44'/60'/1'/0/0".toNativeUtf8().cast<Char>(),
  );
  final secretPrivateKeyCustom = walletCore.TWHDWalletGetKey(
    walletImp,
    coinType,
    customDerivationPath,
  );
  walletCore.TWStringDelete(customDerivationPath);
  final addressCustom = walletCore.TWStringUTF8Bytes(
    walletCore.TWCoinTypeDeriveAddress(coinType, secretPrivateKeyCustom),
  ).cast<Utf8>().toDartString();
  print("Custom-derived address:   '$addressCustom'");
  print("");

  print(
    "RECEIVE funds: Perform send from somewhere else to this address: $address",
  );
  print("");

  // Steps for sending:
  // 1. put together a send message (contains sender and receiver address, amount, gas price, etc.)
  // 2. sign this message
  // 3. broadcast this message to the P2P network -- not done in this sample
  print("SEND funds:");
  const dummyReceiverAddress = "0xC37054b3b48C3317082E7ba872d7753D13da4986";
  final secretPrivKey = walletCore.TWPrivateKeyData(secretPrivateKeyDefault);

  print("preparing transaction (using AnySigner) ... ");
  const chainIdB64 = "AQ=="; // base64(parse_hex("01"))
  const gasPriceB64 =
      "1pOkAA=="; // base64(parse_hex("d693a4")) decimal 3600000000
  const gasLimitB64 = "Ugg="; // base64(parse_hex("5208")) decimal 21000
  const amountB64 =
      "A0i8paFgAA=="; // base64(parse_hex("0348bca5a160"))  924400000000000
  final transaction =
      "{"
      "\"chainId\":\"$chainIdB64"
      "\",\"gasPrice\":\"$gasPriceB64"
      "\",\"gasLimit\":\"$gasLimitB64"
      "\",\"toAddress\":\"$dummyReceiverAddress"
      "\",\"transaction\":{\"transfer\":{\"amount\":\"$amountB64"
      "\"}}}";
  print("transaction: $transaction");

  print("signing transaction ... ");
  final json = walletCore.TWStringCreateWithUTF8Bytes(
    transaction.toNativeUtf8().cast<Char>(),
  );
  final result = walletCore.TWAnySignerSignJSON(
    json,
    secretPrivKey,
    TWCoinType.TWCoinTypeEthereum,
  );
  final signedTransaction = walletCore.TWStringUTF8Bytes(
    result,
  ).cast<Utf8>().toDartString();
  print("done");
  print(
    "Signed transaction data (to be broadcast to network):  (len ${signedTransaction.length}) '$signedTransaction'",
  );
  // see e.g. https://github.com/flightwallet/decode-eth-tx for checking binary output content
  print("");
  walletCore.TWStringDelete(json);
  walletCore.TWStringDelete(result);

  print("Bye!");
  walletCore.TWHDWalletDelete(walletImp);

  walletCore.TWStringDelete(passwordTwString);
}
