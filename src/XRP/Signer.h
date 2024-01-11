// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"
#include "Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"

namespace TW::Ripple {

/// Helper class that performs Ripple transaction signing.
class Signer {
  public:
    Proto::SigningInput input;

    Signer() = default;

    /// Initializes a transaction signer.
    explicit Signer(const Proto::SigningInput& input) : input(input) {}

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept;

    /// preImage returns the transaction pre-image without hashing.
    TW::Data preImage() const;

    /// compile returns the final serialized signed transaction.
    Proto::SigningOutput compile(const Data& signatures, const PublicKey& publicKeys) const;

  private:
    static void signPayment(const Proto::SigningInput& input,
                     Proto::SigningOutput& output,
                     Transaction& transaction);

    static void signNfTokenCancelOffer(const Proto::SigningInput& input, Transaction& transaction) noexcept;
};

} // namespace TW::Ripple
