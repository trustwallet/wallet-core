// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../Bitcoin/Amount.h"
#include "../Bitcoin/Script.h"
#include "Data.h"

namespace TW::Decred {

/// Decred transaction output.
struct TransactionOutput {
    /// Transaction amount.
    Bitcoin::Amount value;

    /// Transaction output version.
    uint16_t version;

    /// Ppublic key script setting up conditions to claim this output.
    Bitcoin::Script script;

    /// Initializes an empty transaction output.
    TransactionOutput() = default;

    /// Initializes a transaction output with a value and a script.
    TransactionOutput(Bitcoin::Amount value, uint16_t version, Bitcoin::Script script)
        : value(value), version(version), script(std::move(script)) {}

    /// Encodes the output into the provided buffer.
    void encode(Data& data) const;
};

} // namespace TW::Decred
