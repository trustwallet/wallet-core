// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Ontology/Ont.h"

#include <string>
#include <gtest/gtest.h>

namespace TW::Ontology::tests {

TEST(OntologyOnt, decimals) {
    uint32_t nonce = 0;
    auto tx = Ont().decimals(nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d100000000000000000000000000000000000000000000000000000000000000000000000000000000"
              "380008646563696d616c731400000000000000000000000000000000000000010068164f6e746f6c6f67"
              "792e4e61746976652e496e766f6b650000",
              rawTx);
}

TEST(OntologyOnt, queryBalance) {
    uint32_t nonce = 0;
    auto address = Address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD");
    auto tx = Ont().balanceOf(address, nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d100000000000000000000000000000000000000000000000000000000000000000000000000000000"
              "4d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f6614000000000000000000"
              "00000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000",
              rawTx);
}

TEST(OntologyOnt, transfer) {
    PrivateKey privateKey1(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"));
    Signer signer1(privateKey1);

    PrivateKey privateKey2(parse_hex("4646464646464646464646464646464646464646464646464646464646464652"));
    Signer signer2(privateKey2);

    auto toAddress = Address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    uint32_t nonce = 0;
    uint64_t amount = 1, gasPrice = 500, gasLimit = 20000;
    auto tx = Ont().transfer(signer1, toAddress, amount, signer2, gasPrice, gasLimit, nonce);
    auto rawTx = tx.serialize();
    auto rawTxHex = hex(rawTx);

    EXPECT_EQ("00d100000000f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df6"
              "7100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94aba"
              "c41aaf3ead76586a7cc8516a7cc86c51c1087472616e7366657214000000000000000000000000000000"
              "00000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b65000241407531e7d5bb9ae138"
              "862585a65c26d624f1a7a61011298809d9ed9cf60d10a4504067dee9d549a836b480c4e48904e28f9b42"
              "dd5fa14376cbb1ef27d931eaea552321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e4"
              "47125f927b7486ac4140bcc6df81d7f2f3143f152c446643ac5bf7910ef90046be8c89818264a11d360d"
              "0576d7b092fabafd0913a67ccf8b2f8e3d2bd708f768c2bb67e2d2f759805608232103d9fd62df332403"
              "d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
              rawTxHex);

    // TODO uncomment when nist256p1 Rust implementation is enabled.
    // EXPECT_EQ("00d100000000f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df6"
    //           "7100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94aba"
    //           "c41aaf3ead76586a7cc8516a7cc86c51c1087472616e7366657214000000000000000000000000000000"
    //           "00000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b65000241407531e7d5bb9ae138"
    //           "862585a65c26d624f1a7a61011298809d9ed9cf60d10a450bf9821152ab657ca4b7f3b1b76fb1d7021a4"
    //           "1d4e05d427b941caa2e9ca783afc2321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e4"
    //           "47125f927b7486ac4140bcc6df81d7f2f3143f152c446643ac5bf7910ef90046be8c89818264a11d360d"
    //           "fa89284e6d054503f6ec59833074d0717fbb23a4afaedbc98bd6f7cba2e2cf49232103d9fd62df332403"
    //           "d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
    //           rawTxHex);
}

// Successfully broadcasted: https://explorer.ont.io/tx/785af64758886099b995e2aed914c56b15ab63c6e0c5acf42b66f3bbc3e95f98
// TEST(OntologyOnt, transferUpdatedSign) {
//     PrivateKey privateKey1(parse_hex("3b2bca95860af1baf5ef55f60167ef59db098b5871617c889f42dee4ffcb0c6f"));
//     Signer signer1(privateKey1);
//
//     PrivateKey privateKey2(parse_hex("3b2bca95860af1baf5ef55f60167ef59db098b5871617c889f42dee4ffcb0c6f"));
//     Signer signer2(privateKey2);
//
//     auto toAddress = Address("AUyL4TZ1zFEcSKDJrjFnD7vsq5iFZMZqT7");
//     uint32_t nonce = 2760697417;
//
//     uint64_t amount = 1, gasPrice = 2500, gasLimit = 20000;
//     auto tx = Ont().transfer(signer1, toAddress, amount, signer2, gasPrice, gasLimit, nonce);
//     auto rawTx = tx.serialize();
//     auto rawTxHex = hex(rawTx);
//
//     // The transaction hex signed by using Rust implementation:
//     EXPECT_EQ("00d149e68ca4c409000000000000204e0000000000007ae1b6c6e44a518f8bcffc44e75236c7a2a8f3c2"
//               "7100c66b147ae1b6c6e44a518f8bcffc44e75236c7a2a8f3c26a7cc81490c44262e0c9740f7b3c0e3d04"
//               "6c106901c72cc46a7cc8516a7cc86c51c1087472616e7366657214000000000000000000000000000000"
//               "00000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140362bc7dd7d005b47"
//               "464a2a2f84edd146e993267ceefb4174ef30d0cba0a1aa42bcd043b2349ba7030c2a3d7700d6653cc921"
//               "d17a9f8942d46a93e84ae7968ed223210332aa8304797de2817ca65ce25aede0b176c3c5a61a20caffaf"
//               "2df7fb6bb0b1b4ac4140362bc7dd7d005b47464a2a2f84edd146e993267ceefb4174ef30d0cba0a1aa42"
//               "bcd043b2349ba7030c2a3d7700d6653cc921d17a9f8942d46a93e84ae7968ed223210332aa8304797de2"
//               "817ca65ce25aede0b176c3c5a61a20caffaf2df7fb6bb0b1b4ac",
//               rawTxHex);
//
//     // The transaction hex signed by using `trezor-crypto`:
//     EXPECT_NE("00d149e68ca4c409000000000000204e0000000000007ae1b6c6e44a518f8bcffc44e75236c7a2a8f3c2"
//               "7100c66b147ae1b6c6e44a518f8bcffc44e75236c7a2a8f3c26a7cc81490c44262e0c9740f7b3c0e3d04"
//               "6c106901c72cc46a7cc8516a7cc86c51c1087472616e7366657214000000000000000000000000000000"
//               "00000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140362bc7dd7d005b47"
//               "464a2a2f84edd146e993267ceefb4174ef30d0cba0a1aa42432fbc4ccb6458fdf3d5c288ff299ac2f3c5"
//               "2933078e5bb08925e27814cc967f23210332aa8304797de2817ca65ce25aede0b176c3c5a61a20caffaf"
//               "2df7fb6bb0b1b4ac4140362bc7dd7d005b47464a2a2f84edd146e993267ceefb4174ef30d0cba0a1aa42"
//               "432fbc4ccb6458fdf3d5c288ff299ac2f3c52933078e5bb08925e27814cc967f23210332aa8304797de2"
//               "817ca65ce25aede0b176c3c5a61a20caffaf2df7fb6bb0b1b4ac",
//               rawTxHex);
// }

} // namespace TW::Ontology::tests
