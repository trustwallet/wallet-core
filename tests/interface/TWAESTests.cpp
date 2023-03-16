// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAES.h>

#include <gtest/gtest.h>

auto key = DATA("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4");
auto key2 = DATA("bbc82a01ebdb14698faee4a9e5038de72c995a9f6bcdb21903d62408b0c5ca96");

TEST(TWAES, CBCEncryptZeroPadding) {
	auto iv = DATA("000102030405060708090A0B0C0D0E0F");
    auto data = DATA("6bc1bee22e409f96e93d7e117393172a");

    auto encryptResult = WRAPD(TWAESEncryptCBC(key.get(), data.get(), iv.get(), TWAESPaddingModeZero));
    assertHexEqual(encryptResult, "f58c4c04d6e5f1ba779eabfb5f7bfbd6");
}

TEST(TWAES, CBCDecryptZeroPadding) {
	auto iv = DATA("000102030405060708090A0B0C0D0E0F");
    auto cipher = DATA("f58c4c04d6e5f1ba779eabfb5f7bfbd6");

    auto decryptResult = WRAPD(TWAESDecryptCBC(key.get(), cipher.get(), iv.get(), TWAESPaddingModeZero));
    assertHexEqual(decryptResult, "6bc1bee22e409f96e93d7e117393172a");
}

TEST(TWAES, CBCEncryptPKCS7Padding) {
	auto iv = DATA("37f8687086d31852979e228f4a97925b");
    auto data = DATA("7b226a736f6e727063223a22322e30222c226964223a313535343334333833343735323434362c226572726f72223a7b22636f6465223a2d33323030302c226d657373616765223a2253657373696f6e2052656a6563746564227d7d");

    auto encryptResult = WRAPD(TWAESEncryptCBC(key2.get(), data.get(), iv.get(), TWAESPaddingModePKCS7));
    assertHexEqual(encryptResult, "23c75d1b3228742ddb12eeef5a5016e37a8980a77fabc6dd01e6a355d88851c611d37e0d17a2f9c30f659da6d42ba77aca9b84bd6a95e3924f47d9093fbf16e0fb55b165ec193489645b4f7d2573959305c8fa70f88fe5affc43e3084a5878d1");
}

TEST(TWAES, CBCDecryptPKCS7Padding) {
	auto iv = DATA("debb62725b21c7577e4e498e10f096c7");
    auto cipher = DATA("e7df9810ce66defcc03023ee945f5958c1d4697bf97945daeab5059c2bc6262642cbca82982ac690e77e16671770c200f348f743a7c6e5df5c74eb892ef9b45a9b5ddf0f08fa60c49e5b694688d1b0b521b43975e65b4e8d557a83f4d1aab0af");

    auto decryptResult = WRAPD(TWAESDecryptCBC(key2.get(), cipher.get(), iv.get(), TWAESPaddingModePKCS7));
    assertHexEqual(decryptResult, "7b226a736f6e727063223a22322e30222c226964223a313535343334333833343735323434362c226572726f72223a7b22636f6465223a2d33323030302c226d657373616765223a2253657373696f6e2052656a6563746564227d7d");
}

TEST(TWAES, CTREncrypt) {
	auto iv = DATA("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
    auto data = DATA("6bc1bee22e409f96e93d7e117393172a");

    auto encryptResult = WRAPD(TWAESEncryptCTR(key.get(), data.get(), iv.get()));
    assertHexEqual(encryptResult, "601ec313775789a5b7a7f504bbf3d228");
}

TEST(TWAES, CTRDecrypt) {
	auto iv = DATA("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
    auto cipher = DATA("601ec313775789a5b7a7f504bbf3d228");

    auto decryptResult = WRAPD(TWAESDecryptCTR(key.get(), cipher.get(), iv.get()));
    assertHexEqual(decryptResult, "6bc1bee22e409f96e93d7e117393172a");
}
