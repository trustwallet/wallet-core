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

TEST(NULSSigner, Sign) {
    Proto::TransactionPurpose purpose;
    purpose.set_private_key("5e60f0d6b921cca8233a33bc604fe201431b5611743acfec0118b58b9e4837bf");
    purpose.set_amount(10000000L);
    purpose.set_remark("转账1nuls");
    purpose.set_from_address("Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn");
    purpose.set_to_address("Nse8piWsnAqxkzdcJqDc9qg5kdWayVmz");
    purpose.set_timestamp(1552821707155L);
    purpose.set_use_max_amount(false);

    auto inputs = std::vector<Proto::TransactionInput>();
    inputs.push_back(
        makeInput("00205a76c6d451e7ebf081ef95f7609fc9cec02e7c1827c5edbcd228cf3a427e3e8d", 0,
                  50000000L, 0, "Nse83VkkjfDZd55ZMsh8RL5SG5zM3jMn"));
    *purpose.mutable_utxos() = {inputs.begin(), inputs.end()};

    auto plan = TransactionBuilder::plan(purpose);

    auto signer = NULS::Signer(plan);
    std::vector<uint8_t> signature = signer.sign();
    std::string signedTransaction = hex(signature);

    ASSERT_EQ(
        signedTransaction,
        "020093e1628b69010be8bdace8b4a6316e756c73ffffffff012300205a76c6d451e7ebf081ef95f7609fc9cec0"
        "2e7c1827c5edbcd228cf3a427e3e8d0080f0fa02000000000000000000000217042301ef1d19f5a36f37a6cde9"
        "5ef2b675d9cf94e836e3809698000000000000000000000017042301e26f42d122f6c43a149a49d63b25dfa8be"
        "2f1caa60d36002000000000000000000006b2103e0761beaecbbeb05a5a514c5b1554ba6c7712fa067ac5e3cbb"
        "11d0b9f9c8139000473045022100906fb3b1422126a9199a52595ffea392a615d3ad785c12b8c0f814665c408a"
        "fa022025a2de8ecc4a3478dc80c15f0f38697caa8f2d6fcf9ecd3985210a5c002fb1a8");
}
