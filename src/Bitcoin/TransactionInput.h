// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OutPoint.h"
#include "Script.h"
#include "../Data.h"

#include <vector>

namespace TW {
namespace Bitcoin {

/// Bitcoin transaction input.
class TransactionInput {
  public:
    /// Reference to the previous transaction's output.
    OutPoint previousOutput;

    /// Transaction version as defined by the sender.
    ///
    /// Intended for "replacement" of transactions when information is updated
    /// before inclusion into a block.
    uint32_t sequence;

    /// Computational Script for confirming transaction authorization.
    Script script;

    /// Witness stack.
    std::vector<Data> scriptWitness;

    /// Initializes an empty transaction input.
    TransactionInput() = default;

    /// Initializes a transaction input with a previous output, a script and a
    /// sequence number.
    TransactionInput(const OutPoint& previousOutput, const Script& script, uint32_t sequence)
        : previousOutput(previousOutput), sequence(sequence), script(script) {}

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;

    /// Encodes the witness data into the provided buffer.
    void encodeWitness(Data& data) const;
};

} // namespace Bitcoin
} // namespace TW

/// Wrapper for C interface.
struct TWBitcoinTransactionInput {
    TW::Bitcoin::TransactionInput impl;
};
