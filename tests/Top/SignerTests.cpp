// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Top/Signer.h"
#include "Top/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Top;

// TODO: Add tests

TEST(TopSigner, Sign) {
    auto from = Top::Address("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    auto to = Top::Address("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    auto transaction = Transaction(from, to, 1, 2, 3, {}, {}, {});

    auto privateKey = PrivateKey(parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62"));
    Signer::sign(privateKey, transaction);
    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    EXPECT_EQ(hex(transaction.signature), "0488227084554fae2b4b270cb56c9d8ee2b6bb1ee06863945ecce5bff9f8a197aa4546799dbe85decd6f3a43f52a2b06f77efd53af8e8530463d27c1b5945608bfd19c2d0bfdeb78585cd9e25e28c870c90ae76309d0877cdd43e874ce8238df545c6ff8d25acb0b09f83ac1d155f87c879bd374fd5ae9f90c51c2680de6d9df3301");
    EXPECT_EQ(hex(transaction.encode()), "f09466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a39900325010203808080");
}

TEST(TopSigner, SignWithData) {
    auto from = Top::Address("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    auto to = Top::Address("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    auto transaction = Transaction(from, to, 1, 2, 3, parse_hex("01020304"), parse_hex("05060708"), parse_hex("090a0b0c"));

    auto privateKey = PrivateKey(parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62"));
    Signer::sign(privateKey, transaction);

    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;

    EXPECT_EQ(hex(transaction.signature), "0488227084554fae2b4b270cb56c9d8ee2b6bb1ee06863945ecce5bff9f8a197aa4546799dbe85decd6f3a43f52a2b06f77efd53af8e8530463d27c1b5945608bf789c10a4b4fecf7cd5556e01dcd17bd36d5b638bbdbb6fe371995745b7205f056f740108cf66f367324a24c2c4a22d659b743a102c6d0f4066aae7b972584c3400");
    EXPECT_EQ(hex(transaction.encode()), "f83c9466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a399003250102038401020304840506070884090a0b0c");
}