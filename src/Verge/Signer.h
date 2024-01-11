// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../Data.h"
#include "../proto/Bitcoin.pb.h"

#include <vector>
#include <optional>
#include <utility>

namespace TW::Verge {

using SigningInput = Bitcoin::Proto::SigningInput;
using SigningOutput = Bitcoin::Proto::SigningOutput;
using TransactionPlan = Bitcoin::Proto::TransactionPlan;
using PreSigningOutput = Bitcoin::Proto::PreSigningOutput;

/// Helper class that performs Verge transaction signing.
class Signer {
public:
    /// Hide default constructor
    Signer() = delete;

    /// Returns a transaction plan (utxo selection, fee estimation)
    static TransactionPlan plan(const SigningInput& input) noexcept;

    /// Signs a SigningInput transaction
    static SigningOutput sign(const SigningInput& input, std::optional<SignaturePubkeyList> optionalExternalSigs = {}) noexcept;

    /// Collect pre-image hashes to be signed
    static PreSigningOutput preImageHashes(const SigningInput& input) noexcept;
};

} // namespace TW::Verge
