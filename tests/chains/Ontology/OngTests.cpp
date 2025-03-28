// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"

#include "Ontology/Ong.h"

#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>
#include <string>

namespace TW::Ontology::tests {

TEST(OntologyOng, decimals) {
    uint32_t nonce = 0;
    auto tx = Ong().decimals(nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d100000000000000000000000000000000000000000000000000000000000000000000000000000000"
              "380008646563696d616c731400000000000000000000000000000000000000020068164f6e746f6c6f67"
              "792e4e61746976652e496e766f6b650000",
              rawTx);
}

TEST(OntologyOng, balanceOf) {
    uint32_t nonce = 0;
    auto address = Address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD");
    auto tx = Ong().balanceOf(address, nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d100000000000000000000000000000000000000000000000000000000000000000000000000000000"
              "4d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f6614000000000000000000"
              "00000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b650000",
              rawTx);
}

TEST(OntologyOng, transfer) {
    PrivateKey privateKey1(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"), TWCoinTypeCurve(TWCoinTypeOntology));
    Signer signer1(privateKey1);

    PrivateKey privateKey2(parse_hex("4646464646464646464646464646464646464646464646464646464646464652"), TWCoinTypeCurve(TWCoinTypeOntology));
    Signer signer2(privateKey2);

    Address toAddress("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    uint32_t nonce = 0;
    uint64_t amount = 1, gasPrice = 500, gasLimit = 20000;

    auto tx = Ong().transfer(signer1, toAddress, amount, signer2, gasPrice, gasLimit, nonce);
    auto rawTx = tx.serialize();
    auto rawTxHex = hex(rawTx);

    EXPECT_EQ("00d100000000f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df6"
              "7100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94aba"
              "c41aaf3ead76586a7cc8516a7cc86c51c1087472616e7366657214000000000000000000000000000000"
              "00000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140ac3edf2d00540f9c"
              "2f3b24878936b409c995c425ab5edf247c5b0d812a50df293ff63e173bac71a6cd0772ff78415c46ac64"
              "f625cbc06fe90ccdecf9a94319c42321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e4"
              "47125f927b7486ac41406fea9f12b125d7f65a94774e765a796428b3c6c4c46b0470624b9a1cef4ff420"
              "488828f308c263b35287363e51add8cd49136eb57a397c6ade95df80d9a16282232103d9fd62df332403"
              "d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
              rawTxHex);

    // TODO uncomment when nist256p1 Rust implementation is enabled.
    // EXPECT_EQ("00d100000000f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df6"
    //           "7100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94aba"
    //           "c41aaf3ead76586a7cc8516a7cc86c51c1087472616e7366657214000000000000000000000000000000"
    //           "00000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140ac3edf2d00540f9c"
    //           "2f3b24878936b409c995c425ab5edf247c5b0d812a50df29c009c1e7c4538e5a32f88d0087bea3b91082"
    //           "0487db572e9be6ebddc953200b8d2321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e4"
    //           "47125f927b7486ac41406fea9f12b125d7f65a94774e765a796428b3c6c4c46b0470624b9a1cef4ff420"
    //           "b777d70bf73d9c4dad78c9c1ae52273273d38bf82cde221a1523eb4222c1c2cf232103d9fd62df332403"
    //           "d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
    //           rawTxHex);
}

TEST(OntologyOng, withdraw) {
    PrivateKey privateKey1(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"), TWCoinTypeCurve(TWCoinTypeOntology));
    Signer signer1(privateKey1);

    PrivateKey privateKey2(parse_hex("4646464646464646464646464646464646464646464646464646464646464652"), TWCoinTypeCurve(TWCoinTypeOntology));
    Signer signer2(privateKey2);

    uint32_t nonce = 0;
    uint64_t amount = 1, gasPrice = 500, gasLimit = 20000;
    auto tx =
        Ong().withdraw(signer1, signer1.getAddress(), amount, signer2, gasPrice, gasLimit, nonce);
    auto rawTx = tx.serialize();
    auto rawTxHex = hex(rawTx);

    EXPECT_EQ(
        "00d100000000f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df68b00c6"
        "6b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc81400000000000000000000000000000000000000"
        "016a7cc814fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc8516a7cc86c0c7472616e7366657246726f"
        "6d1400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f"
        "6b6500024140b8b859055c744a89ef4d4f6ae7a58e0a99fef2eb0f6cf09d740b56cf4c7c14ab64e00c28de9b1f"
        "28921cbd62e6bcd6d452ab9871f8f5d2288812ff322ee2f4af2321031bec1250aa8f78275f99a6663688f31085"
        "848d0ed92f1203e447125f927b7486ac41406413b060329e133cd13709c361ccd90b3944477cf3937f1459313f"
        "0ea6435f6f2b1335192a5d1b346fd431e8af912bfa4e1a23ad7d0ab7fc5b808655af5c9043232103d9fd62df33"
        "2403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
        rawTxHex);

    // TODO uncomment when nist256p1 Rust implementation is enabled.
    // EXPECT_EQ(
    //     "00d100000000f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df68b00c6"
    //     "6b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc81400000000000000000000000000000000000000"
    //     "016a7cc814fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc8516a7cc86c0c7472616e7366657246726f"
    //     "6d1400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f"
    //     "6b6500024140b8b859055c744a89ef4d4f6ae7a58e0a99fef2eb0f6cf09d740b56cf4c7c14ab9b1ff3d62164e0"
    //     "d86de3429d1943292b6a3b623bae21cc5c6ba6cb90cd8030a22321031bec1250aa8f78275f99a6663688f31085"
    //     "848d0ed92f1203e447125f927b7486ac41406413b060329e133cd13709c361ccd90b3944477cf3937f1459313f"
    //     "0ea6435f6f2b1335192a5d1b346fd431e8af912bfa4e1a23ad7d0ab7fc5b808655af5c9043232103d9fd62df33"
    //     "2403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
    //     rawTxHex);
}

} // namespace TW::Ontology::tests
