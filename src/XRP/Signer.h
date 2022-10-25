// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

    // TW::Data preImageHash() const;
    TW::Data preImage() const;
    Proto::SigningOutput compile(const Data& signatures, const PublicKey& publicKeys) const;
};

} // namespace TW::Ripple
