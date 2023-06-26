// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "proto/IoTeX.pb.h"
#include "../PrivateKey.h"

namespace TW::IoTeX {

/// Helper class that performs IoTeX transaction signing
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    /// Build the compile output
    static Proto::SigningOutput compile(const Proto::SigningInput& input, const Data& signature, const TW::PublicKey& pubKey) noexcept;
  public:
    Proto::SigningInput input;
    Proto::ActionCore action;

    /// Initializes a transaction signer
    Signer(const Proto::SigningInput& input) : input(input) { toActionCore(); }

    /// Signs the transaction
    ///
    /// \returns the transaction signature or an empty vector if there is an error
    Data sign() const;

    /// Builds the signed transaction
    ///
    /// \returns the signed transaction
    Proto::SigningOutput build() const;

    /// Computes the transaction hash
    Data hash() const;
    /// Get PreImage transaction data
    std::string signaturePreimage() const;
  protected:
    /// Converts to proto ActionCore from transaction input
    void toActionCore();
};

} // namespace TW::IoTeX
