// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../proto/NEAR.pb.h"
#include "../Data.h"
#include "../PublicKey.h"

namespace TW::NEAR {

/// Helper class that performs NEAR transaction signing.
class Signer {
  public:
    Proto::SigningInput input;
    Signer() = delete;
    /// Initializes a transaction signer.
    explicit Signer(const Proto::SigningInput& input) : input(input) {}
    /// Signs the given transaction.
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey) const;
    Data signaturePreimage() const;
};

} // namespace TW::NEAR
