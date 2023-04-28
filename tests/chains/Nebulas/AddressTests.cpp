// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Nebulas/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>

namespace TW::Nebulas::tests {

using namespace std;

TEST(NebulasAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("a1TgpFZWCMmFd2sphb6RKsCvsEyMCNa2Yyv"));
    ASSERT_FALSE(Address::isValid("n2TgpFZWCMmFd2sphb6RKsCvsEyMCNa2Yyv"));
    // normal address test
    ASSERT_TRUE(Address::isValid("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY"));
    // contract address test
    ASSERT_TRUE(Address::isValid("n1zUNqeBPvsyrw5zxp9mKcDdLTjuaEL7s39"));
}

TEST(NebulasAddress, String) {
    ASSERT_THROW(Address("abc"), std::invalid_argument);
    ASSERT_EQ(Address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY").string(),
              "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    ASSERT_EQ(Address(Base58::decode("n1TgpFZWCMmFd2sphb6RKsCvsEyMCNa2Yyv")).string(),
              "n1TgpFZWCMmFd2sphb6RKsCvsEyMCNa2Yyv");

    const auto address = Address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    ASSERT_EQ(address.string(), "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
}

TEST(NebulasAddress, Data) {
    Data data;
    EXPECT_THROW(Address(data).string(), std::invalid_argument);
    ASSERT_EQ(Address(Base58::decode("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY")).string(),
              "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
}

TEST(NebulasAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    const auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");

    EXPECT_THROW(Address(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1)), std::invalid_argument);
}

} // namespace TW::Nebulas::tests
