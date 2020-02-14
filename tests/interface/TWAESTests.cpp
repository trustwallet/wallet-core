// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWAES.h>

#include <gtest/gtest.h>

auto key = DATA("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4");

TEST(TWAES, CBCEncrypt) {
	auto iv = DATA("000102030405060708090A0B0C0D0E0F");
    auto data = DATA("6bc1bee22e409f96e93d7e117393172a");

    auto encryptResult = WRAPD(TWAESCBCEncrypt(key.get(), data.get(), iv.get()));
    assertHexEqual(encryptResult, "f58c4c04d6e5f1ba779eabfb5f7bfbd6");
}

TEST(TWAES, CBCDecrypt) {
	auto iv = DATA("000102030405060708090A0B0C0D0E0F");
    auto cipher = DATA("f58c4c04d6e5f1ba779eabfb5f7bfbd6");

    auto decryptResult = WRAPD(TWAESCBCDecrypt(key.get(), cipher.get(), iv.get()));
    assertHexEqual(decryptResult, "6bc1bee22e409f96e93d7e117393172a");
}

TEST(TWAES, CTREncrypt) {
	auto iv = DATA("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
    auto data = DATA("6bc1bee22e409f96e93d7e117393172a");

    auto encryptResult = WRAPD(TWAESCTREncrypt(key.get(), data.get(), iv.get()));
    assertHexEqual(encryptResult, "601ec313775789a5b7a7f504bbf3d228");
}

TEST(TWAES, CTRDecrypt) {
	auto iv = DATA("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
    auto cipher = DATA("601ec313775789a5b7a7f504bbf3d228");

    auto decryptResult = WRAPD(TWAESCTRDecrypt(key.get(), cipher.get(), iv.get()));
    assertHexEqual(decryptResult, "6bc1bee22e409f96e93d7e117393172a");
}
