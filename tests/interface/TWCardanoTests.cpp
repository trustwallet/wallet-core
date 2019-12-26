// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCardanoAddress.h>
//#include <TrustWalletCore/TWCardanoSigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include "TWTestUtilities.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(TWCardano, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4").get()));
    ASSERT_NE(nullptr, privateKey.get());
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519Extended(privateKey.get());
    ASSERT_NE(nullptr, publicKey);
    ASSERT_EQ(64, publicKey->impl.bytes.size());
    auto address = WRAP(TWCardanoAddress, TWCardanoAddressCreateWithPublicKey(publicKey));
    auto addressString = WRAPS(TWCardanoAddressDescription(address.get()));
    assertStringsEqual(addressString, "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W");

    auto address2 = WRAP(TWCardanoAddress, TWCardanoAddressCreateWithString(STRING("Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W").get()));
    ASSERT_NE(nullptr, address2.get());
    auto address2String = WRAPS(TWCardanoAddressDescription(address2.get()));
    assertStringsEqual(address2String, "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W");

    ASSERT_TRUE(TWCardanoAddressEqual(address.get(), address2.get()));
}

/*
TEST(TWCardano, Sign) {
    // Add signer tests here
}
*/
