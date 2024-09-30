// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <utility>


#include "Data.h"
#include "../PrivateKey.h"
#include "../proto/Oasis.pb.h"
#include "Transaction.h"

namespace TW::Oasis {

/// Helper class that performs Oasis transaction signing.
class Signer {
private:
    Transaction buildTransfer() const;
    Escrow buildEscrow() const;
    ReclaimEscrow buildReclaimEscrow() const;

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
    template <typename T>
    Data signTransaction(T& tx) const;

    /// Builds a signed transaction.
    ///
    /// \returns the signed transaction data or an empty vector if there is an
    /// error.
    Data build() const;
    Data signaturePreimage() const;
    Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey) const;
};

} // namespace TW::Oasis
