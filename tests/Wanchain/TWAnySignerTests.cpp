// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "proto/Ethereum.pb.h"
#include "uint256.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

using namespace TW;

TEST(TWAnySignerWanchain, Sign) {
    auto input = Ethereum::Proto::SigningInput();

    auto chainId = store(uint256_t(1));
    input.set_chain_id(chainId.data(), chainId.size());

    auto nonce = store(uint256_t(9));
    input.set_nonce(nonce.data(), nonce.size());

    auto gasPrice = store(uint256_t(20000000000));
    input.set_gas_price(gasPrice.data(), gasPrice.size());

    auto gasLimit = store(uint256_t(21000));
    input.set_gas_limit(gasLimit.data(), gasLimit.size());

    input.set_to_address("0x3535353535353535353535353535353535353535");

    auto amount = store(uint256_t(1000000000000000000));
    input.set_amount(amount.data(), amount.size());

    auto key = parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646");
    input.set_private_key(key.data(), key.size());

    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeWanchain);

    ASSERT_EQ(hex(output.encoded()), "f86d01098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a0b3efa52659bb3dfcc1356942a5024ca55d742d8a2b551a142c5a0427c11c6272a07f616e839104c8c9c8f83254afc4e5c7010293cd5e471f6756629d17c8392467");

    ASSERT_EQ(hex(output.v()), "25");
    ASSERT_EQ(hex(output.r()), hex(store(uint256_t("81387416166961732074688826827458851755389597777750181844865627349674117325426"))));
    ASSERT_EQ(hex(output.s()), hex(store(uint256_t("57615878674481745412864351256912693004505281517367788339541976444424345429095"))));
}
