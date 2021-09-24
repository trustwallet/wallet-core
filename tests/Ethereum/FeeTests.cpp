// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/Fee.h"
#include "../interface/TWTestUtilities.h"

#include <fstream>
#include <gtest/gtest.h>

using namespace TW::Ethereum;
using namespace TW;

extern std::string TESTS_ROOT;

static nlohmann::json load_json(std::string path) {
    std::ifstream stream(path);
    nlohmann::json json;
    stream >> json;
    return json;
}

TEST(EthereumFee, suggestBaseFeeAndTip) {
    const auto path = TESTS_ROOT + "/Ethereum/Data/eth_feeHistory2.json";
    const auto history = load_json(path);

    auto fee = Fee::suggestFee(history).dump();
    auto expected = R"|(
        {
            "baseFee": "87408740684",
            "maxPriorityFee": "1500000000"
        }
    )|";
    assertJSONEqual(fee, expected);
}
