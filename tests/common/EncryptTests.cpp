// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Encrypt.h"
#include "Data.h"
#include "HexCoding.h"

#include <TrustWalletCore/TWAESPaddingMode.h>

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Encrypt::test {

const Data gKey = parse_hex("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4");

inline void assertHexEqual(const Data& data, const char* expected) {
    EXPECT_EQ(hex(data), expected);
}

TEST(Encrypt, paddingSize) {
    EXPECT_EQ(paddingSize(0, 16, TWAESPaddingModeZero), 0ul);
    EXPECT_EQ(paddingSize(1, 16, TWAESPaddingModeZero), 15ul);
    EXPECT_EQ(paddingSize(8, 16, TWAESPaddingModeZero), 8ul);
    EXPECT_EQ(paddingSize(15, 16, TWAESPaddingModeZero), 1ul);
    EXPECT_EQ(paddingSize(16, 16, TWAESPaddingModeZero), 0ul);
    EXPECT_EQ(paddingSize(17, 16, TWAESPaddingModeZero), 15ul);
    EXPECT_EQ(paddingSize(24, 16, TWAESPaddingModeZero), 8ul);
    EXPECT_EQ(paddingSize(31, 16, TWAESPaddingModeZero), 1ul);
    EXPECT_EQ(paddingSize(32, 16, TWAESPaddingModeZero), 0ul);
    EXPECT_EQ(paddingSize(0, 16, TWAESPaddingModePKCS7), 16ul);
    EXPECT_EQ(paddingSize(1, 16, TWAESPaddingModePKCS7), 15ul);
    EXPECT_EQ(paddingSize(8, 16, TWAESPaddingModePKCS7), 8ul);
    EXPECT_EQ(paddingSize(15, 16, TWAESPaddingModePKCS7), 1ul);
    EXPECT_EQ(paddingSize(16, 16, TWAESPaddingModePKCS7), 16ul);
    EXPECT_EQ(paddingSize(17, 16, TWAESPaddingModePKCS7), 15ul);
    EXPECT_EQ(paddingSize(24, 16, TWAESPaddingModePKCS7), 8ul);
    EXPECT_EQ(paddingSize(31, 16, TWAESPaddingModePKCS7), 1ul);
    EXPECT_EQ(paddingSize(32, 16, TWAESPaddingModePKCS7), 16ul);
}

TEST(Encrypt, AESCBCEncrypt) {
	auto iv = parse_hex("000102030405060708090A0B0C0D0E0F");
    auto data = parse_hex("6bc1bee22e409f96e93d7e117393172a");

    auto encryptResult = AESCBCEncrypt(gKey, data, iv);
    assertHexEqual(encryptResult, "f58c4c04d6e5f1ba779eabfb5f7bfbd6");
}

TEST(Encrypt, AESCBCEncryptWithPadding) {
    const Data key = parse_hex("bf6cfdd852f79460981062f551f1dc3215b5e26609bc2a275d5b2da21798b489");
    const Data message = TW::data("secret message");
    {
        Data iv = parse_hex("f300888ca4f512cebdc0020ff0f7224c");        
        Data encrypted = AESCBCEncrypt(key, message, iv, TWAESPaddingModePKCS7);
        assertHexEqual(encrypted, "7f896315e90e172bed65d005138f224d");
    }
    {
        // with no padding
        Data iv = parse_hex("f300888ca4f512cebdc0020ff0f7224c");        
        Data encrypted = AESCBCEncrypt(key, message, iv, TWAESPaddingModeZero);
        assertHexEqual(encrypted, "11bcbfebb2db19fb5a5cbf458e0f699e");
    }
}

TEST(Encrypt, AESCBCDecrypt) {
	auto iv = parse_hex("000102030405060708090A0B0C0D0E0F");
    auto cipher = parse_hex("f58c4c04d6e5f1ba779eabfb5f7bfbd6");

    auto decryptResult = AESCBCDecrypt(gKey, cipher, iv);
    assertHexEqual(decryptResult, "6bc1bee22e409f96e93d7e117393172a");
}

TEST(Encrypt, AESCBCDecryptWithPadding) {
    const Data key = parse_hex("bf6cfdd852f79460981062f551f1dc3215b5e26609bc2a275d5b2da21798b489");
    {
        const Data encryptedPadded = parse_hex("7f896315e90e172bed65d005138f224d");
        Data iv = parse_hex("f300888ca4f512cebdc0020ff0f7224c");        
        Data encrypted = AESCBCDecrypt(key, encryptedPadded, iv, TWAESPaddingModePKCS7);
        assertHexEqual(encrypted, hex(TW::data("secret message")).c_str());
    }
    {
        // with no padding
        const Data encryptedNotPadded = parse_hex("11bcbfebb2db19fb5a5cbf458e0f699e");
        Data iv = parse_hex("f300888ca4f512cebdc0020ff0f7224c");        
        Data encrypted = AESCBCDecrypt(key, encryptedNotPadded, iv);
        Data expected = TW::data("secret message");
        TW::append(expected, 0);
        TW::append(expected, 0);
        assertHexEqual(encrypted, hex(expected).c_str());
    }
}

TEST(Encrypt, AESCTREncrypt) {
	auto iv = parse_hex("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
    auto data = parse_hex("6bc1bee22e409f96e93d7e117393172a");

    auto encryptResult = AESCTREncrypt(gKey, data, iv);
    assertHexEqual(encryptResult, "601ec313775789a5b7a7f504bbf3d228");
}

TEST(Encrypt, AESCTRDecrypt) {
	auto iv = parse_hex("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
    auto cipher = parse_hex("601ec313775789a5b7a7f504bbf3d228");

    auto decryptResult = AESCTRDecrypt(gKey, cipher, iv);
    assertHexEqual(decryptResult, "6bc1bee22e409f96e93d7e117393172a");
}

TEST(Encrypt, AESCBCEncryptMultipleBlocks) {
    auto key = parse_hex("e1094a016e6029eabc6f9e3c3cd9afb8");
    auto iv = parse_hex("884b972d70acece4ecf9b790ffce177e");
    auto data = parse_hex("726970706c652073636973736f7273206b69636b206d616d6d616c206869726520636f6c756d6e206f616b20616761696e2073756e206f66666572207765616c746820746f6d6f72726f77207761676f6e207475726e20666174616c00");

    auto result = AESCBCEncrypt(key, data, iv);
    assertHexEqual(result, "30e3ce939cdc80df375aaf6c2cdc7bc265f4eea20c90ab4825c5fc4b5c4517395ea1c28559bf0832a07f9a7fb8fc58786683a48aa8319be215a6b4a597eeaa443973b76401fe959c1bcb4991c9ee20b54c0244f8f43f0f0adcbb50e9ea913bf0");
}

TEST(Encrypt, AESCBCDecryptMultipleBlocks) {
    auto key = parse_hex("fac192ceb5fd772906bea3e118a69e8b");
    auto iv = parse_hex("83dbcc02d8ccb40e466191a123791e0e");
    auto data = parse_hex("d172bf743a674da9cdad04534d56926ef8358534d458fffccd4e6ad2fbde479c");

    auto decryptResult = AESCBCDecrypt(key, data, iv);
    assertHexEqual(decryptResult, "d4ade7189ee99ba50399e60a27c9e0fd02cfd1cfa2d15e7491329f361645d2a4");
}

TEST(Encrypt, AESCTRDecryptMultipleBlocks) {
    auto key = parse_hex("fac192ceb5fd772906bea3e118a69e8b");
    auto iv = parse_hex("83dbcc02d8ccb40e466191a123791e0e");
    auto data = parse_hex("d172bf743a674da9cdad04534d56926ef8358534d458fffccd4e6ad2fbde479c");

    auto decryptResult = AESCTRDecrypt(key, data, iv);
    assertHexEqual(decryptResult, "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d");
}

TEST(Encrypt, AESCTREncryptMultipleBlocks) {
    auto key = parse_hex("e1094a016e6029eabc6f9e3c3cd9afb8");
    auto iv = parse_hex("884b972d70acece4ecf9b790ffce177e");
    auto data = parse_hex("726970706c652073636973736f7273206b69636b206d616d6d616c206869726520636f6c756d6e206f616b20616761696e2073756e206f66666572207765616c746820746f6d6f72726f77207761676f6e207475726e20666174616c00");

    auto result = AESCTREncrypt(key, data, iv);
    assertHexEqual(result, "76b0a3ae037e7d6a50236c4c3ba7560edde4a8a951bf97bc10709e74d8e926c0431866b0ba9852d95bb0bbf41d109f1f3cf2f0af818f96d4f4109a1e3e5b224e3efd57288906a48d47b0006ccedcf96fde7362dedca952dda7cbdd359d");
}

TEST(Encrypt, AESCBCEncryptInvalidKeySize) {
    Data iv = Data(16);
    try {
        Data result = AESCBCEncrypt(Data(19), Data(100), iv);
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

TEST(Encrypt, AESCBCDecryptInvalidKeySize) {
    Data iv = Data(16);
    try {
        Data result = AESCBCDecrypt(Data(19), Data(100), iv);
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

TEST(Encrypt, AESCBCDecryptInvalidDataSize) {
    Data iv = Data(16);
    try {
        Data result = AESCBCDecrypt(Data(16), Data(100), iv);
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

TEST(Encrypt, AESCTREncryptInvalidKeySize) {
    Data iv = Data(16);
    try {
        Data result = AESCTREncrypt(Data(19), Data(100), iv);
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

TEST(Encrypt, AESCTRDecryptInvalidKeySize) {
    Data iv = Data(16);
    try {
        Data result = AESCTRDecrypt(Data(19), Data(100), iv);
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

} // namespace TW::Encrypt::tests
