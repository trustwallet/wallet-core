// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include "Ontology/Signer.h"

#include <gtest/gtest.h>

using namespace TW;
namespace TW::Ontology::tests {

TEST(OntologyAccount, validity) {
    auto hexPrvKey = "4646464646464646464646464646464646464646464646464646464646464646";
    auto hexPubKey = "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486";
    auto signer = Signer(PrivateKey(parse_hex(hexPrvKey)));
    auto prvKey = signer.getPrivateKey();
    auto pubKey = signer.getPublicKey();
    EXPECT_EQ(hexPrvKey, hex(prvKey.bytes));
    EXPECT_EQ(hexPubKey, hex(pubKey.bytes));
}

} // namespace TW::Ontology::tests
