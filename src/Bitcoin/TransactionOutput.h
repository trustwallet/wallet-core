// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Amount.h"
#include "Script.h"

#include <memory>

namespace TW {
namespace Bitcoin {

/// Bitcoin transaction output.
struct TransactionOutput {
    /// Transaction amount.
    Amount value;

    /// Usually contains the public key as a Bitcoin script setting up
    /// conditions to claim this output.
    Script script;

    /// Initializes an empty transaction output.
    TransactionOutput() = default;

    /// Initializes a transaction output with a value and a script.
    TransactionOutput(Amount value, const Script& script) : value(value), script(script) {}

    /// Encodes the output into the provided buffer.
    void encode(std::vector<uint8_t>& data) const;
};

} // namespace Bitcoin
} // namespace TW

/// Wrapper for C interface.
struct TWBitcoinTransactionOutput {
    TW::Bitcoin::TransactionOutput impl;
};
