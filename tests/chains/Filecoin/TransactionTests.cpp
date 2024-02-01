// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Filecoin/Address.h"
#include "Filecoin/Transaction.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

namespace TW::Filecoin {

TEST(FilecoinTransaction, EncodeBigInt) {
    ASSERT_EQ(hex(encodeBigInt(0)), "");
    ASSERT_EQ(hex(encodeBigInt(1)), "0001");
    ASSERT_EQ(hex(encodeBigInt(16)), "0010");
    ASSERT_EQ(hex(encodeBigInt(1111111111111)), "000102b36211c7");
    uint256_t reallyBig = 1;
    reallyBig <<= 128;
    ASSERT_EQ(hex(encodeBigInt(reallyBig)), "000100000000000000000000000000000000");
}

TEST(FilecoinTransaction, Serialize) {
    const PrivateKey privateKey(
        parse_hex("2f0f1d2c8de955c7c3fb4d9cae02539fadcb13fa998ccd9a1e871bed95f1941e"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    const Address fromAddress = Address::secp256k1Address(publicKey);
    const Address toAddress("f1hvadvq4rd2pyayrigjx2nbqz2nvemqouslw4wxi");

    Transaction tx(toAddress, fromAddress,
                   /*nonce*/ 0x1234567890,
                   /*value*/ 1000,
                   /*gasLimit*/ 3333333333,
                   /*gasFeeCap*/ 11111111,
                   /*gasPremium*/ 333333,
                   /*method*/ Transaction::MethodType::SEND,
                   /*params*/ Data());

    ASSERT_EQ(hex(tx.message().encoded()),
              "8a0055013d403ac3911e9f806228326fa68619d36a4641d455013d413d4c3fe3d89f99495a48c6046224"
              "a71f0cd71b0000001234567890430003e81ac6aea1554400a98ac744000516150040");
    ASSERT_EQ(hex(tx.cid()),
              "0171a0e40220a3b06c2837a94e3a431a78b00536d0298455ceec3d304adf26a3868147c4e6e1");
}

} // namespace TW::Filecoin
