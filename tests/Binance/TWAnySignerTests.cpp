// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Binance/Address.h"
#include "proto/Binance.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Binance;

TEST(TWAnySignerBinance, Sign) {
    auto input = Proto::SigningInput();
    input.set_chain_id("Binance-Chain-Nile");
    input.set_account_number(0);
    input.set_sequence(0);
    input.set_source(0);

    auto key = parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832");
    input.set_private_key(key.data(), key.size());

    auto& order = *input.mutable_send_order();

    // bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2
    auto fromKeyhash = parse_hex("40c2979694bbc961023d1d27be6fc4d21a9febe6");
    // bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5
    auto toKeyhash = parse_hex("bffe47abfaede50419c577f1074fee6dd1535cd1");

    {
        auto input = order.add_inputs();
        input->set_address(fromKeyhash.data(), fromKeyhash.size());
        auto inputCoin = input->add_coins();
        inputCoin->set_denom("BNB");
        inputCoin->set_amount(1);
    }

    {
        auto output = order.add_outputs();
        output->set_address(toKeyhash.data(), toKeyhash.size());
        auto outputCoin = output->add_coins();
        outputCoin->set_denom("BNB");
        outputCoin->set_amount(1);
    }

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeBinance);

    ASSERT_EQ(hex(output.encoded()), "b801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679");
}
