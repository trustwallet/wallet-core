// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "proto/NULS.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "uint256.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::NULS;

TEST(TWAnySignerNULS, Sign) {
    auto privateKey = parse_hex("0x9ce21dad67e0f0af2599b41b515a7f7018059418bab892a7b68f283d489abc4b");
    auto amount = store(uint256_t(10000000));
    auto balance = store(uint256_t(100000000));
    std::string nonce = "0000000000000000";
    Proto::SigningInput input;

    input.set_from("NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H");
    input.set_to("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe");
    input.set_amount(amount.data(), amount.size());
    input.set_chain_id(1);
    input.set_idassets_id(1);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_balance(balance.data(), balance.size());
    input.set_timestamp(1569228280);
    input.set_nonce(nonce.data(), nonce.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNULS);

    EXPECT_EQ(
        hex(output.encoded()),
        "0200f885885d00008c0117010001f7ec6473df12e751d64cf20a8baa7edd50810f8101000100201d9a00000000"
        "00000000000000000000000000000000000000000000000000080000000000000000000117010001f05e787897"
        "1f3374515eabb6f16d75219d887312010001008096980000000000000000000000000000000000000000000000"
        "0000000000000000000000000000692103958b790c331954ed367d37bac901de5c2f06ac8368b37d7bd6cd5ae1"
        "43c1d7e3463044022028019c0099e2233c7adb84bb03a9a5666ece4a5b65a026a090fa460f3679654702204df0"
        "fcb8762b5944b3aba033fa1a287ccb098150035dd8b66f52dc58d3d0843a");
}

TEST(TWAnySignerNULS, SignToken) {
    auto privateKey =
        parse_hex("0x9ce21dad67e0f0af2599b41b515a7f7018059418bab892a7b68f283d489abc4b");
    auto amount = store(uint256_t(10000000));
    auto balance = store(uint256_t(100000000));
    std::string nonce = "0000000000000000";
    std::string asset_nonce = "0000000000000000";
    Proto::SigningInput input;

    input.set_from("NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H");
    input.set_to("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe");
    input.set_amount(amount.data(), amount.size());
    input.set_chain_id(9);
    input.set_idassets_id(1);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_balance(balance.data(), balance.size());
    input.set_timestamp(1569228280);
    input.set_nonce(nonce.data(), nonce.size());
    input.set_asset_nonce(asset_nonce.data(), asset_nonce.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNULS);

    EXPECT_EQ(hex(output.encoded()),
              "0200f885885d0000d20217010001f7ec6473df12e751d64cf20a8baa7edd50810f810900010080969800"
              "000000000000000000000000000000000000000000000000000000000800000000000000000017010001"
              "f7ec6473df12e751d64cf20a8baa7edd50810f8101000100a08601000000000000000000000000000000"
              "0000000000000000000000000000080000000000000000000117010001f05e7878971f3374515eabb6f1"
              "6d75219d8873120900010080969800000000000000000000000000000000000000000000000000000000"
              "000000000000000000692103958b790c331954ed367d37bac901de5c2f06ac8368b37d7bd6cd5ae143c1"
              "d7e346304402204629e7e39708468a405f8d8dd208d0133a686beb5d3ae829b7a2f5867c74480902203d"
              "0dffac8189b1caa9087e480fd57581e8c382cc4e17034b492dd2178dac851d");
}
