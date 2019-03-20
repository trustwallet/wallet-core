// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "Ontology/Account.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace TW;
using namespace TW::Ontology;


TEST(OntologyAccount, validity) {
    auto hexPrvKey = "4646464646464646464646464646464646464646464646464646464646464646";
    auto hexPubKey = "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486";
    auto acct = Account(hexPrvKey);
    auto prvKey = acct.getPrivateKey();
    auto pubKey = acct.getPublicKey();
    EXPECT_EQ(hexPrvKey, hex(prvKey.bytes));
    EXPECT_EQ(hexPubKey, hex(pubKey.bytes));
    auto msg = parse_hex("00");
    auto signature = acct.sign(msg);
    EXPECT_EQ(64, signature.size());
}