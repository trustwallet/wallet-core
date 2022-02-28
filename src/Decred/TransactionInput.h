// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OutPoint.h"
#include "../Bitcoin/Script.h"
#include "../Data.h"

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

    int64_t valueIn;
    uint32_t blockHeight;
    uint32_t blockIndex = std::numeric_limits<uint32_t>::max();

    /// Computational Script for confirming transaction authorization.
    Bitcoin::Script script;

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;

    /// Encodes the transaction witness information into the provided buffer.
    void encodeWitness(Data& data) const;
};

} // namespace TW::Decred
