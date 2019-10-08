// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Filecoin/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Filecoin;

TEST(FilecoinAddress, Validation) {
    // empty address
    ASSERT_FALSE(Address::isValid(""));
    // invalid checksum
    ASSERT_FALSE(Address::isValid("t15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pppp"));
    // wrong length
    ASSERT_FALSE(Address::isValid("t15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq "));
    // Stellar address
    ASSERT_FALSE(Address::isValid("t15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7p"));

    ASSERT_TRUE(Address::isValid("f17uoq6tp427uzv7fztkbsnn64iwotfrristwpryy"));
}

TEST(FilecoinAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("24dee5d2ceda995013f3227628a75f57185c9b9a52b1415a7b87aecf340b923a")); //pirvate key in base64 "JN7l0s7amVAT8yJ2KKdfVxhcm5pSsUFae4euzzQLkjo="
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    ASSERT_EQ(address.string(), "f1gd53ycqr7kc7rub2dbi2cumx5e4nqbi4tu3mhuy");
}

TEST(FilecoinAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("03cef0bb63eca86361e50fbf85e89282d91f6caa4e4137c556cd112b3bcfa9e82e"), TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "f1gd53ycqr7kc7rub2dbi2cumx5e4nqbi4tu3mhuy");
}

TEST(FilecoinAddress, FromString) {
    auto address = Address("f15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq");
    ASSERT_EQ(address.string(), "f15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq");
}
