// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "THORSwap/Swap.h"
#include "HexCoding.h"
#include "proto/Binance.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

namespace TW::THORSwap {

// Addresses for wallet 'isolate dismiss fury ... note'
const auto Address1Btc = "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8";
const auto Address1Eth = "0xB9F5771C27664bF2282D98E09D7F50cEc7cB01a7";
const auto Address1Thor = "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r";
const Data PrivateKey1Btc = parse_hex("13fcaabaf9e71ffaf915e242ec58a743d55f102cf836968e5bd4881135e0c52c");
const Data PrivateKey1Bnb = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");
const auto VaultEth = "0x5d5fa69cace0352bf520377e055a34a9f8f7257c";
const auto VaultBnb = "bnb1ksxqxurvejkndenuv0alqawpr3e4vtqk855aal";

TEST(THORSwap, SwapBtc) {
    auto res = Swap::build(Chain::BTC, Chain::ETH, "ETH", "", Address1Eth, VaultEth, "100000", PrivateKey1Btc);
    EXPECT_TRUE(res.second.length() > 0);
    EXPECT_EQ(res.second, "Invalid from chain: 2");
}

TEST(THORSwap, SwapBnbBtc) {
    auto res = Swap::build(Chain::BNB, Chain::BTC, "BTC", "", Address1Btc, VaultBnb, "10000000", PrivateKey1Bnb);
    ASSERT_EQ(res.second, "");
    EXPECT_EQ(hex(res.first), "0a1242696e616e63652d436861696e2d4e696c652a41535741503a425443204254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a33343330353031313132207105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e52780a3a0a2a626e62317a3533777765376d64366365777a39737177717a6e306161767061756e30677774367179376a120c0a03424e421080a094a58d1d123a0a2a626e62316b73787178757276656a6b6e64656e757630616c71617770723365347674716b38353561616c120c0a03424e421080a094a58d1d");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(res.first.data(), (int)res.first.size()));

    // check fields
    auto pk2 = tx.private_key();
    EXPECT_EQ(hex(TW::data(pk2)), hex(PrivateKey1Bnb));
    EXPECT_EQ(tx.memo(), "SWAP:BTC BTC:bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8:343050111");
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(tx.send_order().inputs(0).address(), "bnb1z53wwe7md6cewz9sqwqzn0aavpaun0gwt6qy7j");
    EXPECT_EQ(tx.send_order().outputs(0).address(), "bnb1ksxqxurvejkndenuv0alqawpr3e4vtqk855aal");

    // sign and encode resulting input
    Binance::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "b102f0625dee0a7c2a2c87fa0a3a0a2a626e62317a3533777765376d64366365777a39737177717a6e306161767061756e30677774367179376a120c0a03424e421080a094a58d1d123a0a2a626e62316b73787178757276656a6b6e64656e757630616c71617770723365347674716b38353561616c120c0a03424e421080a094a58d1d126a0a26eb5ae9872103ed997e396cf4292f5fce5a42bba41599ccd5d96e313154a7c9ea7049de317c771240ed2b085a2f62e05aef8912ed3b7b29043551e96f47eb12a3f809a51fd15db049560d0f0a8a603e1c25be9bb0a488b0f265dda9622ef86596930fafc6264c2aa81a41535741503a425443204254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a333433303530313131");
}

TEST(THORSwap, SwapBnbRune) {
    auto res = Swap::build(Chain::BNB, Chain::THOR, "RUNE", "", Address1Thor, VaultBnb, "10000000", PrivateKey1Bnb);
    ASSERT_EQ(res.second, "");
    EXPECT_EQ(hex(res.first), "0a1242696e616e63652d436861696e2d4e696c652a44535741503a54484f522052554e453a74686f72317a3533777765376d64366365777a39737177717a6e306161767061756e3067773065786e32723a33343330353031313132207105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e52780a3a0a2a626e62317a3533777765376d64366365777a39737177717a6e306161767061756e30677774367179376a120c0a03424e421080a094a58d1d123a0a2a626e62316b73787178757276656a6b6e64656e757630616c71617770723365347674716b38353561616c120c0a03424e421080a094a58d1d");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(res.first.data(), (int)res.first.size()));

    // check fields
    auto pk2 = tx.private_key();
    EXPECT_EQ(hex(TW::data(pk2)), hex(PrivateKey1Bnb));
    EXPECT_EQ(tx.memo(), "SWAP:THOR RUNE:thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r:343050111");
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(tx.send_order().inputs(0).address(), "bnb1z53wwe7md6cewz9sqwqzn0aavpaun0gwt6qy7j");
    EXPECT_EQ(tx.send_order().outputs(0).address(), "bnb1ksxqxurvejkndenuv0alqawpr3e4vtqk855aal");

    // sign and encode resulting input
    Binance::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "b402f0625dee0a7c2a2c87fa0a3a0a2a626e62317a3533777765376d64366365777a39737177717a6e306161767061756e30677774367179376a120c0a03424e421080a094a58d1d123a0a2a626e62316b73787178757276656a6b6e64656e757630616c71617770723365347674716b38353561616c120c0a03424e421080a094a58d1d126a0a26eb5ae9872103ed997e396cf4292f5fce5a42bba41599ccd5d96e313154a7c9ea7049de317c7712405b6456d5c543e2e7e0859ca59f0f84892b5a8e69ed5b0e2df59d7752a0843a6f2bfde065610766e3fce6737c0e4c2ad75540593dac393a7301f331b0701787801a44535741503a54484f522052554e453a74686f72317a3533777765376d64366365777a39737177717a6e306161767061756e3067773065786e32723a333433303530313131");

    // see https://explorer.binance.org/tx/EC8C648C597255EBEADEA68B090161B010BB7ACB3A91ACC604F4EB530AA7BD9B
    // https://viewblock.io/thorchain/tx/C1668A25D351CB5A8C8CEC9EDC6057EBBB8CD68A1ED41B547533874BC0618877
}

} // namespace
