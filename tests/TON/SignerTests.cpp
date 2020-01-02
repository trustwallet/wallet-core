// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TON/Signer.h"
#include "TON/Address.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::TON;

TEST(TONSigner, extMsg1)
{
    const auto privkey = PrivateKey(parse_hex("c17bedd2e048e132f3d4d2a690499046fa376ef8a8ac9a488c8470045c05cdd7"));
    const auto pubkey = privkey.getPublicKey(TWPublicKeyTypeED25519);
    EXPECT_EQ(hex(parse_hex("37f14c50f6435b11b9326e1218524f7f072d0a5ea8221cca71682e7d6ed64213")), hex(pubkey.bytes));

    Address addr = Address(pubkey);
    EXPECT_EQ("EQAwfJ7-poPBT1cqkDIIb_uM-BaMXUkJTL_7y_C9wwN5kEVJ", addr.string());

    Data extMsg = Signer::buildInitMessage(privkey);
    EXPECT_EQ(244, extMsg.size());
    EXPECT_EQ(
        "b5ee9c724101030100e50002cf880060f93dfd4d07829eae55206410dff719f02d18ba9212997ff797e17b8606f3201189e486b7a3c126932745507bce5491900755fe9fdde2162d979e07023dd00eaa4396f1cce78f77f409bfcadbc58e3cae723aa4d6134810ebbf1c900c99813bc0a000000010010200a2ff0020dd2082014c97ba9730ed44d0d70b1fe0a4f260810200d71820d70b1fed44d0d31fd3ffd15112baf2a122f901541044f910f2a2f80001d31f3120d74a96d307d402fb00ded1a4c8cb1fcbffc9ed5400480000000037f14c50f6435b11b9326e1218524f7f072d0a5ea8221cca71682e7d6ed64213c59ea7fb",
        hex(extMsg)
    );
}

TEST(TONSigner, extMsg2)
{
    const auto privkey = PrivateKey(parse_hex("6fd2527cc03e1bd77700722308baa6348aef2a0f5c03bdd392ab013123659914"));
    const auto pubkey = privkey.getPublicKey(TWPublicKeyTypeED25519);
    EXPECT_EQ(hex(parse_hex("2d1e01087d4c3cff53b407e2bf90c435296e4012facb759a904d1baa6f749a14")), hex(pubkey.bytes));

    Address addr = Address(pubkey);
    EXPECT_EQ("EQC7I0GX_8EdSZIFHFcag1hrDdIAYip9lBUGuqXIifbGmYGz", addr.string());

    Data extMsg = Signer::buildInitMessage(privkey);
    EXPECT_EQ(244, extMsg.size());
    EXPECT_EQ(
        "b5ee9c724101030100e50002cf88017646832fff823a93240a38ae3506b0d61ba400c454fb282a0d754b9113ed8d321194ce5400aec2a7993af86e4df81d9f99524e9a42a787aff70b717102d295288f95a305c0ff3223aa388dc904ac9383d9cea23f26c374dbd348abf1ec32aa1580c000000010010200a2ff0020dd2082014c97ba9730ed44d0d70b1fe0a4f260810200d71820d70b1fed44d0d31fd3ffd15112baf2a122f901541044f910f2a2f80001d31f3120d74a96d307d402fb00ded1a4c8cb1fcbffc9ed540048000000002d1e01087d4c3cff53b407e2bf90c435296e4012facb759a904d1baa6f749a1439ddf944",
        hex(extMsg)
    );
}
