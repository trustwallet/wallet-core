// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "THORSwap/Swap.h"
#include "HexCoding.h"
#include "Coin.h"
#include "proto/Binance.pb.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnySigner.h>
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

namespace TW::THORSwap {

// Addresses for wallet 'isolate dismiss fury ... note'
const auto Address1Btc = "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8";
const auto Address1Eth = "0xB9F5771C27664bF2282D98E09D7F50cEc7cB01a7";
const auto Address1Bnb = "bnb1z53wwe7md6cewz9sqwqzn0aavpaun0gwt6qy7j";
const auto Address1Thor = "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r";
const Data PrivateKey1Btc = parse_hex("13fcaabaf9e71ffaf915e242ec58a743d55f102cf836968e5bd4881135e0c52c");
const Data PrivateKey1Bnb = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");
const auto VaultEth = "0x5d5fa69cace0352bf520377e055a34a9f8f7257c";
const auto VaultBnb = "bnb1ksxqxurvejkndenuv0alqawpr3e4vtqk855aal";


TEST(THORSwap, SwapBtcEth) {
    auto res = Swap::build(Chain::BTC, Chain::ETH, Address1Btc, "ETH", "", Address1Eth, VaultEth, "10000000");
    EXPECT_EQ(res.second, "Invalid from chain: 2");
}

TEST(THORSwap, SwapBnbBtc) {
    auto res = Swap::build(Chain::BNB, Chain::BTC, Address1Bnb, "BTC", "", Address1Btc, VaultBnb, "10000000");
    ASSERT_EQ(res.second, "");
    EXPECT_EQ(hex(res.first), "0a1242696e616e63652d436861696e2d4e696c652a41535741503a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a33343330353031313152480a220a141522e767db6eb19708b0038029bfbd607bc9bd0e120a0a03424e421080ade20412220a14b40c03706cccad36e67c63fbf075c11c73562c16120a0a03424e421080ade204");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(res.first.data(), (int)res.first.size()));

    // check fields
    auto pk2 = tx.private_key();
    EXPECT_EQ(hex(TW::data(pk2)), "");
    EXPECT_EQ(tx.memo(), "SWAP:BTC.BTC:bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8:343050111");
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(hex(tx.send_order().inputs(0).address()), "1522e767db6eb19708b0038029bfbd607bc9bd0e");
    EXPECT_EQ(hex(tx.send_order().outputs(0).address()), "b40c03706cccad36e67c63fbf075c11c73562c16");

    // sign and encode resulting input
    tx.set_private_key(PrivateKey1Bnb.data(), PrivateKey1Bnb.size());
    Binance::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "8102f0625dee0a4c2a2c87fa0a220a141522e767db6eb19708b0038029bfbd607bc9bd0e120a0a03424e421080ade20412220a14b40c03706cccad36e67c63fbf075c11c73562c16120a0a03424e421080ade204126a0a26eb5ae9872103ed997e396cf4292f5fce5a42bba41599ccd5d96e313154a7c9ea7049de317c7712400fea375341e4b6ff558e1326c8e395af4d6b812cb7ae2cd65b09763d25b80a767192db245205d5545a80a42eb0eb3f54009e995029e64e8fc9dde66dd942c5bd1a41535741503a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a333433303530313131");
}

TEST(THORSwap, SwapBnbRune) {
    auto res = Swap::build(Chain::BNB, Chain::THOR, Address1Bnb, "RUNE", "", Address1Thor, VaultBnb, "10000000");
    ASSERT_EQ(res.second, "");
    EXPECT_EQ(hex(res.first), "0a1242696e616e63652d436861696e2d4e696c652a44535741503a54484f522e52554e453a74686f72317a3533777765376d64366365777a39737177717a6e306161767061756e3067773065786e32723a33343330353031313152480a220a141522e767db6eb19708b0038029bfbd607bc9bd0e120a0a03424e421080ade20412220a14b40c03706cccad36e67c63fbf075c11c73562c16120a0a03424e421080ade204");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(res.first.data(), (int)res.first.size()));

    // check fields
    auto pk2 = tx.private_key();
    EXPECT_EQ(hex(TW::data(pk2)), "");
    EXPECT_EQ(tx.memo(), "SWAP:THOR.RUNE:thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r:343050111");
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(hex(tx.send_order().inputs(0).address()), "1522e767db6eb19708b0038029bfbd607bc9bd0e");
    EXPECT_EQ(hex(tx.send_order().outputs(0).address()), "b40c03706cccad36e67c63fbf075c11c73562c16");

    // set private key, sign and encode resulting input
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeBinance, PrivateKey(PrivateKey1Bnb)), Address1Bnb);
    tx.set_private_key(PrivateKey1Bnb.data(), PrivateKey1Bnb.size());
    Binance::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "8402f0625dee0a4c2a2c87fa0a220a141522e767db6eb19708b0038029bfbd607bc9bd0e120a0a03424e421080ade20412220a14b40c03706cccad36e67c63fbf075c11c73562c16120a0a03424e421080ade204126a0a26eb5ae9872103ed997e396cf4292f5fce5a42bba41599ccd5d96e313154a7c9ea7049de317c7712401835c5a54f82badb382a4ab832e284fcebab91422d637ea00a8f246a9bdb5de81de32e404a0ce2debb60e0176cdab941290fde14188a267f49a0688dfd459e1d1a44535741503a54484f522e52554e453a74686f72317a3533777765376d64366365777a39737177717a6e306161767061756e3067773065786e32723a333433303530313131");

    // see https://explorer.binance.org/tx/EC8C648C597255EBEADEA68B090161B010BB7ACB3A91ACC604F4EB530AA7BD9B
    // https://viewblock.io/thorchain/tx/C1668A25D351CB5A8C8CEC9EDC6057EBBB8CD68A1ED41B547533874BC0618877
}

TEST(THORSwap, Memo) {
    EXPECT_EQ(Swap::buildMemo(Chain::BTC, "BNB", "bnb123", 1234), "SWAP:BTC.BNB:bnb123:1234");
}

TEST(THORSwap, WrongFromAddress) {
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, "DummyAddress", "ETH", "", Address1Eth, VaultEth, "100000");
        EXPECT_EQ(res.second, "Invalid from address");
    }
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Btc, "ETH", "", Address1Eth, VaultEth, "100000");
        EXPECT_EQ(res.second, "Invalid from address");
    }
}

TEST(THORSwap, WrongToAddress) {
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Bnb, "ETH", "", "DummyAddress", VaultEth, "100000");
        EXPECT_EQ(res.second, "Invalid to address");
    }
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Bnb, "ETH", "", Address1Btc, VaultEth, "100000");
        EXPECT_EQ(res.second, "Invalid to address");
    }
}

} // namespace
