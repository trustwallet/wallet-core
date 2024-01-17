// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transaction.h"

#include "../Ethereum/RLP.h"

namespace TW::Theta {

using RLP = Ethereum::RLP;

EthereumRlp::Proto::RlpItem prepare(const Coins& coins) noexcept {
    auto thetaWei = store(coins.thetaWei);
    auto tfuelWei = store(coins.tfuelWei);

    EthereumRlp::Proto::RlpItem item;
    auto* rlpList = item.mutable_list();

    rlpList->add_items()->set_number_u256(thetaWei.data(), thetaWei.size());
    rlpList->add_items()->set_number_u256(tfuelWei.data(), tfuelWei.size());

    return item;
}

EthereumRlp::Proto::RlpItem prepare(const TxInput& input) noexcept {
    EthereumRlp::Proto::RlpItem item;
    auto* rlpList = item.mutable_list();

    rlpList->add_items()->set_data(input.address.bytes.data(), input.address.bytes.size());
    *rlpList->add_items() = prepare(input.coins);
    rlpList->add_items()->set_number_u64(input.sequence);
    rlpList->add_items()->set_data(input.signature.data(), input.signature.size());

    return item;
}

EthereumRlp::Proto::RlpItem prepare(const std::vector<TxInput>& inputs) noexcept {
    EthereumRlp::Proto::RlpItem item;
    auto* rlpList = item.mutable_list();

    for (const auto& input : inputs) {
        *rlpList->add_items() = prepare(input);
    }

    return item;
}

EthereumRlp::Proto::RlpItem prepare(const TxOutput& output) noexcept {
    EthereumRlp::Proto::RlpItem item;
    auto* rlpList = item.mutable_list();

    rlpList->add_items()->set_data(output.address.bytes.data(), output.address.bytes.size());
    *rlpList->add_items() = prepare(output.coins);

    return item;
}

EthereumRlp::Proto::RlpItem prepare(const std::vector<TxOutput>& outputs) noexcept {
    EthereumRlp::Proto::RlpItem item;
    auto* rlpList = item.mutable_list();

    for (const auto& output : outputs) {
        *rlpList->add_items() = prepare(output);
    }

    return item;
}

Transaction::Transaction(Ethereum::Address from, Ethereum::Address to,
                         const uint256_t& thetaAmount, const uint256_t& tfuelAmount,
                         uint64_t sequence, const uint256_t& feeAmount /* = 1000000000000*/) {
    auto fee = Coins(0, feeAmount);
    auto coinsInput = Coins(thetaAmount, tfuelAmount + feeAmount);
    auto coinsOutput = Coins(thetaAmount, tfuelAmount);
    auto input = TxInput(from, coinsInput, sequence);
    auto output = TxOutput(to, coinsOutput);

    this->_fee = fee;
    this->inputs.push_back(input);
    this->outputs.push_back(output);
}

Data Transaction::encodePayload() const noexcept {
    const uint64_t txType = 2; // TxSend

    EthereumRlp::Proto::EncodingInput txInput;
    auto* txPropertiesList = txInput.mutable_item()->mutable_list();

    *txPropertiesList->add_items() = prepare(_fee);
    *txPropertiesList->add_items() = prepare(inputs);
    *txPropertiesList->add_items() = prepare(outputs);

    auto txPropertiesEncoded = RLP::encode(txInput);

    Data payload;
    append(payload, RLP::encodeU256(static_cast<uint256_t>(txType)));
    append(payload, txPropertiesEncoded);

    return payload;
}

Data Transaction::encode(const std::string& chainId) const noexcept {
    Data encoded;
    append(encoded, RLP::encodeString(chainId));
    append(encoded, encodePayload());
    return encoded;
}

bool Transaction::setSignature(const Ethereum::Address& address, const Data& signature) noexcept {
    for (auto& input : inputs) {
        if (input.address == address) {
            input.signature = signature;
            return true;
        }
    }
    return false;
}

} // namespace TW::Theta
