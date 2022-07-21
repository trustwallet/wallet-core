// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Everscale/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Everscale;

TEST(EverscaleAddress, Valid) {
    ASSERT_TRUE(Address::isValid("0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a"));
    ASSERT_TRUE(Address::isValid("83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a"));
}

TEST(EverscaleAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("83a0352908060fa87839195d8a763a8d9ab28f8fa41"));
    ASSERT_ANY_THROW(Address::isValid("2147483648:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a"));
}

TEST(EverscaleAddress, FromString) {
    auto address = Address("0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a");
    ASSERT_EQ(address.string(), "0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a");

    auto address_uppercase = Address("0:83A0352908060FA87839195D8A763A8D9AB28F8FA41468832B398A719CC6469A");
    ASSERT_EQ(address.string(), "0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a");
}

//TEST(EverscaleAddress, FromPrivateKey) {
//    // TODO: Check public key type, finalize implementation
//
//    auto privateKey = PrivateKey(parse_hex("__PRIVATE_KEY_DATA__"));
//    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
//    ASSERT_EQ(address.string(), "__ADD_RESULTING_ADDRESS_HERE__");
//}
//
//TEST(EverscaleAddress, FromPublicKey) {
//    // TODO: Check public key type, finalize implementation
//
//    auto publicKey = PublicKey(parse_hex("__PUBLIC_KEY_DATA__"), TWPublicKeyTypeED25519);
//    auto address = Address(publicKey);
//    ASSERT_EQ(address.string(), "__ADD_RESULTING_ADDRESS_HERE__");
//}
//

