// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/Address.h"

#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include "Base58.h"
#include "HDWallet.h"

#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Cardano;
using namespace std;

TEST(CardanoAddress, Validation) {
    // valid V1 address
    ASSERT_TRUE(Address::isValid("DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ"));
    // valid V2 address
    ASSERT_TRUE(Address::isValid("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx"));

    // invalid checksum
    ASSERT_FALSE(Address::isValid("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvm"));
    // random
    ASSERT_FALSE(Address::isValid("hasoiusaodiuhsaijnnsajnsaiussai"));
    // empty
    ASSERT_FALSE(Address::isValid(""));
}

TEST(CardanoAddress, FromString) {
    {
        auto address = Address("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
    }
    {
        auto address = Address("DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ");
        ASSERT_EQ(address.string(), "DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ");
    }
}

/*
TEST(CardanoAddress, DeriveDummy) {
    auto mnemo = "crowd captain hungry tray powder motor coast oppose month shed parent mystery torch resemble index";
    auto wallet = HDWallet(mnemo, "");
    cerr << hex(wallet.getMasterKey(TWCurve::TWCurveED25519).bytes) << endl;
}

TEST(CardanoAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("526d96fffdbfe787b2f00586298538f9a019e97f6587964dc61aae9ad1d7fa23"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    ASSERT_EQ(address.string(), "JBCQYJ2FREG667NAN7BFKH4RFIKPT7CYDQJNW3SNN5Z7F7ILFLKQ346TSU");
}

TEST(AlgorandAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("c2b423afa8b0095e5ae105668b91b2132db4dadbf38acfc64908d3476a00191f"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "YK2CHL5IWAEV4WXBAVTIXENSCMW3JWW36OFM7RSJBDJUO2QADEP5QYVO5I");
}
*/
