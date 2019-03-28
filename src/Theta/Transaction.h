// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <vector>

#include "Coin.h"
#include "../Data.h"
#include "../Ethereum/Address.h"

namespace TW {
namespace Theta {

class TxInput {
  public:
    Ethereum::Address address;
    Coins coins;
    uint64_t sequence;
    Data signature;

    TxInput() = default;
    TxInput(const Ethereum::Address& address, const Coins& coins, uint64_t sequence)
        : address(address), coins(coins), sequence(sequence) {}
    TxInput(const Ethereum::Address& address, const Coins& coins, uint64_t sequence,
            const std::vector<uint8_t>& signature)
        : address(address), coins(coins), sequence(sequence), signature(signature) {}
};

class TxOutput {
  public:
    Ethereum::Address address;
    Coins coins;

    TxOutput() = default;
    TxOutput(const Ethereum::Address& address, const Coins& coins)
        : address(address), coins(coins) {}
};

class Transaction {
  public:
    Coins fee;
    std::vector<TxInput> inputs;
    std::vector<TxOutput> outputs;

    Transaction() = default;
    Transaction(const Coins& fee, const std::vector<TxInput>& inputs,
                const std::vector<TxOutput>& outputs)
        : fee(fee), inputs(inputs), outputs(outputs) {}

    Transaction(const Ethereum::Address& from, const Ethereum::Address& to,
                const uint256_t& thetaAmount, const uint256_t& tfuelAmount, uint64_t sequence,
                const uint256_t& feeAmount = 1000000000000);

    /// Encodes the transaction
    Data encode() const noexcept;

    /// Sets signature
    bool setSignature(const Ethereum::Address& address, const Data& signature) noexcept;
};

} // namespace Theta
} // namespace TW
