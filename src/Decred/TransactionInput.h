// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "OutPoint.h"
#include "../Bitcoin/Script.h"
#include "Data.h"

#include <limits>
#include <vector>

namespace TW::Decred {

/// Decred transaction input.
class TransactionInput {
  public:
    /// Reference to the previous transaction's output.
    OutPoint previousOutput;

    /// Transaction version as defined by the sender.
    ///
    /// Intended for "replacement" of transactions when information is updated
    /// before inclusion into a block.
    uint32_t sequence = std::numeric_limits<uint32_t>::max();

    int64_t valueIn = 0;
    uint32_t blockHeight = 0;
    uint32_t blockIndex = std::numeric_limits<uint32_t>::max();

    /// Computational Script for confirming transaction authorization.
    Bitcoin::Script script;

    TransactionInput() = default;
    /// Initializes a transaction input with a previous output, a script and a
    /// sequence number.
    TransactionInput(OutPoint previousOutput, Bitcoin::Script script, uint32_t sequence)
        : previousOutput(std::move(previousOutput)), sequence(sequence), script(std::move(script)) {}

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;

    /// Encodes the transaction witness information into the provided buffer.
    void encodeWitness(Data& data) const;
};

} // namespace TW::Decred
