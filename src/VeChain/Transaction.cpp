// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transaction.h"

#include "../Ethereum/RLP.h"

namespace TW::VeChain {

using RLP = Ethereum::RLP;

EthereumRlp::Proto::RlpItem prepareClause(const Clause& clause) noexcept {
    auto value = store(clause.value);

    EthereumRlp::Proto::RlpItem item;
    auto* rlpList = item.mutable_list();

    rlpList->add_items()->set_data(clause.to.bytes.data(), clause.to.bytes.size());
    rlpList->add_items()->set_number_u256(value.data(), value.size());
    rlpList->add_items()->set_data(clause.data.data(), clause.data.size());

    return item;
}

EthereumRlp::Proto::RlpItem prepareClauses(const std::vector<Clause>& clauses) noexcept {
    EthereumRlp::Proto::RlpItem item;

    auto* rlpList = item.mutable_list();
    for (const auto& clause : clauses) {
        *rlpList->add_items() = prepareClause(clause);
    }

    return item;
}

Data Transaction::encode() const noexcept {
    EthereumRlp::Proto::EncodingInput input;
    auto* rlpList = input.mutable_item()->mutable_list();

    rlpList->add_items()->set_number_u64(chainTag);
    rlpList->add_items()->set_number_u64(blockRef);
    rlpList->add_items()->set_number_u64(expiration);
    *rlpList->add_items() = prepareClauses(clauses);
    rlpList->add_items()->set_number_u64(gasPriceCoef);
    rlpList->add_items()->set_number_u64(gas);
    rlpList->add_items()->set_data(dependsOn.data(), dependsOn.size());
    rlpList->add_items()->set_number_u64(nonce);
    // Put an empty list - reserved field for backward compatibility.
    rlpList->add_items()->mutable_list();

    if (!signature.empty()) {
        rlpList->add_items()->set_data(signature.data(), signature.size());
    }

    return RLP::encode(input);
}

} // namespace TW::VeChain
