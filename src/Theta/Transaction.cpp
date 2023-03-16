// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "../Ethereum/RLP.h"

namespace TW::Theta {

using RLP = Ethereum::RLP;

Data encode(const Coins& coins) noexcept {
    auto encoded = Data();
    append(encoded, RLP::encode(coins.thetaWei));
    append(encoded, RLP::encode(coins.tfuelWei));
    return RLP::encodeList(encoded);
}

Data encode(const TxInput& input) noexcept {
    auto encoded = Data();
    append(encoded, RLP::encode(input.address.bytes));
    append(encoded, encode(input.coins));
    append(encoded, RLP::encode(input.sequence));
    append(encoded, RLP::encode(input.signature));
    return RLP::encodeList(encoded);
}

Data encode(const std::vector<TxInput>& inputs) noexcept {
    auto encoded = Data();
    for (const auto& input : inputs) {
        append(encoded, encode(input));
    }
    return RLP::encodeList(encoded);
}

Data encode(const TxOutput& output) noexcept {
    auto encoded = Data();
    append(encoded, RLP::encode(output.address.bytes));
    append(encoded, encode(output.coins));
    return RLP::encodeList(encoded);
}

Data encode(const std::vector<TxOutput>& outputs) noexcept {
    auto encoded = Data();
    for (const auto& output : outputs) {
        append(encoded, encode(output));
    }
    return RLP::encodeList(encoded);
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

Data Transaction::encode() const noexcept {
    auto encoded = Data();
    uint16_t txType = 2; // TxSend
    append(encoded, RLP::encode(txType));
    auto encodedData = Data();
    append(encodedData, Theta::encode(_fee));
    append(encodedData, Theta::encode(inputs));
    append(encodedData, Theta::encode(outputs));
    append(encoded, RLP::encodeList(encodedData));
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
