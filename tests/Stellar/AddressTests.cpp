// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Stellar/Address.h"
#include "Bitcoin/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Stellar;

TEST(StellarAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("0103E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D"), TWPublicKeyTypeED25519);
    const auto address = Address(publicKey);
    auto str = hex(address.bytes);
    ASSERT_EQ(string("GAB6EDWGWSRZUYUYCWXAFQFBHE5ZEJPDXCIMVZC3LH2C7IU35FTI2NOQ"), address.string());
}

TEST(StellarAddress, FromString) {
    string stellarAddress = "GAB6EDWGWSRZUYUYCWXAFQFBHE5ZEJPDXCIMVZC3LH2C7IU35FTI2NOQ";
    const auto address = Address(stellarAddress);
    ASSERT_EQ(address.string(), stellarAddress);
}

TEST(StellarAddress, isValid) {
    string stellarAddress = "GABQHYQOY22KHGTCTAK24AWAUE4TXERF4O4JBSXELNM7IL5CTPUWM3SC";
    string bitcoinAddress = "1Ma2DrB78K7jmAwaomqZNRMCvgQrNjE2QC";

    ASSERT_TRUE(Address::isValid(stellarAddress));
    ASSERT_FALSE(Address::isValid(bitcoinAddress));
}
