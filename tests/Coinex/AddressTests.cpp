// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Cosmos/Address.h"

#include <gtest/gtest.h>
#include <TrustWalletCore/TWHDWallet.h>

namespace TW::Cosmos {

TEST(Address, CoinexValid) {
    ASSERT_TRUE(Address::isValid("cettest1pxf3sgcxpu99e4havmeus7gmmt8u3j9j6ra6me"));
}

TEST(Address, CoinexInvalid) {
    ASSERT_FALSE(Address::isValid("cettest1pxf3sgcxpu99e4havmeus7gmmt8u3j9j6ra63"));
}

TEST(Address, CoinexFromKeyHash) {
    auto privateKey = PrivateKey(parse_hex("c4c4f50e9af57eb281f07fb5e4ebb76d7ca1132549ffaa3aba3862fe58653244"));
    auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(hex(publicKeyData.bytes.begin(), publicKeyData.bytes.end()), "0232c6d612a131d09ba11fd3e1f93c89613e9da9bc215cc0e2322e3073b8049bdd");

    auto publicKey = PublicKey(publicKeyData);
    auto address = Address("coinex", publicKey);
    ASSERT_EQ(address.string(), "coinex178w9m7yzkagpq090t593r6gqe5gkfkvjqjdxsw");
    auto address_2 = Address("cettest", publicKey);
    ASSERT_EQ(address_2.string(), "cettest178w9m7yzkagpq090t593r6gqe5gkfkvjl76znl");
}

} // namespace TW::Cosmos
