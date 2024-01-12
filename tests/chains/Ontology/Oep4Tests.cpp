// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"

#include "Ontology/Oep4.h"
#include "Ontology/Signer.h"
#include <gtest/gtest.h>
#include <string>

namespace TW::Ontology::tests {

TEST(OntologyOep4, name) {
    std::string wing_hex{"ff31ec74d01f7b7d45ed2add930f5d2239f7de33"};
    auto wing_addr = Address(parse_hex(wing_hex));
    Oep4 wing(wing_addr);

    uint32_t nonce = 0x1234;
    auto tx = wing.name(nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d1341200000000000000000000000000000000000000000000000000000000000000000000000000001c00c1046e616d656733def739225d0f93dd2aed457d7b1fd074ec31ff0000", rawTx);
}

TEST(OntologyOep4, symbol) {
    std::string wing_hex{"ff31ec74d01f7b7d45ed2add930f5d2239f7de33"};
    auto wing_addr = Address(parse_hex(wing_hex));
    Oep4 wing(wing_addr);

    uint32_t nonce = 0x1234;
    auto tx = wing.symbol(nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d1341200000000000000000000000000000000000000000000000000000000000000000000000000001e00c10673796d626f6c6733def739225d0f93dd2aed457d7b1fd074ec31ff0000", rawTx);
}

TEST(OntologyOep4, decimals) {
    std::string wing_hex{"ff31ec74d01f7b7d45ed2add930f5d2239f7de33"};
    auto wing_addr = Address(parse_hex(wing_hex));
    Oep4 wing(wing_addr);

    uint32_t nonce = 0x1234;
    auto tx = wing.decimals(nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d1341200000000000000000000000000000000000000000000000000000000000000000000000000002000c108646563696d616c736733def739225d0f93dd2aed457d7b1fd074ec31ff0000", rawTx);
}

TEST(OntologyOep4, totalSupply) {
    std::string wing_hex{"ff31ec74d01f7b7d45ed2add930f5d2239f7de33"};
    auto wing_addr = Address(parse_hex(wing_hex));
    Oep4 wing(wing_addr);

    uint32_t nonce = 0x1234;
    auto tx = wing.totalSupply(nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d1341200000000000000000000000000000000000000000000000000000000000000000000000000002300c10b746f74616c537570706c796733def739225d0f93dd2aed457d7b1fd074ec31ff0000", rawTx);
}

TEST(OntologyOep4, balanceOf) {
    std::string wing_hex{"ff31ec74d01f7b7d45ed2add930f5d2239f7de33"};
    auto wing_addr = Address(parse_hex(wing_hex));
    Oep4 wing(wing_addr);
    auto user = Address("AeaThtPwh5kAYnjHavzwmvxPd725nVTvbM");

    uint32_t nonce = 0x1234;
    auto tx = wing.balanceOf(user, nonce);
    auto rawTx = hex(tx.serialize());
    EXPECT_EQ("00d1341200000000000000000000000000000000000000000000000000000000000000000000000000003614fa2254ffaee3c3e1172e8e98f800e4105c74988e51c10962616c616e63654f666733def739225d0f93dd2aed457d7b1fd074ec31ff0000", rawTx);
}

TEST(OntologyOep4, addressHack) {
    auto ownerbin = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto payerbin = parse_hex("4646464646464646464646464646464646464646464646464646464646464652");

    PrivateKey owner(ownerbin);
    PrivateKey payer(payerbin);

    auto pubKey = owner.getPublicKey(TWPublicKeyTypeNIST256p1);
    Address addr(pubKey);

    EXPECT_EQ("AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5", addr.string());

    auto payerpub_key = payer.getPublicKey(TWPublicKeyTypeNIST256p1);
    Address payer_addr(payerpub_key);
    EXPECT_EQ("APniYDGozkhUh8Tk7pe35aah2HGJ4fJfVd", payer_addr.string());
}

TEST(OntologyOep4, transfer) {
    PrivateKey fromPrivate(parse_hex("4646464646464646464646464646464646464646464646464646464646464652"));
    Signer from(fromPrivate);

    PrivateKey payerPrivate(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"));
    Signer payer(payerPrivate);

    auto toAddress = Address("AVY6LfvxauVQAVHDV9hC3ZCv7cQqzfDotH");

    uint32_t nonce = 0x1234;
    uint64_t amount = 233;
    uint64_t gasPrice = 2500;
    uint64_t gasLimit = 50000;

    std::string wing_hex{"ff31ec74d01f7b7d45ed2add930f5d2239f7de33"};
    auto wing_addr = Address(parse_hex(wing_hex));
    Oep4 wing(wing_addr);

    auto tx = wing.transfer(from, toAddress, amount, payer, gasPrice, gasLimit, nonce);
    auto rawTxBytes = tx.serialize();
    auto rawTx = hex(rawTxBytes);
    // Transaction Hex tab
    // https://explorer.ont.io/testnet/tx/710266b8d497e794ecd47e01e269e4aeb6f4ff2b01eaeafc4cd371e062b13757
    EXPECT_EQ("00d134120000c40900000000000050c3000000000000fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a4d02e9001496f688657b95be51c11a87b51adfda4ab69e9cbb1457e9d1a61f9aafa798b6c7fbeae35639681d7df653c1087472616e736665726733def739225d0f93dd2aed457d7b1fd074ec31ff00024140bd2923854d7b84b97a107bb3cddf18c8e3dddd2f36b41a1f5f5b23366484daa22871cfb819923fe01e9cb1e9ed16baa2b05c2feb76bcbe2ec125f72701c5e965232103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac41406d638653597774ce45812ea2653250806b657b32b7c6ad3e027ddeba91e9a9da4bb5dacd23dafba868cb31bacb38b4a6ff2607682a426c1dc09b05a1e158d6cd2321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac", rawTx);
    // TODO uncomment when nist256p1 Rust implementation is enabled.
    // EXPECT_EQ(rawTx, "00d134120000c40900000000000050c3000000000000fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a4d02e9001496f688657b95be51c11a87b51adfda4ab69e9cbb1457e9d1a61f9aafa798b6c7fbeae35639681d7df653c1087472616e736665726733def739225d0f93dd2aed457d7b1fd074ec31ff00024140bd2923854d7b84b97a107bb3cddf18c8e3dddd2f36b41a1f5f5b23366484daa2d78e3046e66dc020e1634e1612e9455d0c8acac2305ae0563293d39bfa9d3bec232103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac41406d638653597774ce45812ea2653250806b657b32b7c6ad3e027ddeba91e9a9dab44a2531dc2504589734ce4534c74b58bdc0f3457cd53267331ec5211b0a4e842321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac");
}

TEST(OntologyOep4, transferMainnet) {
    auto from = Signer(
        PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464652")));

    auto payer = Signer(
        PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646")));

    auto toAddress = Address("AUJJhwRNi4RsNfvuexLETxXEb6szu9D5Ad");

    uint32_t nonce = 0x1234;
    uint64_t amount = 233;
    uint64_t gasPrice = 2500;
    uint64_t gasLimit = 50000;

    // wing oep4 mainnet address
    std::string wing_hex{"00c59fcd27a562d6397883eab1f2fff56e58ef80"};
    auto wing_addr = Address(parse_hex(wing_hex));
    Oep4 wing(wing_addr);

    auto tx = wing.transfer(from, toAddress, amount, payer, gasPrice, gasLimit, nonce);
    auto rawTx = hex(tx.serialize());
    // Transaction Hex tab
    // https://explorer.ont.io/tx/70b276aaeb6b4578237390ec339b6a196f4620bdef8df1717032d32576ccef4a
    EXPECT_EQ("00d134120000c40900000000000050c3000000000000fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a4d02e900148962e81f62cb76068b5f204ea5425d64d57147191457e9d1a61f9aafa798b6c7fbeae35639681d7df653c1087472616e736665726780ef586ef5fff2b1ea837839d662a527cd9fc500000241403c3a5e738f99e8f98ac4f59e225e549e2483bb60aee1771ef8ef189255e1670825d6a4c401f2e103348877393d8355c4d295b21fdfaf3dc4fea9b0459f1e1507232103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac41409501ccaab299dc660da9084dd6e8f22658f7687e77319b17b97149c3f023806d04b300baa52874eae57ccde935bb64e2c16c59e00e0efe7086ae93c1153b80722321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac", rawTx);
}

} // namespace TW::Ontology::tests
