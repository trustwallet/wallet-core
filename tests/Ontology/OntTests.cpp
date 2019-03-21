// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"

#include "Ontology/Ont.h"

#include <string>
#include <iostream>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Ontology;


TEST(OntologyOnt, queryBalance) {
    std::string address = "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD";
    auto tx = Ont().balanceOf(address);
    auto serializedTx = hex(tx.serialize());
    auto hexInvokeCode = "1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b65";
    ASSERT_EQ(86, serializedTx.find(hexInvokeCode));
}
