// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "gtest/gtest.h"
#include "TWTestUtilities.h"

#include <TrustWalletCore/TWAES.h>

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

TEST(TWAES, CBCEncryptMultipleBlocks) {
    auto key = DATA("e1094a016e6029eabc6f9e3c3cd9afb8");
    auto iv = DATA("884b972d70acece4ecf9b790ffce177e");
    auto data = DATA("726970706c652073636973736f7273206b69636b206d616d6d616c206869726520636f6c756d6e206f616b20616761696e2073756e206f66666572207765616c746820746f6d6f72726f77207761676f6e207475726e20666174616c00");

    auto result = WRAPD(TWAESCBCEncrypt(key.get(), data.get(), iv.get()));
    assertHexEqual(result, "30e3ce939cdc80df375aaf6c2cdc7bc265f4eea20c90ab4825c5fc4b5c4517395ea1c28559bf0832a07f9a7fb8fc58786683a48aa8319be215a6b4a597eeaa443973b76401fe959c1bcb4991c9ee20b54c0244f8f43f0f0adcbb50e9ea913bf0");
}

TEST(TWAES, CBCDecryptMultipleBlocks) {
    auto key = DATA("fac192ceb5fd772906bea3e118a69e8b");
    auto iv = DATA("83dbcc02d8ccb40e466191a123791e0e");
    auto data = DATA("d172bf743a674da9cdad04534d56926ef8358534d458fffccd4e6ad2fbde479c");

    auto decryptResult = WRAPD(TWAESCBCDecrypt(key.get(), data.get(), iv.get()));
    assertHexEqual(decryptResult, "d4ade7189ee99ba50399e60a27c9e0fd02cfd1cfa2d15e7491329f361645d2a4");
}

TEST(TWAES, CTRDecryptMultipleBlocks) {
    auto key = DATA("fac192ceb5fd772906bea3e118a69e8b");
    auto iv = DATA("83dbcc02d8ccb40e466191a123791e0e");
    auto data = DATA("d172bf743a674da9cdad04534d56926ef8358534d458fffccd4e6ad2fbde479c");

    auto decryptResult = WRAPD(TWAESCTRDecrypt(key.get(), data.get(), iv.get()));
    assertHexEqual(decryptResult, "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d");
}

TEST(TWAES, CTREncryptMultipleBlocks) {
    auto key = DATA("e1094a016e6029eabc6f9e3c3cd9afb8");
    auto iv = DATA("884b972d70acece4ecf9b790ffce177e");
    auto data = DATA("726970706c652073636973736f7273206b69636b206d616d6d616c206869726520636f6c756d6e206f616b20616761696e2073756e206f66666572207765616c746820746f6d6f72726f77207761676f6e207475726e20666174616c00");

    auto result = WRAPD(TWAESCTREncrypt(key.get(), data.get(), iv.get()));
    assertHexEqual(result, "76b0a3ae037e7d6a50236c4c3ba7560edde4a8a951bf97bc10709e74d8e926c0431866b0ba9852d95bb0bbf41d109f1f3cf2f0af818f96d4f4109a1e3e5b224e3efd57288906a48d47b0006ccedcf96fde7362dedca952dda7cbdd359d");
}
