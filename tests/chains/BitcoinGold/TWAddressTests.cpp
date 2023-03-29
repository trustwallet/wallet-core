// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include "TestUtilities.h"
#include "Bitcoin/Address.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"
#include "Coin.h"
#include <gtest/gtest.h>
#include <TrustWalletCore/TWAnyAddress.h>

using namespace TW;

const char *PRIVATE_KEY = "CA6D1402199530A5D610A01A53505B6A344CF61B0CCB2902D5AEFBEA63C274BB";
const char *ADDRESS = "GSGUyooxtCUVBonYV8AANp7FvKy3WTvpMR";
const char *FAKEADDRESS =  "GSGUyooxtCUVBonYV9AANp7FvKy3WTvpMR";

TEST(TWBitcoinGoldAddress, Valid) {
    ASSERT_TRUE(Bitcoin::Address::isValid(std::string(ADDRESS)));
    ASSERT_FALSE(Bitcoin::Address::isValid(std::string(FAKEADDRESS)));
}

TEST(TWBitcoinGoldAddress, PubkeyToAddress) {
    const auto privateKey = PrivateKey(parse_hex(PRIVATE_KEY));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

    /// construct with public key
    auto address = Bitcoin::Address(PublicKey(publicKey), p2pkhPrefix(TWCoinTypeBitcoinGold));
    ASSERT_EQ(address.string(), ADDRESS);
}
