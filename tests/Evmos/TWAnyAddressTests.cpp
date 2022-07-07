// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

TEST(EvmosAnyAddress, EvmosValidate) {
    auto string = STRING("0x30627903124Aa1e71384bc52e1cb96E4AB3252b6");

    EXPECT_TRUE(TWAnyAddressIsValid(string.get(), TWCoinTypeEvmos));

    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeEvmos));
    
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "30627903124aa1e71384bc52e1cb96e4ab3252b6");    
}

TEST(EvmosAnyAddress, EvmosCreate) {
    auto publicKeyHex = "045a0c6b83b8bd9827e507270cadb499b7e3a9095246f6a2213281f783d877c98b256742741b0639f317768fe4f4c2762660c2112283a7685d815507dee3229173"; // shoot island position ...
    const auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyHex).get(), TWPublicKeyTypeSECP256k1Extended));

    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeEvmos));

    EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWAnyAddressDescription(addr.get())).get())), std::string("0x8f348F300873Fd5DA36950B2aC75a26584584feE"));

    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "8f348f300873fd5da36950b2ac75a26584584fee");
}

TEST(EvmosAnyAddress, NativeEvmosValidate) {
    auto string = STRING("evmos1mwspdc5y6lj0glm90j9sg7vmr5ksmqcnzhg0h9");

    EXPECT_TRUE(TWAnyAddressIsValid(string.get(), TWCoinTypeNativeEvmos));

    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeNativeEvmos));

    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "dba016e284d7e4f47f657c8b04799b1d2d0d8313");
}

TEST(EvmosAnyAddress, NativeEvmosCreate) {
    auto publicKeyHex = "035a0c6b83b8bd9827e507270cadb499b7e3a9095246f6a2213281f783d877c98b"; // shoot island position ...
    const auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyHex).get(), TWPublicKeyTypeSECP256k1));

    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeNativeEvmos));

    EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWAnyAddressDescription(addr.get())).get())), std::string("evmos1mwspdc5y6lj0glm90j9sg7vmr5ksmqcnzhg0h9"));

    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "dba016e284d7e4f47f657c8b04799b1d2d0d8313");
}
