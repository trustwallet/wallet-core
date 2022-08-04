// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"

#include "Ontology/Oep.h"

#include <string>

#include <boost/any.hpp>
#include <gtest/gtest.h>
#include <iostream>

using namespace TW;
using namespace TW::Ontology;

const std::string contractAddr = "2f34b28eb98a1dd901d303f5294c87546fb37fe7";

TEST(OntologyOep4Token, decimals) {
    uint32_t nonce = 3020883377;
    auto tx = Oep(contractAddr).decimals(nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d1b1050fb40000000000000000000000000000000000000000000000000000000000000000000000002000c108646563696d616c7367e77fb36f54874c29f503d301d91d8ab98eb2342f0000", rawTx);
}

TEST(OntologyOep4Token, queryBalance) {
    uint32_t nonce = 4157872545;
    auto address = Address("ANXE3XovCwBH1ckQnPc6vKYiTwRXyrVToD");
    auto tx = Oep(contractAddr).balanceOf(address, nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d1a119d4f700000000000000000000000000000000000000000000000000000000000000000000000036144a03aaf03d12fd4d46bfcc260bda73ecef33b83b51c10962616c616e63654f6667e77fb36f54874c29f503d301d91d8ab98eb2342f0000", rawTx);
}

TEST(OntologyOep4Token, transfer) {
    auto signer1 = Signer(
        PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646")));
    auto signer2 = Signer(
        PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464652")));
    auto toAddress = Address("ARR6PsaBwRttzCmyxCMhL7NmFk1LqExD7L");
    uint32_t nonce = 2232822985;
    uint64_t amount = 1000, gasPrice = 2500, gasLimit = 200;
    auto tx = Oep(contractAddr).transfer(signer1, toAddress, amount, signer2, gasPrice, gasLimit, nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d1c92c1685c409000000000000c80000000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df6"
              "4d02e8031469c329fbb30a490979ea1a6f0b6a3a91235f6bd714fbacc8214765d457c8e3f2b5a1d3c498"
              "1a2e9d2a53c1087472616e7366657267e77fb36f54874c29f503d301d91d8ab98eb2342f000241402b62"
              "b4c6bc07667019e5c9a1fa1b83ca71ee23ddb763446406b1b03706bf50a6180b13e255a08ade7da376df"
              "d34faee7f51c4f0056325fa79aaf7de0ef25d64e2321031bec1250aa8f78275f99a6663688f31085848d"
              "0ed92f1203e447125f927b7486ac41408aa88ae92ea30a9e5059de8594f462af7dfa7545fffa6654e94e"
              "edfb910bcd5452a26d1554d5d980db84d00dd330aab2fc68316660c8ae5af2c806085157e8ce232103d9"
              "fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
              rawTx);
}