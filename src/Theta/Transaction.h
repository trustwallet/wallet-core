// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <string>
#include <vector>

#include "Coins.h"
#include "Data.h"
#include "Ethereum/Address.h"

namespace TW::Theta {

class TxInput {
  public:
    Ethereum::Address address;
    Coins coins;
    uint64_t sequence;
    Data signature;

    TxInput(Ethereum::Address address, Coins coins, uint64_t sequence)
        : address(std::move(address)), coins(std::move(coins)), sequence(sequence) {}
    TxInput(Ethereum::Address address, Coins coins, uint64_t sequence, const Data& signature)
        : address(std::move(address)), coins(std::move(coins)), sequence(sequence), signature(std::move(signature)) {}
};

class TxOutput {
  public:
    Ethereum::Address address;
    Coins coins;

    TxOutput(Ethereum::Address address, Coins coins)
        : address(std::move(address)), coins(std::move(coins)) {}
};

class Transaction {
  public:
    Coins _fee;
    std::vector<TxInput> inputs;
    std::vector<TxOutput> outputs;

    Transaction() = default;
    Transaction(Coins fee, std::vector<TxInput> inputs, std::vector<TxOutput> outputs)
        : _fee(std::move(fee)), inputs(std::move(inputs)), outputs(std::move(outputs)) {}

    Transaction(Ethereum::Address from, Ethereum::Address to,
                const uint256_t& thetaAmount, const uint256_t& tfuelAmount, uint64_t sequence,
                const uint256_t& feeAmount = 1000000000000);

    /// Encodes the essential part of the transaction without a Chain ID.
    Data encodePayload() const noexcept;

    /// Encodes the transaction with the given `chainId`.
    Data encode(const std::string& chainId) const noexcept;

    /// Sets signature
    bool setSignature(const Ethereum::Address& address, const Data& signature) noexcept;
};

} // namespace TW::Theta
