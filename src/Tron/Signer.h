// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../PrivateKey.h"
#include "../proto/Tron.pb.h"

namespace TW::Tron {

/// Helper class that performs Tron transaction signing.
class Signer {
  public:
    Proto::SigningInput input;
    Signer() = delete;
    /// Initializes a transaction signer.
    explicit Signer(const Proto::SigningInput& input) : input(input) {}
    /// Signs the given transaction.
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    Proto::SigningOutput compile(const Data& signature) const;
    Data signaturePreimage() const;
};

} // namespace TW::Tron
