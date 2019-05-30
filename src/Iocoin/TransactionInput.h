// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OutPoint.h"
#include "Bitcoin/Script.h"
#include "../Data.h"

//XXXX
#include "../proto/Iocoin.pb.h"
#include <vector>

namespace TW::Iocoin {

/// Iocoin transaction input.
class TransactionInput {
  public:
    /// Reference to the previous transaction's output.
    OutPoint previousOutput;

    /// Computational Script for confirming transaction authorization.
    Bitcoin::Script script;

    uint32_t sequence;
    /// Initializes a transaction input with a previous output, a script and a
    /// sequence number.
    TransactionInput(OutPoint previousOutput, Bitcoin::Script script)
        : previousOutput(std::move(previousOutput)), script(std::move(script)), sequence(4294967295) {}

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;
};

} // namespace TW::Iocoin

/// Wrapper for C interface.
struct TWIocoinTransactionInput {
    TW::Iocoin::TransactionInput impl;
};
