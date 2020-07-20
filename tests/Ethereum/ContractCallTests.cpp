// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/ContractCall.h"
#include "HexCoding.h"

#include <fstream>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Ethereum;

extern std::string TESTS_ROOT;

TEST(ContractCall, Approval) {
    auto path = TESTS_ROOT + "/Ethereum/Data/erc20.json";
    std::ifstream stream(path);
    nlohmann::json abi;
    stream >> abi;
    auto call = parse_hex("095ea7b30000000000000000000000005aaeb6053f3e94c9b9a09f33669435e7ef1beaed0000000000000000000000000000000000000000000000000000000000000001");
    auto decoded = decodeCall(call, abi);
}
