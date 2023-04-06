// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Cosmos/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

namespace TW::Cosmos::tests {

TEST(StargazeAddress, IsValid) {
    ASSERT_TRUE(Address::isValid(TWCoinTypeStargaze, "stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy"));
    ASSERT_FALSE(Address::isValid(TWCoinTypeStargaze, "cosmos1kd5q7qejlqz94kpmd9pvr4v2gzgnca3lchdmcs"));
}

TEST(StargazeAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433"));
    auto address = Address(TWCoinTypeStargaze, privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    ASSERT_EQ(address.string(), "stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy");
}

TEST(StargazeAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("02cbfdb5e472893322294e60cf0883d43df431e1089d29ecb447a9e6d55045aae5"), TWPublicKeyTypeSECP256k1);
    auto address = Address(TWCoinTypeStargaze, publicKey);
    ASSERT_EQ(address.string(), "stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy");
}

} // namespace TW::Stargaze::tests
