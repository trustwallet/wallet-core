// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Ontology/Signer.h"

#include <iostream>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Ontology;


TEST(OntologySigner, validity) {
    auto hexPrvKey = "4646464646464646464646464646464646464646464646464646464646464646";
    auto hexPubKey = "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486";
    auto prvKey = Signer::getPrivateKey(hexPrvKey);
    auto pubKey = Signer::getPublicKey(prvKey);
    EXPECT_EQ(hexPrvKey, hex(prvKey.bytes));
    EXPECT_EQ(hexPubKey, hex(pubKey.bytes));
    auto msg = parse_hex("00");
    auto signature = Signer::sign(prvKey, msg);
    std::cout << hex(signature) << std::endl;
    auto result = Signer::verify(pubKey, signature, msg);
    std::cout << std::boolalpha << result << std::endl;
//    auto hexSignature = "015138e0c7d24ad665bd121e6649f9eb3ef920e4ed20ea67af630a383000bbdd1044dbc03e3d0be1f2c2f92f4f5727043b492c31b1cb1a76ea3709b9d056778e6d";
//    result = Signer::verify(pubKey, parse_hex(hexSignature), msg);
//    std::cout << std::boolalpha << result << std::endl;
}