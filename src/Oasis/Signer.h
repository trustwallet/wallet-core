// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <utility>


#include "../Data.h"
#include "../PrivateKey.h"
#include "../proto/Oasis.pb.h"
#include "Transaction.h"

namespace TW::Oasis {

/// Helper class that performs Oasis transaction signing.
class Signer {
public:
    Proto::SigningInput input;

    /// Hide default constructor
    Signer() = delete;

    /// Initializes a transaction signer
    explicit Signer(Proto::SigningInput input) : input(input) {};

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs the transaction.
    ///
    /// \returns the transaction signature or an empty vector if there is an
    /// error.
    Data sign(Transaction& tx) const;

    /// Builds a signed transaction.
    ///
    /// \returns the signed transaction data or an empty vector if there is an
    /// error.
    Data build() const;
};

} // namespace TW::Oasis
