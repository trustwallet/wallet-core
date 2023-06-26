// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "Stellar/Address.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::Stellar::tests {

TEST(StellarAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("0103E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D"), TWPublicKeyTypeED25519);
    const auto address = Address(publicKey);
    auto str = hex(address.bytes);
    ASSERT_EQ(string("GAB6EDWGWSRZUYUYCWXAFQFBHE5ZEJPDXCIMVZC3LH2C7IU35FTI2NOQ"), address.string());

    const auto privateKey = PrivateKey(parse_hex("94d1a980d5e528067d44bf8a60d646f556e40ca71e17cd4ead2d56f89e4bd20f"));
    const auto publicKey2 = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    EXPECT_ANY_THROW(new Address(publicKey2));
}

TEST(StellarAddress, FromString) {
    string stellarAddress = "GAB6EDWGWSRZUYUYCWXAFQFBHE5ZEJPDXCIMVZC3LH2C7IU35FTI2NOQ";
    const auto address = Address(stellarAddress);
    ASSERT_EQ(address.string(), stellarAddress);

    EXPECT_ANY_THROW(new Address(""));
}

TEST(StellarAddress, isValid) {
    string stellarAddress = "GABQHYQOY22KHGTCTAK24AWAUE4TXERF4O4JBSXELNM7IL5CTPUWM3SC";
    string bitcoinAddress = "1Ma2DrB78K7jmAwaomqZNRMCvgQrNjE2QC";

    ASSERT_TRUE(Address::isValid(stellarAddress));
    ASSERT_FALSE(Address::isValid(bitcoinAddress));
    ASSERT_FALSE(Address::isValid("qc64537q3cvjmc2cgkz10y58waj4294967296r10ccchmrmrdzq03783"));
}

} // namespace TW::Stellar::tests
