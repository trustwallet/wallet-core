// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>

#include "NULS/Address.h"
#include "NULS/Signer.h"
#include "NULS/TransactionBuilder.h"

using namespace TW;
using namespace TW::NULS;

inline auto makeInput(std::string fromHash, int32_t fromIdx, uint64_t amount, uint64_t lock_time,
                      std::string address) {
    auto input = Proto::TransactionInput();
    input.set_from_index(fromIdx);
    input.set_from_hash(fromHash);
    input.set_amount(amount);
    input.set_lock_time(lock_time);
    input.set_address(address);
    return input;
}

TEST(NULSSigner, NoChangeTransfer) {
    Proto::TransactionPurpose purpose;
    purpose.set_private_key("5e60f0d6b921cca8233a33bc604fe201431b5611743acfec0118b58b9e4837bf");
    purpose.set_amount(149900000L);
    purpose.set_remark("转账1nuls");
    purpose.set_from_address("Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn");
    purpose.set_to_address("Nse8piWsnAqxkzdcJqDc9qg5kdWayVmz");
    purpose.set_timestamp(1552821707155L);
    purpose.set_use_max_amount(false);

    auto inputs = std::vector<Proto::TransactionInput>();
    inputs.push_back(
        makeInput("00205a76c6d451e7ebf081ef95f7609fc9cec02e7c1827c5edbcd228cf3a427e3e8d", 0,
                  50000000L, 0, "Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn"));
    inputs.push_back(
        makeInput("00205a76c6d451e7ebf081ef95f7609fc9cec02e7c1827c5edbcd228cf3a427e3e8d", 0,
                  50000000L, 0, "Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn"));
    inputs.push_back(
        makeInput("00205a76c6d451e7ebf081ef95f7609fc9cec02e7c1827c5edbcd228cf3a427e3e8d", 0,
                  50000000L, 0, "Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn"));
    *purpose.mutable_utxos() = {inputs.begin(), inputs.end()};

    auto plan = TransactionBuilder::plan(purpose);

    ASSERT_EQ(plan.change(), 0L);
    ASSERT_EQ(plan.outputs_size(), 1);
    ASSERT_EQ(plan.fee(), 100000L);
    ASSERT_EQ(plan.amount(), 149900000L);
    ASSERT_EQ(plan.available_amount(), 150000000L);
}

TEST(NULSSigner, MaxAmount) {
    Proto::TransactionPurpose purpose;
    purpose.set_private_key("5e60f0d6b921cca8233a33bc604fe201431b5611743acfec0118b58b9e4837bf");
    purpose.set_amount(0L);
    purpose.set_remark("Max transaction size test");
    purpose.set_from_address("Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn");
    purpose.set_to_address("Nse8piWsnAqxkzdcJqDc9qg5kdWayVmz");
    purpose.set_timestamp(1552821707155L);
    purpose.set_use_max_amount(true);

    auto inputs = std::vector<Proto::TransactionInput>();
    inputs.push_back(
        makeInput("00205a76c6d451e7ebf081ef95f7609fc9cec02e7c1827c5edbcd228cf3a427e3e8d", 0,
                  500000000L, 0, "Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn"));
    inputs.push_back(
        makeInput("00205a76c6d451e7ebf081ef95f7609fc9cec02e7c1827c5edbcd228cf3a427e3e8d", 0,
                  1000000000L, 0, "Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn"));
    inputs.push_back(
        makeInput("00205a76c6d451e7ebf081ef95f7609fc9cec02e7c1827c5edbcd228cf3a427e3e8d", 0,
                  50000000L, 0, "Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn"));

    *purpose.mutable_utxos() = {inputs.begin(), inputs.end()};

    auto plan = TransactionBuilder::plan(purpose);

    ASSERT_EQ(plan.change(), 0L);
    ASSERT_EQ(plan.fee(), 100000L);
    ASSERT_EQ(plan.amount(), 1549900000L);
    ASSERT_EQ(plan.available_amount(), 1550000000L);
    ASSERT_EQ(plan.inputs_size(), inputs.size());
    ASSERT_EQ(plan.outputs_size(), 1);
}

TEST(NULSSigner, MaxTransactionSize) {
    Proto::TransactionPurpose purpose;
    purpose.set_private_key("5e60f0d6b921cca8233a33bc604fe201431b5611743acfec0118b58b9e4837bf");
    purpose.set_amount(100000000L);
    purpose.set_remark("Max transaction size test");
    purpose.set_from_address("Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn");
    purpose.set_to_address("Nse8piWsnAqxkzdcJqDc9qg5kdWayVmz");
    purpose.set_timestamp(1552821707155L);
    purpose.set_use_max_amount(true);

    auto inputs = std::vector<Proto::TransactionInput>();
    for (int i = 0; i < 6300; ++i) {
        inputs.push_back(
            makeInput("00205a76c6d451e7ebf081ef95f7609fc9cec02e7c1827c5edbcd228cf3a427e3e8d", 0,
                      50000000L, 0, "Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn"));
    }

    *purpose.mutable_utxos() = {inputs.begin(), inputs.end()};

    auto plan = TransactionBuilder::plan(purpose);

    ASSERT_EQ(plan.change(), 0L);
    ASSERT_EQ(plan.fee(), 30000000L);
    ASSERT_EQ(plan.amount(), 306920000000L);
    ASSERT_EQ(plan.available_amount(), 306950000000L);
    ASSERT_LE(plan.inputs_size(), 6140L);
    ASSERT_EQ(plan.outputs_size(), 1);
}
