// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transaction.h"

#include "Ethereum/RLP.h"
#include "RLP.h"
#include "proto/EthereumRlp.pb.h"
#include "uint256.h"

using namespace TW;

namespace TW::Aion {

static const uint128_t gTransactionType = 1;

Data Transaction::encode() const noexcept {
    auto nonceData = store(nonce);
    auto amountData = store(amount);
    auto timestampData = store(timestamp);

    EthereumRlp::Proto::EncodingInput input;
    auto* rlpList = input.mutable_item()->mutable_list();

    rlpList->add_items()->set_number_u256(nonceData.data(), nonceData.size());
    rlpList->add_items()->set_data(to.bytes.data(), to.bytes.size());
    rlpList->add_items()->set_number_u256(amountData.data(), amountData.size());
    rlpList->add_items()->set_data(payload.data(), payload.size());
    rlpList->add_items()->set_number_u256(timestampData.data(), timestampData.size());

    *rlpList->add_items() = RLP::prepareLong(gasLimit);
    *rlpList->add_items() = RLP::prepareLong(gasPrice);
    *rlpList->add_items() = RLP::prepareLong(gTransactionType);

    if (!signature.empty()) {
        rlpList->add_items()->set_data(signature.data(), signature.size());
    }

    return Ethereum::RLP::encode(input);
}

} // namespace TW::Aion
