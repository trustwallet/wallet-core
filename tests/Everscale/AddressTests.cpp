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

//TEST(EverscaleAddress, Valid) {
//    ASSERT_TRUE(Address::isValid("__ADD_VALID_ADDRESS_HERE__"));
//
//    // TODO: Add more tests
//}
//
//TEST(EverscaleAddress, Invalid) {
//    ASSERT_FALSE(Address::isValid("__ADD_INVALID_ADDRESS_HERE__"));
//
//    // TODO: Add more tests
//}
//
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
//TEST(EverscaleAddress, FromString) {
//    auto address = Address("__ADD_VALID_ADDRESS_HERE__");
//    ASSERT_EQ(address.string(), "__ADD_SAME_VALID_ADDRESS_HERE__");
//}
