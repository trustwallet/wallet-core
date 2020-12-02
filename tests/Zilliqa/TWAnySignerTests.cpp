// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "uint256.h"
#include "proto/Zilliqa.pb.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Zilliqa;

TEST(TWAnySignerZilliqa, Sign) {
    auto input = Proto::SigningInput();
    auto &tx = *input.mutable_transaction();
    auto &transfer = *tx.mutable_transfer();
    auto key = parse_hex("0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748");
    auto amount = store(uint256_t(1000000000000));
    auto gasPrice = store(uint256_t(1000000000));

    input.set_version(65537);
    input.set_nonce(2);
    input.set_to("zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz");
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(1);
    input.set_private_key(key.data(), key.size());
    transfer.set_amount(amount.data(), amount.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeZilliqa);

    ASSERT_EQ(hex(output.signature()), "001fa4df08c11a4a79e96e69399ee48eeecc78231a78b0355a8ca783c77c139436e37934fecc2252ed8dac00e235e22d18410461fb896685c4270642738ed268");
}
