// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWData.h>
#include "../interface/TWTestUtilities.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

TEST(TWCardano, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4").get()));
    ASSERT_NE(nullptr, privateKey.get());
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519Extended(privateKey.get()));
    ASSERT_NE(nullptr, publicKey.get());
    ASSERT_EQ(64, publicKey.get()->impl.bytes.size());
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeCardano));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "addr1s3tl64970vuthz2j0qkz7kd2ya5j3fxuhdnv333vu38e6c37e4dq80ek4raf7hs3adag2tzpuxz7895a2x8xde5f8jqa8lrjyuqfj5k50pm668");

    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("addr1s3tl64970vuthz2j0qkz7kd2ya5j3fxuhdnv333vu38e6c37e4dq80ek4raf7hs3adag2tzpuxz7895a2x8xde5f8jqa8lrjyuqfj5k50pm668").get(), TWCoinTypeCardano));
    ASSERT_NE(nullptr, address2.get());
    auto address2String = WRAPS(TWAnyAddressDescription(address2.get()));
    assertStringsEqual(address2String, "addr1s3tl64970vuthz2j0qkz7kd2ya5j3fxuhdnv333vu38e6c37e4dq80ek4raf7hs3adag2tzpuxz7895a2x8xde5f8jqa8lrjyuqfj5k50pm668");

    ASSERT_TRUE(TWAnyAddressEqual(address.get(), address2.get()));
}

TEST(TWCardano, SigningNotImplemented) {
    // not implemented, returns empty data
    auto result = WRAPD(TWAnySignerSign(WRAPD(TWDataCreateWithSize(0)).get(), TWCoinType::TWCoinTypeCardano));
    EXPECT_EQ(TWDataSize(result.get()), 0);
}
