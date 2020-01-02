// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Algorand/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Algorand;

TEST(AlgorandAddress, Validation) {
    // empty address
    ASSERT_FALSE(Address::isValid(""));
    // invalid checksum
    ASSERT_FALSE(Address::isValid("JBCQYJ2FREG667NAN7BFKH4RFIKPT7CYDQJNW3SNN5Z7F7ILFLKQ346TS3"));
    // wrong length
    ASSERT_FALSE(Address::isValid("JBCQYJ2FREG667NAN7BFKH4RFIKPT7CYDQJNW3SNN5Z7F7ILFLKQ346TSU "));
    // Stellar address
    ASSERT_FALSE(Address::isValid("GABQHYQOY22KHGTCTAK24AWAUE4TXERF4O4JBSXELNM7IL5CTPUWM3SC"));

    ASSERT_TRUE(Address::isValid("HXIWBVQGOO6ZWE5NYJO22XMYRUGZ6TGNX2K2EERPT3ZIWPHE5CLJGB2GEA"));
}

TEST(AlgorandAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("526d96fffdbfe787b2f00586298538f9a019e97f6587964dc61aae9ad1d7fa23"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    ASSERT_EQ(address.string(), "JBCQYJ2FREG667NAN7BFKH4RFIKPT7CYDQJNW3SNN5Z7F7ILFLKQ346TSU");
}

TEST(AlgorandAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("c2b423afa8b0095e5ae105668b91b2132db4dadbf38acfc64908d3476a00191f"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "YK2CHL5IWAEV4WXBAVTIXENSCMW3JWW36OFM7RSJBDJUO2QADEP5QYVO5I");
}

TEST(AlgorandAddress, FromString) {
    auto address = Address("PITDOF57RHOVLT37KM7DCXDCETLDL3OA5CBAN7LQ44Z36LGFC27IJ2IQ64");
    ASSERT_EQ(address.string(), "PITDOF57RHOVLT37KM7DCXDCETLDL3OA5CBAN7LQ44Z36LGFC27IJ2IQ64");
}
