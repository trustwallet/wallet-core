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
    EXPECT_EQ(hex(transaction.encode()), "f8b49466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a39900325010203808080b8820488227084554fae2b4b270cb56c9d8ee2b6bb1ee06863945ecce5bff9f8a197aa4546799dbe85decd6f3a43f52a2b06f77efd53af8e8530463d27c1b5945608bfd19c2d0bfdeb78585cd9e25e28c870c90ae76309d0877cdd43e874ce8238df545c6ff8d25acb0b09f83ac1d155f87c879bd374fd5ae9f90c51c2680de6d9df3301");
}

TEST(TopSigner, SignWithData) {
    auto from = Top::Address("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    auto to = Top::Address("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    auto transaction = Transaction(from, to, 1, 2, 3, data(std::string("12345678")), data(std::string("hello world")), data(std::string("top unit test")));

    auto privateKey = PrivateKey(parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62"));
    Signer::sign(privateKey, transaction);

    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;

    EXPECT_EQ(hex(transaction.signature), "0488227084554fae2b4b270cb56c9d8ee2b6bb1ee06863945ecce5bff9f8a197aa4546799dbe85decd6f3a43f52a2b06f77efd53af8e8530463d27c1b5945608bf216da6f9276fb23e718ef5c85bdcc8b63d5f88540b1b68978837e35334beb60d1de3efbe6948bb10636f9f014553b8b0ca5697a66b0c69c9dc0ce893289fce5100");
    EXPECT_EQ(hex(transaction.encode()), "f8d49466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a399003250102038831323334353637388b68656c6c6f20776f726c648d746f7020756e69742074657374b8820488227084554fae2b4b270cb56c9d8ee2b6bb1ee06863945ecce5bff9f8a197aa4546799dbe85decd6f3a43f52a2b06f77efd53af8e8530463d27c1b5945608bf216da6f9276fb23e718ef5c85bdcc8b63d5f88540b1b68978837e35334beb60d1de3efbe6948bb10636f9f014553b8b0ca5697a66b0c69c9dc0ce893289fce5100");
}