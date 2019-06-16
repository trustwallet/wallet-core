// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Nebulas/Address.h"
#include "../src/Base58.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Nebulas;

TEST(NebulasAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("n1TgpFZWCMmFd2sphb6RKsCvsEyMCNa2Ytt"));
    ASSERT_FALSE(Address::isValid("n1TgpFZWCMmFd2sphb6RKstttEyMCNa2Yyv"));
}

TEST(NebulasAddress, String) {
    ASSERT_EQ(Address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY").string(),
        "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    ASSERT_EQ(Address(Base58::bitcoin.decode("n1TgpFZWCMmFd2sphb6RKsCvsEyMCNa2Yyv")).string(),
        "n1TgpFZWCMmFd2sphb6RKsCvsEyMCNa2Yyv"
    );
    
    const auto address = Address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    ASSERT_EQ(address.string(), "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
}

TEST(NebulasAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
}

TEST(NebulasAddress, IsValid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_TRUE(Address::isValid("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY"));
}
