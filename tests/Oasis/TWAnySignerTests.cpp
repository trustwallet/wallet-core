// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "proto/Oasis.pb.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Oasis;

TEST(TWAnySignerOasis, Sign) {
    auto input = Proto::SigningInput();
    auto output = Proto::SigningOutput();
    auto& transfer = *input.mutable_transfer();

    transfer.set_gas_price(0);
    transfer.set_gas_amount("0");
    transfer.set_nonce(0);
    transfer.set_to("oasis1qrrnesqpgc6rfy2m50eew5d7klqfqk69avhv4ak5");
    transfer.set_amount("10000000");
    transfer.set_context("oasis-core/consensus: tx for chain a245619497e580dd3bc1aa3256c07f68b8dcc13f92da115eadc3b231b083d3c4");


    auto key = parse_hex("4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0");
    input.set_private_key(key.data(), key.size());

    ANY_SIGN(input, TWCoinTypeOasis);

    EXPECT_EQ("a273756e747275737465645f7261775f76616c7565585ea4656e6f6e636500666d6574686f64707374616b696e672e5472616e7366657263666565a2636761730066616d6f756e74410064626f6479a262746f5500c73cc001463434915ba3f39751beb7c0905b45eb66616d6f756e744400989680697369676e6174757265a26a7075626c69635f6b6579582093d8f8a455f50527976a8aa87ebde38d5606efa86cb985d3fb466aff37000e3b697369676e61747572655840e331ce731ed819106586152b13cd98ecf3248a880bdc71174ee3d83f6d5f3f8ee8fc34c19b22032f2f1e3e06d382720125d7a517fba9295c813228cc2b63170b",
              hex(output.encoded()));
}
